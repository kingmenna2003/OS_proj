#include "types.h"
#include "fcntl.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "datetime.h"


extern uint64 kbd_int_count;

#ifndef BOOT_EPOCH
#define BOOT_EPOCH 0
#endif


uint64
sys_kbdint(void){
  return kbd_int_count;
}

extern int countsyscall;

uint64
sys_countsyscall(void){
  return countsyscall;
}

extern uint ticks; // already declared in kernel/proc.c

static uint64 seed = 1;

uint64
lcg_rand()
{
  seed = (1103515245 * seed + 12345) & 0x7fffffff;
  return seed;
}

uint64
sys_sysrand(void)
{
  seed += ticks; // use ticks to update the seed slightly
  return lcg_rand();
}


uint64
sys_datetime(void) {
  struct datetime dt;
  uint64 addr;

     // Get user buffer address with error checking
     // Get user buffer address
  argaddr(0, &addr);
     // Read hardware timer (mtime)
  volatile uint64 mtime_reg = (volatile uint64)(CLINT_MTIME + KERNBASE);
  uint64 ticks = mtime_reg;

     // Convert to seconds since boot (10MHz clock)
  uint64 seconds_since_boot = ticks / 10000000;
  uint64 current_epoch = BOOT_EPOCH + seconds_since_boot;

     // Convert epoch to date/time components
  uint64 rem_seconds = current_epoch % 86400;
  dt.hour = rem_seconds / 3600;
  dt.minute = (rem_seconds % 3600) / 60;
  dt.second = rem_seconds % 60;

     // Date calculation
  uint64 days = current_epoch / 86400;
  dt.year = 1970;

     // Year calculation with leap years
  while(1) {
    int leap = (dt.year % 4 == 0 && dt.year % 100 != 0) || (dt.year % 400 == 0);
    int year_days = leap ? 366 : 365;
    if(days < year_days) break;
    days -= year_days;
    dt.year++;
  }

     // Month/day calculation
  int days_in_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};
  if((dt.year % 4 == 0 && dt.year % 100 != 0) || dt.year % 400 == 0) {
      days_in_month[1] = 29;
    }

  dt.month = 0;
  while(dt.month < 12 && days >= days_in_month[dt.month]) {
    days -= days_in_month[dt.month];
    dt.month++;
    }
  dt.month += 1;  // Convert to 1-based month
  dt.day = days + 1;  // Convert to 1-based day

     // Copy to userspace
  if(copyout(myproc()->pagetable, addr, (char*)&dt, sizeof(dt)) < 0)
    return -1;

     return 0;
}
