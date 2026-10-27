#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if(argc != 2){
        printf("Invalid arguments\n");
        return 0;
    }
    clock_t begin = clock();
    long long it_count = strtoll(argv[1], NULL, 10);
    double divider = 1;
    double pi_div_4 = 0;
    double current_mult = 1;
    for(long long i = 0; i < it_count; i++){
        pi_div_4 += current_mult * (1.0 / divider);
        divider += 2;
        current_mult *= -1;
    }
    double pi = pi_div_4 * 4;
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%.15f\n", pi);
    printf("%.15f\n", time_spent);
    return 0;
}
