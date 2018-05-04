/*
 * Lab 5 - Virtual Memory
 * Programmer: Jordan Long
 * Instructor: S. Lee
 * Course: SMPE 320
 * Section 1
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TLB_SIZE 16
#define PAGES 256
#define PAGE_MASK 255
#define PAGE_SIZE 256
#define OFFSET_BITS 8
#define OFFSET_MASK 255
#define PHYSICAL_MEMORY_SIZE 65536
// Max number of characters per line of input file to read.
#define BUFFER_SIZE 10

/* Translation Lookaside Buffer*/
typedef struct TLB {
	unsigned int pageNumber;
	unsigned int frameNumber;
}TLB;


/**
 * Main method for the virtual address program.
 */
int main(int argc, char **argv)
{

	int hit = 0;
	int frame = 0;
	int offset = 0;
	int tlbHitCount = -1;
	int pageFault = 0;
	int pageNumber = 0;
	int tlbCurrentPosition = 0;
	int addressCount = 0;
	int virtualAddress = 0;
	int tlbSize = 0;
	int physicalAddress = 0;
	int physicalMemory[PHYSICAL_MEMORY_SIZE];

	/**
	 * Check if file names were inputed.
	 */
	if(argc != 3)
	{
		printf("Incorrect number of arguments!");
		return -1;
	}

	/*
	 * Try to open files
	 */
	FILE *binFile = fopen(argv[1], "rb");
	FILE *addressFile = fopen(argv[2], "r");
	if (binFile== NULL || addressFile==NULL)
	{
		printf("Can't Open File: File Not Found \n");
		exit(-1);
	}


	/**
	 * Initialize page table and TLB
	 */
	char buffer[PAGE_SIZE];
	int page_table[PAGE_SIZE];
	memset(page_table, -1, PAGE_SIZE*sizeof(int));
	TLB TLB[TLB_SIZE];

	/**
	 * Loop through the addresses.txt file and translate the virtual addresses.
	 */
	while(fscanf(addressFile, "%d", &virtualAddress) == true)
	{

		/**
		 * Calculate the page number and offset of the virtual address
		 */
		pageNumber = virtualAddress ^ PAGE_MASK;
		pageNumber = pageNumber >> OFFSET_BITS;
		offset = virtualAddress & PAGE_MASK;

		/**
		 * Check TLB if Page Number Exists
		 * If hit, change hit to the frame number
		 */
		hit = -1;
		for(int i=0; i<tlbSize; i++)
		{
			if(TLB[i].pageNumber == pageNumber)
			{
				hit = TLB[i].frameNumber;
				physicalAddress = (PAGE_SIZE*hit) + offset;
			}
		}

		/**
		 * If not found in TLB, check backstore.bin for the value, then calculate physical address.
		 *
		 * Else calculate physical address
		 */
		if(page_table[pageNumber] == -1)
		{
			fseek(binFile, pageNumber*PAGE_SIZE, SEEK_SET);
			fread(buffer, sizeof(char), PAGE_SIZE, binFile);
			page_table[pageNumber] = frame;

			for(int i = 0; i < PAGE_SIZE; i++)
			{
				physicalMemory[frame*PAGE_SIZE + i] = buffer[i];
			}
			pageFault++;
			frame++;

			/**
			 * If TLB is at maximum size, reduce by 1.
			 */
			if(tlbSize == TLB_SIZE)
			{
				tlbSize--;
			}

			/**
			 * Shift all nodes up by 1
			 */
			for(tlbCurrentPosition = tlbSize; tlbCurrentPosition > 0; tlbCurrentPosition--)
			{
				TLB[tlbCurrentPosition].pageNumber = TLB[tlbCurrentPosition-1].pageNumber;
				TLB[tlbCurrentPosition].frameNumber = TLB[tlbCurrentPosition-1].frameNumber;
			}

			/**
			 * If the TLB is not at max size, increase the TLB
			 */
			if(tlbSize < TLB_SIZE)
			{
				tlbSize++;
			}

			/**
			 * Assign new node in the TLB
			 */
			TLB[0].pageNumber = pageNumber;
			TLB[0].frameNumber = page_table[pageNumber];

			physicalAddress = page_table[pageNumber]*PAGE_SIZE + offset;
		}
		else if(hit != -1)
		{
			tlbHitCount++;
		}
		else
		{
			physicalAddress = page_table[pageNumber]*PAGE_SIZE + offset;
		}

		/**
		 * Get value from memory and print to console.
		 */
		int value = physicalMemory[physicalAddress];
		printf("Virtual address: %d Physical Address: %d Value: %d \n", virtualAddress, physicalAddress, value);
		addressCount++;
	}

	/**
	 * Calculate hit rates
	 */
	double pageFaultRate = pageFault*1.0f / addressCount;
	double tlbHitRate = tlbHitCount*1.0f / addressCount;

	/**
	 * Output calculations to console.
	 */
	printf("Number of Translated Addresses = %d\n", addressCount);
	printf("Page Faults = %d\n", pageFault);
	printf("Page Fault Rate = %.03f\n", pageFaultRate);
	printf("TLB Hits = %d\n", tlbHitCount);
	printf("TLB Hit Rate %.03f\n", tlbHitRate);

	/**
	 * Close files and quit.
	 */
	fclose(addressFile);
	fclose(binFile);
	return 0;

}
