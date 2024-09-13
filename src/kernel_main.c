char glbl[128];

char huge_array[8192];


// function to retrieve the counter
unsigned long get_timer_count() {

  unsigned long *timer_count_register = 0x3f003004;

  return *timer_count_register;
}


// function to wait for 1ms
void wait_1ms() {

  unsigned long start_time = get_timer_count();

  while ((get_timer_count() - start_time) < 1000) {

  }
}


// function to call for the timer method, wait 1ms, and
void kernel_main() {

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

  while(1) {

  }
}

