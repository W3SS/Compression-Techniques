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

vector < vector <double> > dataset_creator(char *argv[], int);
vector <vector <double> >  Centroid_formation(vector < vector <double> >*, vector < vector <int> >, int);
vector < vector <int> > clusterize(vector < vector <double> > *, vector <vector <double> > );
vector <vector <double> > Vector_quantization (vector < vector <double> > *, vector <vector <double> >, vector < vector <int> > );


vector < vector <double> > dataset_creator(char *argv[], int n)
{

	FILE *file;	
	vector < vector <double> > dataset;
	int count = 0;
	vector <double> temp;
	for (int i = 1; i < 4; i++)
	{
		unsigned char Imagedata[256][256];
		// cout << argv[i];
		if (!(file=fopen(argv[i],"rb")))
		{
			cout << "Cannot open file: " << argv[i] <<endl;
			exit(1);
		}
		fread(Imagedata, sizeof(unsigned char), 256*256, file);
		fclose(file);
		for(int j = 0; j < 256; j = j + n)
		{
			for(int k = 0; k < 256; k = k + n)
			{
				for(int x = 0; x < n; x++)
				{
					for(int y = 0; y < n; y++)
					{
						// cout<<x << " ";
						// cout<<y << " ";
						// cout<<int(Imagedata[x+j][y+k]);
						temp.push_back(double(Imagedata[x+j][y+k]));
					}
				}
				dataset.push_back(temp);
				vector <double>().swap(temp);
			}
		}

	}
	
	return dataset;

}


vector <vector <double> >  Centroid_formation(vector < vector <double> > *dataset, vector < vector <int> > clusters, int n)
{	
	cout << "Entered Centroid_formation \n";
	vector <vector <double> > centroid(clusters.size());
	for(int i = 0; i < clusters.size(); i++)
	{
		cout << "Centroid: " << i << " ";
		for(int k = 0; k < n; k++)
		{
			int avg = 0;
			// cout <<n << " n";
			for(int j = 0; j < clusters[i].size(); j++)
			{
				avg = avg + (*dataset)[clusters[i][j]][k];
			}
			cout << (avg*1.0)/clusters[i].size() << " ";
			centroid[i].push_back((avg*1.0)/clusters[i].size());
		}
		cout <<"\n";
		// cout << "Probabity_in_interval" << prob << "\n";
	}
	return centroid;
}

vector < vector <int> > clusterize(vector < vector <double> >*dataset, vector <vector <double> > centroid)
{
	cout << "Entered clusterize\n";
	// cout << centroid[0].size() <<"\n" ;
	int dist;
	vector <int> dist_sort(centroid.size());
	vector < vector <int> > cluster(centroid.size());
	for(int i = 0; i < dataset -> size(); i++)
	{
		for(int j = 0; j < centroid.size(); j++)
		{
			dist = 0;
			for (int k = 0; k < centroid[0].size(); k++)
			{				
				dist = dist + (int(centroid[j][k]) - int((*dataset)[i][k])) * (int(centroid[j][k]) - int((*dataset)[i][k]));
				// cout << centroid[j][k] << "\n";
			}
			// cout << dist << " Distance " << j << " \n";
			dist_sort[j] = dist;
		}
		vector<int>::iterator result = min_element(dist_sort.begin(), dist_sort.end());
		// cout << "Index: " << result - dist_sort.begin() << "\n";
		cluster[result - dist_sort.begin()].push_back(i);
		// cout << "completed\n";
	}
	return cluster;
}

