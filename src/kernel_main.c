#include "rprintf.h"
#include "serial.h"
#include "mmu.h"
#include "list.h"
#include "delays.h"
#include "fat.h"

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

void kernel_main() {

  if(fatInit() != 0) {
    esp_printf(putc, "FAT initialization failed!\n");
    return;
  }
  esp_printf(putc, "FAT filesystem initialized successfully.\n");

  struct root_directory_entry rde;
  if(fatOpen("TEST    TXT", &rde) == 0){
    char buf[512]; // Buffer to hold read data

    int bytes_read = fatRead(&rde, buf, sizeof(buf)); // Read data
    if (bytes_read > 0) {
      buf[bytes_read] = '\0'; // Null-terminate
      esp_printf(putc, "Read %d bytes: %s\n", bytes_read, buf);
      } else {
        esp_printf(putc, "Failed to read file\n");
      }
  } else {
    esp_printf(putc, "File not found\n");
  }

  print_execution_level();
  // call to the delays file with the wait_msec fuction
  wait_msec(5);

  extern int __bss_start, __bss_end;
  char *bssstart, *bssend;

  bssstart = (char *)&__bss_start;
  bssend = (char *)&__bss_end;

  // for loop to go throughout the bss and change the values to 0
  for(char *i = bssstart; i <= bssend; i++){
   *i = 0;
  }

  // memory mapping: map va to pa
  void *virtual_address = (void *)0x00000000;
  void *physical_address = (void *)0x00200000;

  mapPages(virtual_address, physical_address);

  // enable MMU
  mmu_on();


  while(1) {

  }
}
