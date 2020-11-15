#include "timing.h"


void timing(double* wcTime, double* cpuTime)
{
   struct timeval tp;
   struct rusage ruse;

   gettimeofday(&tp, NULL);
   *wcTime=(double) (tp.tv_sec + tp.tv_usec/1000000.0); 
  
   getrusage(RUSAGE_SELF, &ruse);
   *cpuTime=(double)(ruse.ru_utime.tv_sec+ruse.ru_utime.tv_usec / 1000000.0);
}

void mem_avail(void)
{
  char *cmd = "awk '{ if (NR == 2) { print $4 }}' /proc/meminfo";
  
  FILE *cmdfile = popen(cmd, "r");
  char result[256] = { 0 };
  
  while (fgets(result, sizeof(result), cmdfile) != NULL) {
    printf("%s\n", result);
  }
  
  pclose(cmdfile);
}