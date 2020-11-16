#include "timing.h"
#include "memory.c"


void timing(double* wcTime, double* cpuTime)
{
   struct timeval tp;
   struct rusage ruse;
   setlocale(LC_NUMERIC, "");
   gettimeofday(&tp, NULL);
   *wcTime=(double) (tp.tv_sec + tp.tv_usec/1000000.0); 
  
   getrusage(RUSAGE_SELF, &ruse);
   *cpuTime=(double)(ruse.ru_utime.tv_sec+ruse.ru_utime.tv_usec / 1000000.0);

   //*majorPft = ruse.ru_majflt;
   long major = ruse.ru_majflt;
   long minor = ruse.ru_minflt;
   printf("************************************************************\n");
   printf("# Total page fault = %'ld; (major/minor) = (%'ld/%'ld)\n", 
            major + minor, major, minor);
}

