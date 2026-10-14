#ifndef __WATCH_DOG_H
#define __WATCH_DOG_H
#include <stdint.h>
#define WATCH_DOG_RELOAD_5S 781U
void watch_dog_init(uint16_t reload);
// Called only after a complete motor control cycle; never feeds directly.
void watch_dog_control_progress(void);
// Main-loop feed requires new control progress.
void feed_dog(void);
// Explicit checkpoints in blocking calibration, with control IRQ suspended.
void watch_dog_calibration_progress(void);
#endif
