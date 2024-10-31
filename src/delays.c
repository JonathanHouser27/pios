#include "delays.h"


// retrieve the time counter
unsigned long get_timer_count() {
  // point to counter in memory
  volatile unsigned long *timer_count_register = (volatile unsigned long*)0x3f003004;
  return *timer_count_register; // return the count
}

// function to insert 'nop' into the cpu for as many cycles as desired
void wait_cycles(unsigned int number_of_cycles) {
  while (number_of_cycles > 0) {
    __asm__("nop"); // assembly inst for no operation to bypass a cycle
    number_of_cycles--;
  }
}


// function to wait for 'n' milliseconds
void wait_msec(unsigned long ms) {
  // set start time to the count from memory
  unsigned long start_time = get_timer_count();
  //wait until 1ms has elapsed
  unsigned long wait_time = 1000 * ms;
  while(get_timer_count() - start_time < wait_time) {
    // wait
  }
}