int iteration = 0;
vector <vector <double> > Vector_quantization (vector < vector <double> > *dataset, vector <vector <double> > centroid, vector < vector <int> > clusters)
{
	iteration++;
	vector < vector <int> > newClusters;
	cout << "Iteration: " << iteration << "\n";
	// for(int i = 0; i < r.size(); i++)
	// {
	// 	cout << "Centroid: " << r[i] << " ";
	// 	cout << "Borders: " << t[i] << "  " << t[i+1] << "\n";
	// }
	cout << "\n";
	vector <vector <double> > newCentroids = Centroid_formation(&(*dataset), clusters, int(centroid[0].size()));
	// for(int i = 0; i < r.size(); i++)
	// {
	// 	cout << "Centroid: " << r[i] << " ";
	// }
	int dist = 0;
	for(int j = 0; j < centroid.size(); j++)
	{		
		for (int k = 0; k < centroid[0].size(); k++)
		{
			dist = dist + (centroid[j][k] - newCentroids[j][k]) * (centroid[j][k] - newCentroids[j][k]);
		}
	}
	if (dist == 0)
	{		
		return newCentroids;
	}	
	newClusters = clusterize(&(*dataset),newCentroids);
	Vector_quantization(&(*dataset), newCentroids, newClusters);

}
int main (int argc, char *argv[])
{
	int n = atoi(argv[4]);
	int block_dim = atoi(argv[5]);
	vector < vector <double> > dataset = dataset_creator(argv, block_dim);
	cout << dataset.size() <<"\n";
	vector <vector <double> > centroid;
	vector < vector <int> > clusters;
	int temp;
	for(int i = 0; i < n; i++){
		// cout << rand() % dataset.size() + 1 << " " << i << "\n";
		temp = rand() % dataset.size() + 1;
		// cout << dataset[temp][0] << "\n";
		centroid.push_back(dataset[temp]);
		cout << "Centroid: " << i << " "; 
		for (int j = 0; j<block_dim*block_dim; j++)
		{
			cout << centroid[i][j] << " ";
		}
		cout << "\n";
	}

	clusters = clusterize(&dataset,centroid);
	centroid = Vector_quantization(&dataset, centroid, clusters);


	string filenames[] = {"elaine.256", "f16.256", "house.256", "moon.256", "chem.256", "couple.256"}; 
	unsigned char Imagedata[256][256];
	FILE *file;	
	BIT_FILE *outFile_file;
	vector <double> temp_c;
	vector <int> dist_sort(centroid.size());
	for (int i = 0; i < 6; i++)
	{
		string name = "Output_vector/op_" + filenames[i] + ".raw";
		string name_file = "Output_vector/op_" + filenames[i] + ".dat";
		outFile_file = OpenOutputBitFile((char*)(name_file.c_str()));
		// cout << argv[i];


		if (!(file=fopen(filenames[i].c_str(),"rb")))
		{
			cout << "Cannot open file: " << argv[i] <<endl;
			exit(1);
		}
		fread(Imagedata, sizeof(unsigned char), 256*256, file);
		fclose(file);

		for(int j = 0; j < 256; j = j + block_dim)
		{
			for(int k = 0; k < 256; k = k + block_dim)
			{
				for(int x = 0; x < block_dim; x++)
				{
					for(int y = 0; y < block_dim; y++)
					{
						// cout<<x << " ";
						// cout<<y << " ";
						// cout<<int(Imagedata[x+j][y+k]);
						temp_c.push_back(double(Imagedata[x+j][y+k]));
					}
				}
				
				for(int j1 = 0; j1 < centroid.size(); j1++)
				{
					int dist = 0;
					for (int k1 = 0; k1 < centroid[0].size(); k1++)
					{				
						dist = dist + (int(centroid[j1][k1]) - int(temp_c[k1])) * (int(centroid[j1][k1]) - int(temp_c[k1]));
						// cout << centroid[j][k] << "\n";
					}
					// cout << dist << " Distance " << j << " \n";
					dist_sort[j1] = dist;
				}
				vector<int>::iterator result = min_element(dist_sort.begin(), dist_sort.end());
				vector <double>().swap(temp_c);
				int cntr = 0;
				for(int x = 0; x < block_dim; x++)
				{
					for(int y = 0; y < block_dim; y++)
					{
						Imagedata[x+j][y+k] = centroid[result - dist_sort.begin()][cntr++];
						OutputBits( outFile_file, result - dist_sort.begin(), log(n)/log(2));
					}
				}

			}
		}

		if (!(file=fopen(name.c_str(),"wb")))
		{
			cout << "Cannot open file: " << name << endl;
			exit(1);
		}
		CloseOutputBitFile(outFile_file);

		fwrite(Imagedata, sizeof(unsigned char), 256*256, file);
		fclose(file);
	}
	return 0;	
}