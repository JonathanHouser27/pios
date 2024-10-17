#include "rprintf.h"
#include "rprintf.c"
#include "serial.c"
#include "mmu.h"
#include "mmu.c"


char glbl[128];

char huge_array[8192];

unsigned int el;

unsigned int getEL(){
  asm("mrs %0, CurrentEL"
    :"=r"(el)
    :
    :);
  return el>>2;
}

void print_execution_level() {
  el = getEL();
  esp_printf(putc, "Current Execution Level is %d\r\n", el);
}

// function to retrieve the counter
unsigned long get_timer_count() {
  // point to the counter in memory
  unsigned long *timer_count_register = 0x3f003004;
  return *timer_count_register; // return the count
}


// function to wait for 1ms
void wait_1ms() {
  // set the start time to the count from memory
  unsigned long start_time = get_timer_count();
  // call the count from memory and once that - start_time < 1000
  // then 1ms has elapsed, end the loop and continue the program
  while ((get_timer_count() - start_time) < 1000) {

  }
}


// function to call for the timer method, wait 1ms, and
void kernel_main() {

  print_execution_level();

  unsigned long timer_value = get_timer_count();
  wait_1ms();
  timer_value = get_timer_count();

  extern int __bss_start, __bss_end;
  char *bssstart, *bssend, *i;

  bssstart = &__bss_start;
  bssend = &__bss_end;

  // for loop to go throughout the bss and change the values to 0
  for(*i = *bssstart; *i <= *bssend; i++){
   *i = 0;
  }

  // memory mapping: map va to pa
  void *virtual_address = (void *)0x40000000;
  void *physical_address = (void *)0x3F000000;

  mapPages(virtual_address, physical_address);

  // enable MMU
  mmu_on();


  while(1) {

  }
}

