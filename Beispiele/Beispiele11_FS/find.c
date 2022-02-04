#include <stdio.h> 
#include <dirent.h> 
#include <string.h>
  


int main(int argc, char **argv) {

    if (argc!=3) {
      printf("usage: find path filename\n");
      return 1;
    }

    struct dirent *de;  // Pointer for directory entry 
        
    DIR *dr = opendir(argv[1]); 
    if (dr == NULL) {
        printf("error: could not open directory '%s'\n", argv[1] ); 
        return 1; 
    } 
  
    while ((de = readdir(dr)) != NULL) {
       if (strcmp(de->d_name, argv[2])==0) {
           if (de->d_type==DT_DIR)
              printf("Found directory '%s' in '%s'\n", argv[2], argv[1]);
           else if (de->d_type==DT_REG)
              printf("Found file '%s' in '%s'\n", argv[2], argv[1]);
           else
              printf("Found other file-type for '%s' in '%s'\n", argv[2], argv[1]);       
           return 0;
       }
       printf("%s\n", de->d_name); 
    }

    closedir(dr);     

    return 0; 
} 
