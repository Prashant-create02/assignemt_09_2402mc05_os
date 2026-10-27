#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int before = getfaults();
  printf(1, "=== faulttest: Testing Lazy Page Allocation ===\n");

  char *base = sbrk(32 * 4096);
  if(base == (char*)-1){
    printf(1, "sbrk failed\n");
    exit();
  }

  // Write to exactly 5 distinct pages out of the 32 lazily allocated pages
  int target_pages[5] = {0, 4, 11, 19, 31};
  for(int i = 0; i < 5; i++){
    base[target_pages[i] * 4096] = 'A' + i;
  }

  int after = getfaults();
  printf(1, "Page faults handled after touching 5 pages: %d (Expected: 5)\n",
         after - before);

  printf(1, "\n=== Testing Out-of-Bounds Access in Child Process ===\n");
  int pid = fork();
  if(pid == 0){
    // Child accesses an address well above myproc()->sz
    printf(1, "Child (pid %d) attempting invalid access above sz...\n", getpid());
    char *invalid_ptr = base + (40 * 4096);
    *invalid_ptr = 'X';
    printf(1, "ERROR: Child survived invalid access!\n");
    exit();
  } else if(pid > 0){
    wait();
    printf(1, "Parent (pid %d) survived! Only the offending child process was killed.\n",
           getpid());
  }

  exit();
}
