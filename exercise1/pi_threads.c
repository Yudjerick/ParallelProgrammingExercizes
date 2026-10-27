#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef struct {
    long long first;
    long long last;
} ThreadArgs;

void* calc_pi_part(void* arg){
    ThreadArgs* data = (ThreadArgs*)arg;
    double* result = malloc(sizeof(double));
    *result = 0.0;
    int current_mult;
    if(data->first % 2 == 0){
        current_mult = 1;
    }
    else{
        current_mult = -1;
    }
    for (long long i = data->first; i <= data->last; i++)
    {
        *result += current_mult / (double)(2*i+1);
        current_mult *= -1;
    }
    return (void*)result;
}

int main(int argc, char *argv[]) {
    if(argc != 3){
        printf("Invalid arguments\n");
        return 0;
    }
    clock_t begin = clock();
    long long it_count = strtoll(argv[1], NULL, 10);
    int thread_count = atoi(argv[2]);

    pthread_t* thread_ids = (pthread_t*)malloc(thread_count * sizeof(pthread_t));
    ThreadArgs* thread_args_arr = (ThreadArgs*)malloc(thread_count * sizeof(ThreadArgs));

    long long elements_per_thread = (long long)((double)it_count / (double)thread_count); 
    if(it_count % thread_count != 0){
        elements_per_thread += 1;
    }

    for(long long i = 0; i < thread_count; i++){
        thread_args_arr[i].first = i * elements_per_thread;
        thread_args_arr[i].last = (i + 1) * elements_per_thread - 1;
        if(thread_args_arr[i].last >= it_count){
            thread_args_arr[i].last = it_count - 1;
        }
        if (pthread_create(&thread_ids[i], NULL, calc_pi_part, &thread_args_arr[i]) != 0) {
            printf("Failed to create thread");
            return 1;
        }
    }

    double final_ans = 0.0;

    for(long long i = 0; i < thread_count; i++){
        void* thread_result;
        if (pthread_join(thread_ids[i], &thread_result) != 0) {
            printf("Failed to join thread");
            return 1;
        }
        
        final_ans += *(double*)thread_result;
        free(thread_result);
    }
    
    final_ans = 4.0 * final_ans;
    free(thread_ids);
    free(thread_args_arr);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%.15f\n", final_ans);
    printf("%.15f\n", time_spent);
    return 0;
}
