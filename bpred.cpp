#include <fstream>
#include <iostream>
#include <cstring>

#define NumPredictor 4

using namespace std;

/*
//// NOTE ABOUT UPLOADING YOUR CODE:

You can either copy this code and put it your PDF report. Or upload it in the cloud (e.g., Google Drive) and put the link in yur PDF file. 

////////////////////////////////////
*/


bool predict_A(int* myState, int pc, char outcome, int index, int GHR) // ignore pc and GHR
{
	// complete this function for part A
	bool predicted = false;

	// predict
	if(myState[index]>1)
		predicted = true;

	// update
	if (outcome == 'T' && myState[index]<3) myState[index]++;
	if (outcome == 'N' && myState[index]>0) myState[index]--;

	return predicted;
}

bool predict_B(int* myState, int pc, char outcome, int mode, int GHR) // ignore GHR
{
	return predict_A(myState, pc, outcome, pc, GHR); 
	
}

bool predict_C(int* myState, int pc, char outcome, int mode, int GHR) // ignore PC
{
	return predict_A(myState,pc,outcome,GHR,GHR);
}

bool (*p[3]) (int* myState, int pc, char outcome, int mode, int GHR) = {predict_A,predict_B,predict_C};

int main (int argc, char* argv[]) // the program runs like this: ./program <filename> <mode>
{
	// input file (i.e., test.txt)
	string filename = argv[1];
	int counter =0; 
	
	// choosing which predictor to use:
	int mode; // this shows which branch predictor (A, B, or C) we are using
	if (argc > 2)  
		mode = stoi(argv[2]); // the input could be either 0, 1, or 2 (for A, B and C)
	else mode = 0;// the default is Predictor A.
	

	ifstream fin;

	// opening file
	fin.open(filename.c_str());
	if (!fin){ // making sure the file is correctly opened
		cout << "Error opening " << filename << endl;
		exit(1);
	}
	
	// this will be used to store the current state of our 2-bit counters
	int myState[NumPredictor]; // For A, only myState[0] will be used. 

	// Global history register
	int GHR = 0; // initial state of GHR is 00. 

	// initializing the counters to "10" 
	for (int i=0; i<NumPredictor; i++)
		myState[i] = 2; // initial state is "10". i.e., weak taken

	// reading branches one by one
	// this is the main loop of the code
	int incorrect = 0; 
	while(fin && counter<20){
		int pc;
		char outcome;
		bool predicted;
		fin >> pc; // reading pc
		fin >> outcome; // reading outcome, i.e., T or N

		counter += 1; // keeping track of the total number of branches

		//cout << "GHR: " << (GHR>>1) << (GHR&1) << endl; 

		predicted = (*p[mode])(myState,pc,outcome, mode, GHR);

		//update GHR
		GHR <<= 1; 
		GHR |= (outcome=='T'); 
		GHR &= 3; 

		// print your results here, or write them to a file

		if ( (predicted == false && outcome == 'T') || (predicted == true && outcome == 'N') ) incorrect ++; 

		cout << pc << " " << outcome << " P: " << predicted << endl;
	}
		cout << "Incorrect: " << incorrect << endl; 
	// close file
	fin.close();
	return 0;
}
