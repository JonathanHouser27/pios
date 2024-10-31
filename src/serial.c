#include "serial.h"

int putc(int data) {
  volatile unsigned int *mu_io = (volatile unsigned int *) 0x3F215040;
  *mu_io = (unsigned int)data;
  return data;
}
