#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE* meminfo = fopen("/proc/meminfo","r");
    char* mem_total_c = malloc(sizeof(char)*200);
    char* mem_free_c = malloc(sizeof(char)*200);
    char* cached_c = malloc(sizeof(char)*200);
    int mem_total;
    int mem_free;
    int cached;

    fscanf(meminfo,"%s %d",mem_total_c,&mem_total);
    fscanf(meminfo,"%*s %s %d",mem_free_c,&mem_free);
    fscanf(meminfo,"%*s %*s %*d %*s %*s %*d %*s %s %d",cached_c,&cached);
    printf("%s %d kb\n ",mem_total_c,mem_total);
    printf("%s %d kb\n ",mem_free_c,mem_free);
    printf("%s %d kb\n ",cached_c,cached);
    fclose(meminfo);
}