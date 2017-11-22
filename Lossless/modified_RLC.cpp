#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <sys/stat.h>
#include "bitio.h"
#include <iostream>
#include <algorithm>

using namespace std;
void PrintFile (BIT_FILE *outFile, unsigned long info_per_byte,int count)
{		
	// cout << count << "\n";
	unsigned long temp = info_per_byte & 0x80;
	// cout <<  temp << ": & 0x80  \n";
	if (count == 1)
	{
		if (temp == 0)
		{
			// cout << "MSB = 0 :" << info_per_byte << "\n";
			OutputBits(outFile,info_per_byte,8);
		}
		else
		{
			// cout << "MSB = 1 :" << info_per_byte << "\n";
			OutputBits(outFile,0x81,8);
			OutputBits(outFile,info_per_byte,8);
		}
	}

	else if (count > 1 && count <= 127)
	{
		unsigned long count_print = 0x80 | count;
		OutputBits(outFile,count_print,8);
		OutputBits(outFile,info_per_byte,8);
	} 
	else if (count > 127 && count < 65535)
	{
		OutputBits(outFile,0x80,8);
		OutputBits(outFile,count,16);
		OutputBits(outFile,info_per_byte,8);
	}
}

void RLE(BIT_FILE * inFile, BIT_FILE * outFile)
{
	unsigned long info_per_byte_prev = InputBits(inFile, 8);
	unsigned long info_per_byte_next;
	int count = 1;
	while(1)
	{
		info_per_byte_next = InputBits(inFile, 8);
		if (info_per_byte_next != info_per_byte_prev)
		{			
			PrintFile(outFile, info_per_byte_prev,count);
			count = 0;
		}
		count++;
		info_per_byte_prev = info_per_byte_next;

		if (info_per_byte_next == 300)
		{
			break;
		}		
	}


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