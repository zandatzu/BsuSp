#include <stdio.h>

int main(){    
 	int var    = 789;
	int *ptr   = &var;	// Zeiger auf var
	int **ptr2 = &ptr;	// Zeiger auf ptr

 	printf("Wert von var = %d\n", var);
   	printf("Wert von var via single pointer = %d\n", *ptr );    
 	printf("Wert von var via double pointer = %d\n", **ptr2 );     
 	return 0;
} 
