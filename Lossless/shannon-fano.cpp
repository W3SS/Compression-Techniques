#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <sys/stat.h>
#include "bitio.h"
#include <iostream>
#include <algorithm>
#include <math.h>
using namespace std;

int codeCount;
double sum_entropy = 0;
struct Tuple{
	vector <double> weights;
	vector <int> elements;
};
vector <pair <double,int> > get_sorted_histogram(BIT_FILE * inFile)
{
	unsigned long info_per_byte;
	vector <double> hist;
	
	vector < pair <double,int> > sorted_hist;
	hist.assign(256,0);
	double count = 0;
	while(1)
	{
		//printf("Checking....");
		info_per_byte = InputBits(inFile, 8);
		hist[info_per_byte]++;
		//printf("%lu \n",info_per_byte);
		count++;
		if (info_per_byte == 300)
		{
			break;
		}
	}
	// finding entropy
	
	for (int i = 0; i < 256; i++)
	{
		sum_entropy = sum_entropy + hist[i];
	}
	double entropy = 0.0;
	for (int i = 0; i < 256; i++)
	{
		if (hist[i] >=0.5){
			// cout << hist[i] << "\n";
			// cout << hist[i]/sum_entropy;
			// cout << log(sum_entropy/hist[i]);
			entropy = entropy + ((hist[i]/sum_entropy)* log(sum_entropy/hist[i]));
		}
		sorted_hist.push_back(make_pair (hist[i],i));
	}
	// cout << "Entropy:  " << entropy << "\n\n\n";
	entropy = entropy/log(2);

	cout << "Entropy:  " << entropy << "\n\n\n";
	sort (sorted_hist.begin(), sorted_hist.end());

	
	return sorted_hist;
}

Tuple* get_sorted_weights(char *argv[])
{
	BIT_FILE *inFile;
	inFile = OpenInputBitFile(argv[1]);
	vector < pair <double,int> > sorted_hist;
	Tuple* tuple = new Tuple;

	sorted_hist = get_sorted_histogram(inFile);
	CloseInputBitFile(inFile);
	// vector <double> weights;
	// vector <int> elements;
	
	for (int i = 255; i >= 0; i--)
	{
		// cout << sorted_hist[i].first << " " << sorted_hist[i].second << "\n";
		if (sorted_hist[i].first == 0)
			{break;}
		//cout << sorted_hist[i].first;
		tuple -> weights.push_back(sorted_hist[i].first);
		tuple -> elements.push_back(sorted_hist[i].second);
		
	}
	return tuple;
}

struct Node {
	//int data;
	// vector <double> weights;
	Node* left;
	Node* right;
	double partition;

};
// void Search(Node*, int);
// Node* MakeNewNode(int);

Node* MakeNewNode(Node* root, double partition)
{
	root = new Node;
	root-> left = root-> right = NULL;
	root -> partition = partition;
	return root;
}
Node* MakeTree(Node* root, vector <double> weights){
	// cout << "\n" << weights.size() << " The number of weights";
	// cout << "\n" << weights[0] << " first element in vector \n";
	if (weights.size() != 1)
	{
		// cout << "Not a leaf  ";
		double partition;
		double sum = 0;
		for (vector<double>::iterator it = weights.begin(); it != weights.end();++it)
			sum = sum + *it;

		double sum_temp1 = weights[0];
		// cout << sum_temp1;
		int i;
		for (i = 0; i < weights.size()-1; i++)
		{
			double sum_temp2 = sum_temp1 + weights[i+1];
			// cout << int(sum_temp1 - sum/2) << "   " << int(sum_temp2 - sum/2) << "\n";
			if (abs(int(sum_temp1 - sum/2)) <  abs(int(sum_temp2 - sum/2)) )
			{
				// cout << weights[i] <<  "   Partition\n";
				partition = weights[i];
				// root -> partition = weights[i];
				break;
			}
			sum_temp1 = sum_temp2;
		}

		vector <double> :: const_iterator first1 = weights.begin();
		vector <double> :: const_iterator first = weights.begin() + i+1;
		vector <double> :: const_iterator last = weights.begin() + weights.size();
		vector <double> vector_left (first1, first);
		vector <double> vector_right (first, last); 

		root =  MakeNewNode(root, partition);
		root -> left = MakeTree(root -> left, vector_left);
		root -> right = MakeTree(root -> right, vector_right);

	}

	else
	{
		// cout << "Got a leaf \n";
		root =  MakeNewNode(root, weights[0]);
	}
	
	

	return root;
}

