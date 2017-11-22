#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <sys/stat.h>
#include "bitio.h"
#include <iostream>
#include <algorithm>

using namespace std;

void PrintFile (BIT_FILE *outFile, unsigned long info_per_byte,unsigned long count)
{		
	if (count > 255 && count < 65535){
		// cout << 0x00  << " " << count << " " << info_per_byte << " ";
		OutputBits( outFile,0, 8);
		OutputBits( outFile,count,16);
		OutputBits( outFile,info_per_byte,8);
		// cout << "More than 255";
	}
	else{
	// cout << count << " " << info_per_byte << " ";		
	OutputBits( outFile,count,8);
	OutputBits( outFile,info_per_byte,8);	
	}
}


void RLE(BIT_FILE * inFile, BIT_FILE * outFile)
{
	unsigned long info_per_byte_prev = InputBits(inFile, 8);
	unsigned long info_per_byte_next;
	unsigned long count = 1;

	while(1)
	{
		info_per_byte_next = InputBits(inFile, 8);
		if (info_per_byte_next != info_per_byte_prev)
		{
			PrintFile(outFile, info_per_byte_prev, count);
			count = 0;
		}
		count++;
		info_per_byte_prev = info_per_byte_next;

		if (info_per_byte_next == 300)
		{
			break;
		}		
	}

	CloseInputBitFile(inFile);
	CloseOutputBitFile(outFile);
}

int main (int argc, char *argv[])
{
	BIT_FILE *inFile;
	inFile = OpenInputBitFile(argv[1]);
	BIT_FILE *outFile;
	outFile = OpenOutputBitFile(argv[2]);
	RLE(inFile, outFile);
	

	return 0;
}