#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sys/stat.h>
#include "bitio.h"
#include <iostream>
#include <algorithm>
#include <math.h>
#include <queue>
using namespace std;

int codeCount;
double sum_entropy = 0;
struct Node {
	//int data;
	// vector <double> weights;
	Node* left;
	Node* right;
	int weight;
	int element;


};
// void Search(Node*, int);
// Node* MakeNewNode(int);

Node* MakeNewNode(int weight, int element)
{
	Node *root = new Node;
	root-> left = root-> right = NULL;
	root -> weight = weight;
	root -> element = element;
	return root;
}


vector <pair <double,int> > get_sorted_histogram(BIT_FILE * inFile, int which)
{
	unsigned long info_per_byte;
	vector <double> hist;
	sum_entropy = 0;
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
	if (which == 1)
		return sorted_hist;
	sort (sorted_hist.begin(), sorted_hist.end());

	
	return sorted_hist;
}

struct boolValue
{
	bool operator()(Node* node1, Node* node2)
	{
		return (node1->weight > node2->weight);
	}
};

template<typename T> void print_queue(T queue) {
    while(!queue.empty()) {
        // cout << "Element: " << queue.top() -> element << "   Weight: "<< queue.top() -> weight << "\n";
        queue.pop();
    }
    cout << '\n';
}

template<typename T> int length(T queue) {
	int count = 0;
    while(!queue.empty()) {
        count++;
        // cout << count << "\n";
        queue.pop();
    }
    return count;
}

//creating queue having leaves (Nodes)
priority_queue <Node*, vector <Node*>, boolValue > get_weights(char *argv[])
{
	BIT_FILE *inFile;
	inFile = OpenInputBitFile(argv[1]);
	vector < pair <double,int> > sorted_hist;
	priority_queue <Node*, vector <Node*>, boolValue > queueNodes; 

	sorted_hist = get_sorted_histogram(inFile,0);
	CloseInputBitFile(inFile);
	// vector <double> weights;
	// vector <int> elements;
	
	for (int i = 255; i >= 0; i--)
	{
		// cout << sorted_hist[i].first << " " << sorted_hist[i].second << "\n";
		if (sorted_hist[i].first == 0)
			{break;}
		//cout << sorted_hist[i].first;
		queueNodes.push(MakeNewNode(sorted_hist[i].first,sorted_hist[i].second));
	}
	// print_queue(queueNodes);
	return queueNodes;
}

Node* MakeTree(char *argv[])
{
	Node *leftNode, *rightNode, *tempNode;
	int totalWeight;
	priority_queue <Node*, vector <Node*>, boolValue > sortedQueueNodes;
	sortedQueueNodes = get_weights(argv);
	print_queue(sortedQueueNodes);
	// cout << "size" << length(sortedQueueNodes);	
	// cout << "Make tree \n";
	while(length(sortedQueueNodes) != 1)
	{
		// cout << "In loop " << length(sortedQueueNodes) << "\n";
		leftNode = sortedQueueNodes.top();
		sortedQueueNodes.pop();
		rightNode = sortedQueueNodes.top();
		sortedQueueNodes.pop();

		totalWeight = leftNode -> weight + rightNode -> weight;

		tempNode = MakeNewNode(totalWeight,300);
		tempNode -> left = leftNode;
		tempNode -> right = rightNode;
		sortedQueueNodes.push(tempNode);
		// break;
	}
	return sortedQueueNodes.top();
}
unsigned long getBinaryCode(string str){

	unsigned long temp = 0;
	// int countBinary = 0;
	while(str != "")
	{
		// countBinary++;
		codeCount++;
		char t = str.at(0);
		// cout << "What "<< t;
		if(t == '1')
		{
			// cout << "it is 1";
			temp = temp | 0x01;
			temp = temp << 1;
		}
		else
		{
			// cout << "it is 0";
			temp = temp | 0x00;
			temp = temp << 1;
		}
		// cout << str << "\n";
		str.erase(str.begin());

	}
	temp = temp >> 1;
	return temp;
}
void getCode(Node *root, string str, vector <unsigned long> &codewords, vector <int> &Length)
{	
	unsigned long tempCode;
	if (root == NULL){
        return;
	}
 	// cout << i << " " << root -> element << "\n";
    if (root->element != 300){
    	codeCount = 0;
    	tempCode = getBinaryCode(str);
        cout << "Symbol: " << root->element << " Codeword: " << str <<" Length: " << codeCount << "\n" ;
    	codewords[root->element] = tempCode;
    	Length[root->element] = codeCount;
    }
 
    getCode(root->left, str + "1", codewords, Length);
    getCode(root->right, str + "0", codewords, Length);
	
}
vector <unsigned long> getCodewords(Node *root, vector <int> &Length)
{
	vector <unsigned long> codewords(256);	
	unsigned long temp_code = 0;

	getCode(root, "", codewords, Length);
	// codewords[i] = temp_code;
	// cout << "Element: " << i << " Codeword: " << codewords[i] << "\n";

	return codewords;
	
}
int getCount(unsigned long code)
{
	int cnt = 1;

	if (code >= 1)
		cnt = int(floor((log(code)/log(2))+1));
	// cout << cnt << " ";
	return cnt;
}

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

int main(int argc, char *argv[])
{
	Node* root = NULL;
	// cout << "Checked 1\n";
	vector <unsigned long> codewords_sorted;
	vector <int> Length(256);
	vector < pair <double,int> > histogram;
	// cout << "Checked 2\n";
	root = MakeTree(argv);
	// cout << "Checked 3\n"; 
	codewords_sorted = getCodewords(root,Length);
	BIT_FILE *sampleFile;
	sampleFile = OpenInputBitFile(argv[1]);
	histogram = get_sorted_histogram(sampleFile,1);

	double avg_code_len = 0.0;
	for (int i = 0; i < 256; i++)
	{
		if (histogram[i].first > 0){
		// cout << histogram[i].first << "\n";
		avg_code_len = avg_code_len + (double(histogram[i].first)/sum_entropy)* Length[i];
		}
	}
	cout << "\n The average code length using Huffman coding is " << avg_code_len << "\n";
	PrintFile(&codewords_sorted, &Length, argv);
	// cout << "\n\n\n\n";
	// for (int i = 0; i < 126; i++)
	// {
	// 	cout << codewords_sorted[i] << "\n"; 
	// }



	return 0;
}
