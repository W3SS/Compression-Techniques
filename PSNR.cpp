#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
using namespace std;


#define Size 256


int main(int argc, char *argv[])

{
	// file pointer
	FILE *file;
	int BytesPerPixel = atoi(argv[3]);
	

	// image data array
	unsigned char Imagedata_quantised[Size][Size][BytesPerPixel];
	unsigned char Imagedata[Size][Size][BytesPerPixel];

	// read image "ride.raw" into image data matrix
	if (!(file=fopen(argv[1],"rb")))
	{
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	if (!(file=fopen(argv[2],"rb")))
	{
		cout << "Cannot open file: " << argv[2] <<endl;
		exit(1);
	}
	fread(Imagedata_quantised, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);
	double MSE = 0;
	for(int k = 0; k < BytesPerPixel; k++)
	{
		for(int i = 0; i < Size; i++)
		{
			for (int j = 0; j < Size; j++)
			{
				MSE = MSE + (Imagedata[i][j][k] - Imagedata_quantised[i][j][k]) * (Imagedata[i][j][k] - Imagedata_quantised[i][j][k]);
			}
		}
	}
	// cout << MSE;
	MSE = (1.0 * MSE)/(Size*Size*BytesPerPixel);
	double PSNR = 10*log((255.0*255.0)/MSE);

	cout << "PSNR: " << PSNR;




	
	return 0;
}









	