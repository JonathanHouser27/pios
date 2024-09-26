#include <stdint.h>

void putc(int data) {
  volatile uint32_t *mu_io = (volatile uint32_t *) 0x3F215040;
  *mu_io = (uint32_t)data;
}
