//Ryan Schleritzauer
//Jacobi Iteration 
//This program heats up a heating plate until the episolon value is hit, or the counter hits the max iterations
//THe number that the serial solution answer is is 3371

#include <iostream>
#include <ctime>

#include <fstream>
using namespace std;

// 1002 because of the constant area around
const int maxSize=1002;

float array1[maxSize][maxSize];
float array2[maxSize][maxSize];
const float eplsilonCutOff = 0.01;
const int maxIterations = 5000;

clock_t startIteration,endIteration,startProgram, endProgram;
float differenceIteration=0.0;
float differenceProgram=0.0;
int numThreadsCounter=4;
void eyeCandy(int counter,float changedTempFromIteration);
void printArray();
float calculateChange(int numThreadsCounter);

int main()
{
	//runs the program multiple times to see best thread vs time
	

		startProgram = clock();
		int counter = 1;
		const float linearyDifferenceConstant = (float)100/(maxSize);
		float linearyDifference = linearyDifferenceConstant;
		float changedTempFrom1Interation=100.0;

		//set up the initial heating plate with the constants that need set
		for(int r=0; r<maxSize;r++)
		{
			for(int c=0; c<maxSize; c++)
			{
				array1[r][c] =0;
			}
		}
	
		//right side
		for(int c=0; c<maxSize;c++)
		{
			array1[c][maxSize-1] = linearyDifference;
			linearyDifference = linearyDifference + linearyDifferenceConstant;
		
		}
	
		linearyDifference = linearyDifferenceConstant;
		//bottom side
		for(int r=0; r<maxSize; r++)
		{
			array1[maxSize-1][r] = linearyDifference;
			linearyDifference = linearyDifference + linearyDifferenceConstant;
		}
	
	
#pragma acc data copy(array1),create(array2)
	//breaks out of loop if counter <= max iterations
		while(counter <= maxIterations && changedTempFrom1Interation >= eplsilonCutOff)
		{
			//startIteration = clock();
			changedTempFrom1Interation = calculateChange(numThreadsCounter); //goes to the calculate change function, which does an interation across the plate changing the temp
			//endIteration = clock();
			//every 100 iterations print out info for the user
			if(counter % 100 == 0)
			{
				eyeCandy(counter,changedTempFrom1Interation);
			}
			//printArray();
			counter++;
		
		}

			//if epsiloncutoff is hit, print out info and set counter to 5001 and kick out of loop
			if(changedTempFrom1Interation <= eplsilonCutOff)
			{
				eyeCandy(counter,changedTempFrom1Interation);

				endProgram = clock();
				ofstream myfile; //outputs results to text file for testing purposes
				myfile.open ("results.txt",ios::app);
				myfile << "Iteration count is " << counter << endl;
				myfile <<  "The max temperature change for this iteration was " << changedTempFrom1Interation << endl;
				myfile << "The number of threads was " << numThreadsCounter << endl;
				myfile << "The time it took for the whole program to run was " << (float)((endProgram - startProgram)/CLOCKS_PER_SEC) << " seconds" << endl;
				myfile << endl;
				myfile << endl;
				myfile.close();
				cout << "The time it took for the whole program to run was " << (float)((endProgram - startProgram)/CLOCKS_PER_SEC) << " seconds" << endl;
			}
			else if(counter <=maxIterations)
			{
				cout << "You hit max iterations" << endl;
			}
		
			
	
		system("pause");
		return 0;

}

//prints the array out for testing purposes
void printArray()
{
	for(int c=0; c < maxSize; c++)
	{
		for(int r=0; r < maxSize; r++)
		{
			cout << array1[c][r] << " ";
		}
		cout << endl;
	}
}
//prints out output for the user to see
void eyeCandy(int counter,float changedTempFromIteration)
{

	cout << "Iteration count is " << counter << endl;
	cout << "The max temperature change for this iteration was " << changedTempFromIteration << endl;
	//cout << "The time it took for the last iteration was " << (float)((endIteration - startIteration)/CLOCKS_PER_SEC) <<" seconds" << endl;

	for(int i=1; i <= 5; i++)
	{
		cout << array1[maxSize-i][maxSize-i] << endl;
	}



}
//this funcation calculates the change between the places on the plate

float calculateChange(int numThreadsCounter)
{
	
	float minTempChanged = 0.0;
	
	

	float changedTemp = 0.0;

#pragma acc parallel loop reduction(max:minTempChanged)
		for(int c=1; c<maxSize-1; c++)
		{
			for(int r=1;r<maxSize-1;r++)
			{
			  array2[c][r] = ((array1[c][r+1] + array1[c][r-1] + array1[c-1][r] + array1[c+1][r]) /4); //takes the average of the 4 neighbors 
	
			  minTempChanged = max(minTempChanged,abs(array2[c][r] - array1[c][r])); //checks to see if it is smaller than last temp change
				 
			 
			}


		}
#pragma acc parallel loop
		for(int c=0; c<maxSize-1; c++)
		{
			for(int r=0; r<maxSize-1;r++)
			{
				array1[c][r] = array2[c][r];
			}
		}
	

	return minTempChanged; //return the minTempChanged to main to check for epislon 
}






 	