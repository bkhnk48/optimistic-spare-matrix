 #include <time.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include "timing.c"

 int binarySearch(int *array, int number_of_elements, int key) {
         int low = 0, high = number_of_elements-1, mid;
         while(low <= high) {
                 mid = (low + high)/2;
            #ifdef DO_PREFETCH
            // low path
            __builtin_prefetch (&array[(mid + 1 + high)/2], 0, 1);
            // high path
            __builtin_prefetch (&array[(low + mid - 1)/2], 0, 1);
            #endif

                 if(array[mid] < key)
                         low = mid + 1; 
                 else if(array[mid] == key)
                         return mid;
                 else if(array[mid] > key)
                         high = mid-1;
         }
         return -1;
 }
 int main() {
     int SIZE = 1024*1024*512;
     int *array =  malloc(SIZE*sizeof(int));
     for (int i=0;i<SIZE;i++){
       array[i] = i;
     }
     int NUM_LOOKUPS = 1024*1024*8;
     srand(time(NULL));
     int *lookups = malloc(NUM_LOOKUPS * sizeof(int));
     for (int i=0;i<NUM_LOOKUPS;i++){
       lookups[i] = rand() % SIZE;
     }
     
     double wc1, wc2, cpu;
     
     timing(&wc1, &cpu);
     for (int i=0;i<NUM_LOOKUPS;i++){
       int result = binarySearch(array, SIZE, lookups[i]);
     }
     timing(&wc2, &cpu);
     #ifdef DO_PREFETCH
	printf("\nBuiltin Prefetch quicksort version");
     #else
	printf("\nNomarl quicksort version");
     #endif
     printf("=> Execution time = %f ms\n", (wc2 - wc1)*1000);
     printf("=====================\n");
     free(array);
     free(lookups);
 }

