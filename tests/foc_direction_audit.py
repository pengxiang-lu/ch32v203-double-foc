"""Host-side direction audit of the actual FOC C functions (no hardware access).

Run with Python and --cc <host gcc>. Firmware sources are read without editing.
PWM phase comparisons remove common-mode voltage. Current tests use balanced
synthetic phase currents; they cannot verify ADC wiring or sensor polarity.
"""
import argparse
import math
import re
from pathlib import Path
import subprocess
import tempfile

parser = argparse.ArgumentParser()
parser.add_argument('--cc', required=True)
args = parser.parse_args()
root = Path(__file__).resolve().parents[1]
base = next(root.glob('*/CH32*/Software'))
header = (base / 'foc.h').read_bytes().decode('gb18030')
foc = (base / 'foc.c').read_bytes().decode('gb18030')
foc = '\n'.join(line for line in foc.splitlines() if not line.startswith('#include'))
current = (base / 'inline_current.c').read_bytes().decode('gb18030')
current = current[current.index('void current_duty_calculate'):]
control = (base / 'motor_control.c').read_bytes().decode('gb18030')
zero_statement = re.search(r'motor_p->zero_location = normalize\([\s\S]*?\);', control).group()
poles_statement = re.search(r'motor_p->pole_pairs = \(int16_t\)[^;]+;', control).group()
calibration = r'''
#define func_abs(x) ((x)>=0?(x):-(x))
static int check_calibration(void) {
    struct {int32_t zero_location, rotation_direction, pole_pairs;} motor, *motor_p=&motor;
    int cases=0;
    for(int s=-1;s<=1;s+=2) for(int p=1;p<=16;p++)
    for(int raw=0;raw<ENCODER_PRECISION;raw++) {
        motor.rotation_direction=s; motor.pole_pairs=p; motor.zero_location=raw;
        ZERO_STATEMENT
        if(normalize(s*p*raw-motor.zero_location)!=ENCODER_PRECISION/4) return 1;
        cases++;
    }
    for(int s=-1;s<=1;s+=2) {
        int32_t encoder_data_integral=s*2341;
        POLES_STATEMENT
        if(motor.pole_pairs!=7) return 1;
    }
    printf("Calibration invariant: %d cases passed; pole-pair rounding passed\n",cases);
    return 0;
}
'''.replace('ZERO_STATEMENT', zero_statement).replace('POLES_STATEMENT', poles_statement)

prelude = r'''
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#define ENCODER_OUTPUT_MAX 16383
#define ENCODER_PRECISION 16384
#define OUTPUT_DUTY_MAX 4000
#define MOTOR_DUTY_MAX 4096
#define MOTOR_DUTY_BIT 12
#define SPWM 0
#define SVPWM_7 1
#define SVPWM_5 2
#define SENSOR_A_B 0
#define SENSOR_A_C 1
#define SENSOR_B_C 2
#define SENSOR_A_B_C 3
typedef struct {int32_t i_a,i_b,i_c,i_q,i_d;} current_struct;
'''
main = r'''
int main(void) {
    if(check_calibration()) return 2;
    double phase_error=0, q_error=0, d_error=0;
    int samples=0;
    const int inputs[][2]={{700,0},{-700,0},{0,700},{0,-700},{500,300},{-500,300}};
    for(int a=0;a<16384;a+=16) for(int j=0;j<6;j++) {
        int q=inputs[j][0], d=inputs[j][1];
        double t=a*2*3.141592653589793/16384;
        double alpha=d*cos(t)-q*sin(t), beta=d*sin(t)+q*cos(t);
        foc_struct f={0};
        foc_init(&f,1,0,1);
        foc_calculate(&f,a,q,d);
        double pa=f.output_duty[0], pb=f.output_duty[1], pc=f.output_duty[2];
        double oa=(2*pa-pb-pc)/3, ob=(pb-pc)/sqrt(3);
        double e=fabs(atan2(oa*beta-ob*alpha,oa*alpha+ob*beta))*180/3.141592653589793;
        if(e>phase_error) phase_error=e;
        current_struct c={0};
        c.i_a=lround(alpha);
        c.i_b=lround(-alpha/2+sqrt(3)*beta/2);
        c.i_c=lround(-alpha/2-sqrt(3)*beta/2);
        current_duty_calculate(&f,&c);
        if(fabs(c.i_q-q)>q_error) q_error=fabs(c.i_q-q);
        if(fabs(c.i_d-d)>d_error) d_error=fabs(c.i_d-d);
        samples++;
    }
    printf("PWM=%d sensor=%d samples=%d phase_error_deg=%.4f iq_error=%.1f id_error=%.1f\n",
           PWM_MODE,CURRENT_SENSOR_TYPE,samples,phase_error,q_error,d_error);
    return (phase_error>1 || q_error>4 || d_error>4);
}
'''
failures = []
with tempfile.TemporaryDirectory(prefix='foc_audit_') as temp:
    source = Path(temp) / 'audit.c'
    source.write_text(prelude + header + foc + current + calibration + main, encoding='utf-8')
    for pwm, sensor in [(1, 1), (0, 1), (2, 1), (1, 0), (1, 2), (1, 3)]:
        exe = Path(temp) / f'audit_{pwm}_{sensor}.exe'
        built = subprocess.run([
            args.cc, '-std=c99', '-O2', f'-DPWM_MODE={pwm}',
            f'-DCURRENT_SENSOR_TYPE={sensor}', str(source), '-lm', '-o', str(exe)
        ], capture_output=True, text=True)
        if built.returncode:
            print(built.stderr)
            failures.append((pwm, sensor, 'compile'))
            continue
        result = subprocess.run([str(exe)], capture_output=True, text=True)
        print(result.stdout.strip())
        if result.returncode:
            failures.append((pwm, sensor, 'numerical comparison'))

# At Uq>0, Ud=0 and commanded angle=0, ideal alignment is +90 electrical deg.
print('Old calibration: computed angle at alignment; expected 90 electrical deg')
for poles in [5, 7, 8, 14]:
    for direction in [1, -1]:
        estimated = direction * poles * 90 % 360
        error = (estimated - 90 + 180) % 360 - 180
        print(f'poles={poles}, dir={direction:+d}: angle={estimated}, '
              f'error={error:+d}, ideal torque factor={math.cos(math.radians(error)):+.2f}')
print('Transform failures:', failures)
raise SystemExit(bool(failures))
