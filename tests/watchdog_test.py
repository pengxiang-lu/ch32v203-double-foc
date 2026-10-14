"""Run watchdog feed-policy tests with a host GCC and mocked IWDG registers."""
import argparse
from pathlib import Path
import subprocess
import tempfile

parser = argparse.ArgumentParser()
parser.add_argument('--cc', required=True)
args = parser.parse_args()
root = Path(__file__).resolve().parents[1]
base = next(root.glob('*/CH32*/Software')).parent
source = (base / 'Hardware/watch_dog.c').read_text(encoding='gb18030')
source = source.replace('#include "driver_config.h"', '')
stub = r'''
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#define IWDG_WriteAccess_Enable 0x5555
#define IWDG_Prescaler_256 6
static unsigned feeds, starts, reload_value;
void IWDG_WriteAccessCmd(unsigned v) { assert(v==0x5555); }
void IWDG_SetPrescaler(unsigned v) { assert(v==6); }
void IWDG_SetReload(unsigned v) { reload_value=v; }
void IWDG_ReloadCounter(void) { ++feeds; }
void IWDG_Enable(void) { ++starts; }
'''
tests = r'''
int main(void) {
    feed_dog(); watch_dog_calibration_progress();
    assert(feeds==0);
    watch_dog_init(781);
    assert(reload_value==781 && starts==1 && feeds==1);
    for(int i=0;i<100;i++) feed_dog();
    assert(feeds==1);
    watch_dog_control_progress();
    assert(feeds==1); /* Interrupt alone never feeds. */
    feed_dog(); assert(feeds==2);
    feed_dog(); assert(feeds==2);
    watch_dog_control_progress(); watch_dog_control_progress();
    feed_dog(); assert(feeds==3);
    for(int i=0;i<100;i++) feed_dog();
    assert(feeds==3); /* Stalled control cannot be masked by main loop. */
    watch_dog_control_progress();
    watch_dog_calibration_progress(); assert(feeds==4);
    feed_dog(); assert(feeds==4); /* Discard pre-calibration progress. */
    watch_dog_control_progress(); feed_dog(); assert(feeds==5);
    control_progress=UINT32_MAX; last_fed_progress=UINT32_MAX;
    watch_dog_control_progress(); feed_dog(); assert(feeds==6);
    watch_dog_init(65535); assert(reload_value==4095);
    watch_dog_init(0); assert(reload_value==0);
    puts("Watchdog policy tests passed");
}
'''
with tempfile.TemporaryDirectory(prefix='watchdog_test_') as temp:
    c = Path(temp) / 'test.c'
    exe = Path(temp) / 'test.exe'
    c.write_text(stub + source + tests, encoding='utf-8')
    subprocess.run([args.cc, '-std=c99', '-Wall', '-Wextra', '-Werror',
                    str(c), '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
