#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//LongLife: a machine generation algorithm to generate the Life pattern that will stay active the longest.
//Warning: this program is a memory hog. Compile for 64 bit.

#define targetSize 20
#define setSize chunkSize*2
#define sets(num, x, y) (setMemory[chunkSize*chunkSize*4*num+chunkSize*2*x+y]) //Modified from http://stackoverflow.com/questions/2438142/dynamic-memory-allocation-for-3d-array
																		  //for 3d dynamic arrays
																		  
#define chunks(num, x, y) (chunkMemory[chunkSize*chunkSize*4*num+chunkSize*2*x+y])

#include "life.h"

void generateSets(int setMemory[], int chunkMemory[], int chunkSize);
void sortSetLives(int setLives[160000][2]);
																		  
main()
{
	printf("LongLife by compucat (c) August 2015\n");
	int i, j, k;
	int chunkSize=5;
	
	//A bit wasteful, but I'm being lazy and using an int to store a bit.
	//These are 1-D dynamic arrays, accessed as 3-D arrays using the macros sets(num, x, y) and chunks(num, x, y).
	int *setMemory, *chunkMemory;
	long int setLives[160000][2]; //This holds the lifetime of each set and is used to sort the sets.
								 //Bank 0 (setLives[][0]) holds the lifetime, bank 1 holds the numbers 0=159999-this is used to eliminate
								 //the chore of moving large sets around in memory so much by making a pseudo-pointer.
	
	
	chunkMemory=malloc(sizeof(int)*20*chunkSize*chunkSize);//Allocate memory for 20 5x5 chunks
	if(chunkMemory==NULL) //Catch the exception
	{
		printf("Fatal error: not enough memory for new chunks. LongLife aborted.\nPress Enter to exit.");
		getchar();
		exit(1);
	}
	setMemory=malloc(sizeof(int)*160000*setSize*setSize);//Allocate memory for 160,000 10x10 sets
	if(setMemory==NULL) //Catch the exception
	{
		printf("Fatal error: not enough memory for new sets. LongLife aborted.\nPress Enter to exit.");
		getchar();
		exit(1);
	}
	
	srand(time(NULL)); //seed pseudoRNG
	for(i=0; i<20; i++) for(j=0; j<chunkSize; j++) for(k=0; k<chunkSize; k++) chunks(i, j, k)=(rand>(RAND_MAX/2)); //Generate 20 random chunks
	
	while(chunkSize<targetSize)
	{
		
		generateSets(setMemory, chunkMemory, chunkSize); //Generate 160,000 possible sets
		for(i=0; i<160000; i++) setLives[i][1]=i; //Set default 1, 2, 3...160000 index order
		for(i=0; i<160000; i++) setLives[i][0]=simStableLife(setSize, setMemory, i, chunkSize); //Rate the sets
		
		sortSetLives(setLives); //Sort the pseudo-pointers by set life length
		
		free(chunkMemory);//Free chunk memory
		chunkSize=chunkSize*2;
		chunkMemory=malloc(sizeof(int)*20*chunkSize*chunkSize);//Allocate memory for set-size chunks
		if(chunkMemory==NULL) //Catch the exception
		{
			printf("Fatal error: not enough memory for new chunks. LongLife aborted.\nPress Enter to exit.");
			getchar();
			exit(1);
		}
		
		
		for(i=0; i<10; i++) for(j=0; j<chunkSize; j++) for(k=0; k<chunkSize; k++) chunks(i, j, k)=sets(setLives[i][1], j, k); //Copy 10 best sets to chunks
		//Note here that setLives[i][1] holds the index of the ith best set-this is done in the bubble sort.
		
		for(i=10; i<20; i++) for(j=0; j<chunkSize; j++) for(k=0; k<chunkSize; k++) chunks(i, j, k)=(rand>(RAND_MAX/2)); //Generate 10 random chunks
		
		free(setMemory); //Free set memory
		setMemory=malloc(sizeof(int)*160000*setSize*setSize); //Allocate memory for new size sets
		if(setMemory==NULL) //Catch the exception
		{
			printf("Fatal error: not enough memory for new sets. LongLife aborted.\nPress Enter to exit.");
			getchar();
			exit(1);
		}
		//If chunk size is greater than or equal to target size, then exit mainloop (handled in while definition)
	}
	
	//TODO: If time, add checker to ensure randoms aren't better than algorithmically generated patterns
	
	FILE* out=fopen("LongLifeOutput.txt", "a");
	printf("Done! Writing file output...");
	fprintf(out, "***LongLife by compucat (c) August 2015 output***\n\nAttempted to find %dx%d patterns\n\nDue to algorithm limitations, found %dx%d patterns\n\n", targetSize, targetSize, chunkSize, chunkSize);
	
	for(i=0; i<10; i++) 
	{
		fprintf(out, "***Pattern %d***\n\n", i+1);
		for(j=0; j<chunkSize; j++) 
		{
			for(k=0; k<chunkSize; k++)
			{
				if(chunks(i, j, k)==1) fprintf(out, "*");
				else fprintf(out, " ");
			}
			fprintf(out, "\n");
		}
		fprintf(out, "\n");
	}
	fprintf(out, "***End LongLife output***\n\n");
	fclose(out);
	printf("Done! Press Enter to exit...");
	getchar();	
}

void generateSets(int setMemory[], int chunkMemory[], int chunkSize)
{
	int set=0;
	int i, j, k, l; //Index of UL, UR, LL, LR chunks, respectively
	int a, b; //loop indices
	for(i=0; i<20; i++) for(j=0; j<20; j++) for(k=0; k<20; k++) for(l=0; l<20; l++)
	{
		for(a=0; a<chunkSize; a++) for(b=0; b<chunkSize; b++)
		{
			sets(set, a, b)=chunks(i, a, b);
			sets(set, a+20, b)=chunks(j, a, b);
			sets(set, a, b+20)=chunks(k, a, b);
			sets(set, a+20, b+20)=chunks(l, a, b);
		}
		set++;
	}
	
}

void sortSetLives(int setLives[160000][2]) //I'm being lazy and using a bubble sort here. I might eventually implement a quicksort.
{
	int i, temp1, temp2;
	int sort=1;
	while(sort==1)
	{
		sort=0;
		for(i=0; i<159999; i++)
		{
			if(setLives[i][0]<setLives[i+1][0])
			{
				temp1=setLives[i][0];
				temp2=setLives[i][1];
				setLives[i][0]=setLives[i+1][0];
				setLives[i][1]=setLives[i+1][1];
				setLives[i+1][0]=temp1;
				setLives[i+1][1]=temp2;
				sort=1;
			}
		}
	}
}
