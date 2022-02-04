#include <stdlib.h> 	
#include <stdio.h>
#include <dlfcn.h>	/* dynamic loading shared libs */

void (*hello)(char *);	/* function pointer to library function */

int main() {
   void *handle;
   char *error;	
			
   handle = dlopen ("./libhello.so.1", RTLD_LAZY); /* resolve symbols lazily */
   if (handle==0) {  
      printf("%s\n", dlerror());
      exit(1);  	
   }

   hello = dlsym(handle, "hello");	/* resolve function */
   error = dlerror();
   if (error!=NULL) {
      printf("%s\n", dlerror());
      exit(1);  	
   }
  
   (*hello)("shared library loaded at runtime.");	/* call lib function */

   dlclose(handle); /* release shared lib */
}
