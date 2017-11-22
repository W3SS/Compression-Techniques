#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sys/stat.h>
#include "bitio.h"
#include <iostream>
#include <algorithm>
#include <math.h>
using namespace std;


void Lloyd (vector <double> &, double, vector <double> &, vector <double> &);
vector <double> histogram(char *argv[]);
double MSE_calculater(vector <double> *, vector <double> *, vector <double> *);
void r_updater(vector <double> &, vector <double> &, vector <double> &);
void t_updater(vector <double> &, vector <double> &);

vector <double> histogram(char *argv[])
{

	BIT_FILE * inFile;
	
	vector <double> hist(256);
	unsigned long info_per_byte;
	int count = 0;
	for (int i = 0; i < 3; i++)
	{
		inFile = OpenInputBitFile(argv[i]);
		while(1)
		{
			//printf("Checking....");
			info_per_byte = InputBits(inFile, 8);
			hist[info_per_byte]++;
			//printf("%lu \n",info_per_byte);
			count++;
			if (info_per_byte == 300)
			{
				CloseInputBitFile(inFile);
				break;
			}
			
		}
	}
	for (int i = 0; i < 256; i++)
	{
		hist[i] = hist[i]/count;
	}	
	// cout << count;
	return hist;

}

double MSE_calculater(vector <double> *r, vector <double> *hist, vector <double> *t)
{
	double MSE = 0;
	for(int i = 0; i < (*r).size(); i++)
	{
		for(int j = (*t)[i]; j < (*t)[i+1]; j++)
		{
			MSE = MSE + (*hist)[j] * ((*r)[i] - j) * ((*r)[i] - j);
		}
	}
	return MSE;
}

void r_updater(vector <double> &r, vector <double> &hist, vector <double> &t)
{	
	
	for(int i = 0; i < r.size(); i++)
	{
		r[i] = 0;
		double prob = 0;
		for(int j = t[i]; j <= t[i+1]; j++)
		{
			prob = prob + hist[j];
		}
		cout << "Probabity_in_interval" << prob << "\n";

		for(int j = t[i]; j <= t[i+1]; j++)
		{
			r[i] = r[i] + (hist[j] * j)/prob;
		}
	}
}

void t_updater(vector <double> &r, vector <double> &t)
{
	for(int i = 1; i < r.size(); i++)
	{
		t[i] = (r[i-1] + r[i])/2.0;
	}
}

int iteration = 0;
void Lloyd (vector <double> &hist, double MSE, vector <double> &r, vector <double> &t)
{
	iteration++;
	cout << "Iteration: " << iteration << "\n";
	for(int i = 0; i < r.size(); i++)
	{
		cout << "Centroid: " << r[i] << " ";
		cout << "Borders: " << t[i] << "  " << t[i+1] << "\n";
	}
	cout << "\n";
	r_updater(r, hist, t);
	for(int i = 0; i < r.size(); i++)
	{
		cout << "Centroid: " << r[i] << " ";
	}
	double new_MSE = MSE_calculater(&r, &hist, &t);
	if (new_MSE - MSE < 0.0001 && new_MSE - MSE > -0.0001)
	{		
		return;
	}	
	t_updater(r, t);
	Lloyd(hist,new_MSE, r, t);

}
int main (int argc, char *argv[])
{

	vector <double> hist = histogram(argv);
	int n = atoi(argv[4]);
	vector <double> r(n);
	vector <double> t(n+1);
	t[0] = 0;
	t[n] = 255;
	double MSE;
	for(int i = 0; i < n; i++)
	{
		r[i] = (256/(n+1))* (i+1);
	}

	t_updater(r, t);

	MSE = MSE_calculater(&r, &hist, &t);
	Lloyd(hist,MSE,r, t);

	cout << "Final ans: \n";
	for(int i = 0; i < r.size(); i++)
	{
		cout << "Centroid: " << r[i] << " ";
		cout << "Borders: " << t[i] << "  " << t[i+1] << "\n";
	}

	BIT_FILE * inFile;
	BIT_FILE *outFile_image;
	BIT_FILE *outFile_file;
	unsigned long info_per_byte;
	string filenames[] = {"elaine.256", "f16.256", "house.256", "moon.256", "chem.256", "couple.256"}; 
	for (int i = 0; i < 6; i++)
	{
		inFile = OpenInputBitFile((char*)(filenames[i].c_str()));
		string name_image = "Output_scalar/op_" + filenames[i] + ".raw";
		string name_file = "Output_scalar/op_" + filenames[i] + ".dat";
		outFile_image = OpenOutputBitFile((char*)(name_image.c_str()));
		outFile_file = OpenOutputBitFile((char*)(name_file.c_str()));
		while(1)
		{
			//printf("Checking....");
			info_per_byte = InputBits(inFile, 8);
			int ind;
			for (ind = 0; ind < n; ind++)
			{
				if(info_per_byte >= t[ind] && info_per_byte < t[ind + 1])
					break;
			}
			OutputBits( outFile_image, r[ind], 8);
			OutputBits( outFile_file, ind, log(n)/log(2));


			if (info_per_byte == 300)
			{
				CloseInputBitFile(inFile);
				break;
			}
			
		}
		CloseOutputBitFile(outFile_image);
		CloseOutputBitFile(outFile_file);
	}

	// Testing_compression();

	
}