unsigned long getCode(Node* root, double weight, unsigned long code)
{
	// cout << "getCode initialised \n";
	if (root -> left != NULL && root -> right != NULL)
	{
		codeCount++;
		// cout << "Entered If statement..\n";
		if(weight >= root -> partition && root -> left -> partition > 0 )
		{			
			// cout << root -> left -> partition << "\n";
			// cout << "0";
			code = code | 0x00;
			code = code << 1;
			return getCode(root -> left, weight, code);
		}
		else if (weight <= root -> partition)
		{
			// cout << root -> left -> partition << "\n";
			// cout << "1";
			code = code | 0x01;
			code = code << 1;
			return getCode(root -> right, weight, code);
		}
	}
	else
	{
		code = code >> 1;
		// cout << "reached leaf \n";
		root -> partition = -1;
		return code;
	}
	// cout << "getCode executed \n";
}

vector<unsigned long> createTable(Node* root, vector <double> weights, vector <int> &counter)
{
	vector <unsigned long> codewords;
	unsigned long code = 0;
	for (int i = 0; i < 256; i++)
	{
		codeCount = 0;
		// cout << "createTable \n";
		if(int(weights[i]) == 0)
			break;
		// cout << "Symbol: " << i << "Codeword: "; 
		// string str = "";		
		codewords.push_back(getCode(root, weights[i],code));		
		counter.push_back(codeCount);		
		// cout << " Weight: " << weights[i] << " Length: " << codeCount<<"\n";
		// cout << "Count = " << counter[i] << "\n";
	}
	return codewords;
}
// int getCount(unsigned long code)
// {
// 	int cnt = 1;

// 	if (code >= 1)
// 		cnt = int(floor((log(code)/log(2))+1));
// 	// cout << cnt << " ";
// 	return cnt;
// }
void PrintFile (vector <unsigned long>* Codeword,vector <int>* length,char *argv[])
{
	BIT_FILE *inFile; BIT_FILE *outFile;
	inFile = OpenInputBitFile(argv[1]);
	outFile = OpenOutputBitFile(argv[2]);
	unsigned long input_byte;
	while(1)
	{
		input_byte = InputBits(inFile, 8);		
		// cout << (*length)[int(input_byte)] << "\n";
		OutputBits( outFile, (*Codeword)[int(input_byte)], (*length)[int(input_byte)]);
		if (input_byte == 300)
		{
			break;
		}
	}
	CloseInputBitFile(inFile);
	CloseOutputBitFile(outFile);
}
void getBinary(unsigned long code, int count)
{
	unsigned long codeTemp;
	for (int i = 0; i < count; i++)
	{
		codeTemp = code & 0x01;
		cout << codeTemp;
		code = code >> 1;
	}
}
int main(int argc, char *argv[])
{
	Node* root = NULL;
	Tuple* weightsElements = NULL;
	vector<unsigned long> codewords;
	vector<unsigned long> codewords_sorted;
	vector<int> length_sorted;
	vector <pair <int,pair <unsigned long, int> > > elementCodeword;
	vector <pair <unsigned long, int> > CodewordLength;
	weightsElements = get_sorted_weights(argv);
	vector <int> counter;
	
	// This command will create the tree
	root = MakeTree(root,weightsElements -> weights);

	// cout << "Tree is made........ \n";
	// The table is created for each symbol to have a codeword. Also length of codeword is got for each
	codewords = createTable(root,weightsElements -> weights, counter);


	int same_count = 0;
	double avg_code_len = 0.0;
	for (int i = 0; i < codewords.size(); i++)
	{
		if (i >= 1 && codewords[i] == codewords[i-1-same_count])
		{
			same_count++;
			codewords[i] = codewords[i-1] + 0x01;
		}
		else
		{same_count = 0;}
		CodewordLength.push_back(make_pair (codewords[i], counter[i]));
		elementCodeword.push_back(make_pair (weightsElements -> elements[i],CodewordLength[i]));
		cout << "Element: " << weightsElements -> elements[i] << " Codeword: "; getBinary(codewords[i],counter[i]); cout << "  Weight: " << weightsElements -> weights[i] << " Length: " << counter[i]<<"\n"; 
		avg_code_len = avg_code_len + (weightsElements -> weights[i]/sum_entropy)* counter[i];
	}
	cout << "\n The average code length using Shannon-Fano is " << avg_code_len << "\n";
	sort(elementCodeword.begin(), elementCodeword.end());
	int count = 0;
	// cout << "\n\n\n\n";
	for (int i = 0; i < 256; i++)
	{
		if (elementCodeword[count].first == i)
		{			
			codewords_sorted.push_back(elementCodeword[count].second.first);
			length_sorted.push_back(elementCodeword[count].second.second);
			count++;
		}
		else
		{
			codewords_sorted.push_back(0);
			length_sorted.push_back(0);
		}
		// cout << "Element: " << i << " Codeword: " << codewords_sorted[i] << " Length: " << length_sorted[i]<<"\n"; 

	}
	PrintFile(&codewords_sorted, &length_sorted, argv);
	// cout << "\n\n\n\n";
	// for (int i = 0; i < 126; i++)
	// {
	// 	cout << codewords_sorted[i] << "\n"; 
	// }



	return 0;
}


