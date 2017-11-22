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
	// cout << "Element: " <<info_per_byte << " count: " << count << "\n";
	for(int i = 0; i < int(count); i++){
		OutputBits( outFile,info_per_byte,8);
	}
	
}


void RLE_decoder(BIT_FILE * inFile, BIT_FILE * outFile)
{
	unsigned long info_per_byte;
	
	while(1)
	{
		int info_per_byte_count = 1;
		info_per_byte = InputBits(inFile, 8);
		unsigned long temp1 = info_per_byte & 0x80;
		if (temp1 != 0)
		{
			// cout << "loop  \n";
			unsigned long count_temp = info_per_byte & 0x7F;
			if (count_temp != 0)
			{
				// cout << "7 bit count \n";
				info_per_byte_count = count_temp;
				info_per_byte = InputBits(inFile, 8);
			}
			else
			{
				// cout << "16 bit count \n";
				info_per_byte_count = InputBits(inFile, 16);
				info_per_byte = InputBits(inFile, 8);
			}
		}
		
		if (info_per_byte == 300)
		{
			break;
		}	
		// cout << "Before Print \n";
		PrintFile(outFile, info_per_byte, info_per_byte_count);			
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
	RLE_decoder(inFile, outFile);
	

	return 0;
}