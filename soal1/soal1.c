#include <stdio.h>
#include <string.h>
#include <pthread.h>

int toPrint = -1;
int ids[100];

void swap(int* a, int* b) 
{ 
    int t = *a; 
    *a = *b; 
    *b = t; 
} 

int partition (int arr[], int low, int high) 
{ 
    int pivot = arr[high];    // pivot 
    int i = (low - 1);  // Index of smaller element 
  
    for (int j = low; j <= high- 1; j++) 
    { 
        // If current element is smaller than or 
        // equal to pivot 
        if (arr[j] <= pivot) 
        { 
            i++;    // increment index of smaller element 
            swap(&arr[i], &arr[j]); 
        } 
    } 
    swap(&arr[i + 1], &arr[high]); 
    return (i + 1); 
} 

void quickSort(int arr[], int low, int high) 
{ 
    if (low < high) 
    { 
        /* pi is partitioning index, arr[p] is now 
           at right place */
        int pi = partition(arr, low, high); 
  
        // Separately sort elements before 
        // partition and after partition 
        quickSort(arr, low, pi - 1); 
        quickSort(arr, pi + 1, high); 
    } 
} 

void* factorial(void *targ){
    unsigned long long int retVal = 1;
    int * input = (int*)targ;
    for(int i=2; i<=*input; i++){
        retVal *= i;
    }
    while(toPrint != *input);
    printf("%d! is %llu\n",*input,retVal);
}

int main( int argc, char *argv[] )  {
    pthread_t tids[argc];
    int nums[argc];
    int sorted[argc];
    printf("number of threads: %d\n",argc-1);
    memset(nums, 0, sizeof(nums));
    for (size_t i=1; i<argc; i++){
        for(size_t j = 0; j < strlen(argv[i]); j++)
        {
            nums[i] = nums[i]*10 + argv[i][j]-'0';
        }
        ids[nums[i]] = i;
        sorted[i] = nums[i];
        if(pthread_create(&(tids[i]), NULL, factorial, (void*)&nums[i])){
            printf("cant create thread %lu\n",i+1);
        }
    }
    quickSort(sorted, 1, argc-1); 
    for(size_t i = 1; i < argc; i++)
    {
        toPrint = sorted[i];
        pthread_join(tids[ids[toPrint]], NULL);
    }

    return 0;
}