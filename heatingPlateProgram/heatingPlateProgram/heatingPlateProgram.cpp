//Ryan Schleritzauer
//Jacobi Iteration
//This program heats up a heating plate until the episolon value is hit, or the counter hits the max iterations
//THe number that the serial solution answer is is 3368

#include <iostream>
#include <ctime>
#include <cmath>
#include <omp.h>
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
int numThreadsCounter=8;
void eyeCandy(int counter,float changedTempFromIteration);
void printArray();
float calculateChange(int numThreadsCounter);

int main()
{
	//runs the program multiple times to see best thread vs time


		startProgram = clock();
		int counter = 0;
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

			array1[c][maxSize-1] = (c*100)/(maxSize-1.0);

		}

		//bottom side
		for(int r=0; r<maxSize; r++)
		{


			array1[maxSize-1][r] = (r*100)/(maxSize-1.0);
		}

	//(r*100)/(MAX-1)

	//breaks out of loop if counter <= max iterations
	//#pragma acc data copy(array1),create(array2)
		while(counter <= maxIterations && changedTempFrom1Interation >= eplsilonCutOff)
		{
			//startIteration = clock();
		//	changedTempFrom1Interation = calculateChange(numThreadsCounter); //goes to the calculate change function, which does an interation across the plate changing the temp


					changedTempFrom1Interation = 0.0;

		omp_set_num_threads(numThreadsCounter);

#pragma omp parallel reduction(max:changedTempFrom1Interation)
//{

	#pragma omp for
	//#pragma acc parallel loop reduction(max:changedTempFrom1Interation)
		for(int c=1; c<maxSize-1; c++)
		{
			for(int r=1;r<maxSize-1;r++)
			{
			  array2[c][r] = ((array1[c][r+1] + array1[c][r-1] + array1[c-1][r] + array1[c+1][r]) /4); //takes the average of the 4 neighbors

			  changedTempFrom1Interation = std::max(changedTempFrom1Interation,(float)(abs(array2[c][r] - array1[c][r]))); //checks to see if it is smaller than last temp change
				//minTempChanged = max(minTempChanged,(array2[c][r] - array1[c][r])); //checks to see if it is smaller than last temp change

			}


		}
#pragma omp for
//#pragma acc parallel loop
		for(int c=0; c<maxSize-1; c++)
		{
			for(int r=0; r<maxSize-1;r++)
			{
				array1[c][r] = array2[c][r];
			}
		}
//}






			//endIteration = clock();
			//every 100 iterations print out info for the user
		//	if(counter % 100 == 0)
			//{
				//eyeCandy(counter,changedTempFrom1Interation);
			//}
			//printArray();
			counter++;

		}

			//if epsiloncutoff is hit, print out info and set counter to 5001 and kick out of loop
			if(changedTempFrom1Interation <= eplsilonCutOff)
			{
				eyeCandy(counter,changedTempFrom1Interation);

				endProgram = clock();

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

//float calculateChange(int numThreadsCounter)
//{

	//float minTempChanged = 0.0;

	//omp_set_num_threads(numThreadsCounter);
//#pragma omp parallel reduction(max:minTempChanged)
	//{
//#pragma omp for
		//for(int c=1; c<maxSize-1; c++)
	//	{
			//for(int r=1;r<maxSize-1;r++)
			//{
			  //array2[c][r] = ((array1[c][r+1] + array1[c][r-1] + array1[c-1][r] + array1[c+1][r]) /4); //takes the average of the 4 neighbors

			  //minTempChanged = std::max(minTempChanged,(float)(abs(array2[c][r] - array1[c][r]))); //checks to see if it is smaller than last temp change


			//}


		//}
//#pragma omp for
		//for(int c=0; c<maxSize-1; c++)
		//{
			//for(int r=0; r<maxSize-1;r++)
			//{
				//array1[c][r] = array2[c][r];
			//}
		//}
	//}


	//return minTempChanged; //return the minTempChanged to main to check for epislon
//}
