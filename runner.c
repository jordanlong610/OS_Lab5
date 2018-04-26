/*
 * Lab 5 - Virtual Memory
 * Programmer: Jordan Long
 * Instructor: S. Lee
 * Course: SMPE 320
 * Section 1
 */


#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define TLB_SIZE 16
#define PAGES 256
#define PAGE_MASK 255
#define PAGE_SIZE 256
#define OFFSET_BITS 8
#define OFFSET_MASK 255
// Max number of characters per line of input file to read.
#define BUFFER_SIZE 10


struct lAddress{
	char lAddress[10];
	char pageNumber[8];
	char offset[8];
};


struct lAddress *lAddress;


/**
 * Reads text file and sets the logical address to the struct.
 */
void readFile(char* fileName)
{

	FILE* myfile = fopen(fileName,"r");

	/**
	 * Check if file is valid. Error and exit if not.
	 */
	if (myfile== NULL)
	{
		printf("Can't Open File: File Not Found \n");
		exit(-1);
	}

	/**
	 * Read in each line of txt file to an array.
	 */
	int i =0;
	int line = 1000;

	while(line--)
	{
		fscanf(myfile,"%s",lAddress[i].lAddress);
		printf("\n%s", lAddress[i].lAddress);	//Print to console
		i++;
	}

}

/**
 * Translates the logical address into a page number and offset.
 */
void translateAddress()
{

	for(int i=0; i<sizeof(lAddress) / sizeof(struct lAddress); i++)
	{


		lAddress[i].offset =
	}





}





/**
 * Main method for the virtual address program.
 */
int main(int argc, char **argv)
{
	struct lAddress LA[1000];
	lAddress = LA;
	char tlb[16];


	readFile("addresses.txt");


}

