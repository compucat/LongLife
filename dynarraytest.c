#include <stdio.h>
#include <stdlib.h>
//test of dynarrays

void function(int array[])
{
	int i;
	for(i=0; i<5; i++) array[i]=88;
}

main()
{
	int array[5];
	int i;
	for(i=0; i<5; i++) array[i]=i;
	for(i=0; i<5; i++) printf("%d, ", array[i]);
	function(array);
	for(i=0; i<5; i++) printf("%d, ", array[i]);
	
}
