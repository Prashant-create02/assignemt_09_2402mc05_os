#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  printf(1, "=== pttest: Page Table at Start-up ===\n");
  vmprint();

  printf(1, "\n=== Calling sbrk(4 * 4096) ===\n");
  char *mem = sbrk(4 * 4096);
  if(mem == (char*)-1){
    printf(1, "sbrk failed\n");
    exit();
  }

  // Touch each of the 4 pages so they are mapped even when Q2 lazy allocation is active
  for(int i = 0; i < 4; i++){
    mem[i * 4096] = (char)i;
  }

  printf(1, "=== pttest: Page Table After sbrk(4 * 4096) ===\n");
  vmprint();

  exit();
}
