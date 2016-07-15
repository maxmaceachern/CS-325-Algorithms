/***********************************************
** Title: Project 1, Max Sub Array Problem
** Authors: W McCumstie, W Kim, M MacEachern
** Date: 04/14/2016
** Desc: Compares 4 different alogirthms
***********************************************/
#include <iostream>
#include <fstream>
#include <math.h>
#include <chrono>
#include <stdlib.h>
using namespace std;
typedef std::chrono::system_clock timeHR;


/***********************************************
** Name: maxOutput
** Desc: Output object of the algorithm functions
***********************************************/
class maxOutput {
	public:
		int max;	// The max values
		int i;		// Start of max sub array
		int j;		// End of the max sub array
};


/***********************************************
** Name: Algorithm 1, Enumeration
** Desc: Solves the max sub array problem by
**		 enumerating over all possible indices
***********************************************/
maxOutput algorithm1(int *arr, int size) {
	int sum;
	maxOutput max;
	max.max = arr[0];
	max.i = 0;
	max.j = 0;
	// Loops through the indices
	for (int i = 0; i < size; i++) {
		for (int j = i; j < size; j++) {
			// Finds and compares the sum/ max sum
			sum = 0;
			for (int c = i; c <= j; c++) {
				sum += arr[c];
			}
			if (max.max < sum) {
				max.max = sum;
				max.i = i;
				max.j = j;
			}
		}
	}
	return max;
}


/***********************************************
** Name: Algorithm 2, Better Enumeration
** Desc: Sames as Algorithm 1 EXCEPT builds on
**		 previously calculated sums
***********************************************/
maxOutput algorithm2(int *arr, int size) {
	int sum;
	maxOutput max;
	max.max = arr[0];
	max.i = 0;
	max.j = 0;
	// Loops through the indices
	for (int i = 0; i < size; i++) {
		sum = 0;
		for (int j = i; j < size; j++) {
			// Finds and compares the sum/ max sum
			sum += arr[j];
			if (max.max < sum) {
				max.max = sum;
				max.i = i;
				max.j = j;
			}
		}
	}
	return max;
}


/***********************************************
** Name: Algorithm 3, Divide and Conquer
** Desc: Splits the array in half and recursively
**		 solves for the max
***********************************************/
maxOutput algorithm3(int *arr, int size) {
	// ENDS THE RECURSION WHEN SIZE IS 1
	maxOutput max;
	if (size == 1) {
		max.max = arr[0];
		max.i = 0;
		max.j = 0;
		return max;
	}

	// Case 1: Max sub array is in the first half
	int halfSize = size / 2;
	maxOutput max1 = algorithm3(arr, halfSize);

	// Case 2: Max sub array is in the second half
	maxOutput max2 = algorithm3(&arr[halfSize], size - halfSize);
	max2.i += halfSize;
	max2.j += halfSize;

	// Case 3: Max sub array spans first and second half of arr
	// Find the max suffix of the first half of arr
	maxOutput max3;
	int midPoint = halfSize - 1;	// Max INDEX of the first half
	int maxSuffix = arr[midPoint];
	int sumSuffix = 0;
	max3.i = midPoint;
	for (int c = midPoint; c >= 0; c--) {
		sumSuffix += arr[c];
		if (sumSuffix > maxSuffix) {
			maxSuffix = sumSuffix;
			max3.i = c;
		}
	}
	// Find the max prefix of the second half of the array
	int maxPrefix = arr[midPoint + 1];
	int sumPrefix = 0;
	max3.j = midPoint + 1;
	for (int c = midPoint + 1; c < size; c++) {
		sumPrefix += arr[c];
		if (sumPrefix > maxPrefix) {
			maxPrefix = sumPrefix;
			max.j = c;
		}
	}
	// Max of case 3
	max3.max = maxPrefix + maxSuffix;

	// Returns the largest max of case[1, 2, 3]
	if ((max1.max > max2.max) && (max1.max > max3.max))
		return max1;
	else if (max2.max > max3.max)
		return max2;
	else
		return max3;
}


/***********************************************
** Name: Algorithm 4, Recursion Inversion
** Desc: Iterates through the array knowning that
**		 the max is either max sub arr[1,..,n -1]
**		 OR max suffix arr[1,..,n - 1]
***********************************************/
maxOutput algorithm4(int *arr, int size) {
	maxOutput maxSuf;
	maxSuf.max = arr[0];
	maxSuf.i = 0;
	maxSuf.j = 0;
	maxOutput maxSub;
	maxSub.max = arr[0];
	maxSub.i = 0;
	maxSub.j = 0;
	int newSuf;
	for (int c = 1; c < size; c++) {
		// Calculates new max suffix
		newSuf = maxSuf.max + arr[c];
		if (newSuf > arr[c]) {
			maxSuf.max = newSuf;
			maxSuf.j = c;
		}
		else {
			maxSuf.max = arr[c];
			maxSuf.i = c;
			maxSuf.j = c;
		}

		// Finds the new max sub array
		if (maxSuf.max > maxSub.max)
			maxSub = maxSuf;
	}
	return maxSub;
}


/***********************************************
** Name: timeFuncArray
** Desc: Returns the run time of a input function
**		 with a single array input
***********************************************/
double timeFuncArray(maxOutput (*func)(int *a, int s), int *arr, int size) {
	timeHR::time_point start;
	timeHR::time_point end;
	start = timeHR::now();
	func(arr, size);
	end = timeHR::now();
	auto diff = end - start;
	return (diff.count());
}


/*********************************************
** Function: createArray
** Desc: Randomly generates an array of n elements
**		 in range [lo, hi]
** Precond: The random number generator is seeded
**			Max size of n is 100000
*********************************************/
void createArray(int* &ptr, int n, int lo, int hi) {
	ptr = new int[n];
	if (hi < lo)
		throw;
	for (int c = 0; c < n; c++) 
		ptr[c] = rand() % (hi - lo + 1) + lo;
}


/***********************************************
** Name: printAlgorithmData
** Desc: Prints the algorithm data to the file
***********************************************/
void printAlgorithmData(ofstream &outFile, maxOutput(*func)(int *a, int s), int *arr, int size) {
	maxOutput max = func(arr, size);
	outFile << "[" << arr[max.i];
	for (int c = max.i + 1; c <= max.j; c++)
		outFile << ", " << arr[c];
	outFile << "]" << endl << max.max << endl;
}


/***********************************************
** Name: main
** Desc: Main running code
***********************************************/
#if 1
int main() {
	// Opens the files
	ifstream file;
	ofstream outFile;
	file.open("MSS_Problems.txt");
	outFile.open("MSS_Results.txt");
	if ((!file.is_open()) || (!outFile.is_open())) {
		cout << "File failed to open" << endl;
		return 0;
	}

	// Variable decleration
	int length;
	int k;
	int *arr;
	int sum;
	int index;
	bool positive = true;

	// Reads the input, calculates and outputs
	char str[256];
	while (!file.eof()) {
		for (int c = 0; c < 256; c++)
			str[c] = 0;
		file.getline(str, 256);

		// Calculates the size of the array
		length = 1;
		k = 0;
		while (str[k] != 0) {
			if (str[k] == 44) // 44 = ","
				length++;
			k++;
		}
		str[k] = 44; // Adds a delimiter to the end of the array

		// Reads the data from the array
		arr = new int[length];
		k = 0;
		sum = 0;
		index = 0;
		while (str[k] != 0) {
			// Deliminter - Saves sum to the array
			if (str[k] == 44) {
				if (positive)
					arr[index] = sum;
				else
					arr[index] = 0 - sum;
				positive = true;
				sum = 0;
				index++;
			}

			// Detects negative numbers
			else if (str[k] == 45) // "-" = 45
				positive = false;

			// Converts the number from ASCII to INT
			else if ((str[k] > 47) && (str[k] < 58))
				sum = sum * 10 + str[k] - 48;
			k++;
		}

		// Prints the array to the output file
		k = 0;
		while (str[k] != 93) { // 93 = "]"
			outFile << str[k];
			k++;
		}
		outFile << "]" << endl;
		
		// Prints the algorithm data
		printAlgorithmData(outFile, algorithm1, arr, length);
		printAlgorithmData(outFile, algorithm2, arr, length);
		printAlgorithmData(outFile, algorithm2, arr, length);
		printAlgorithmData(outFile, algorithm4, arr, length);
		outFile << endl;

		// Deletes the array
		delete arr;
	}
	file.close();
	outFile.close();
}


#elif 1
// Code used to generate run time data
int main() {
	srand(time(0));
	int *arr;
	int n1[] = { 10, 100, 200, 400, 500, 700, 900, 1100, 1300, 1500 };
	int n2[] = { 100, 1000, 2500, 5000, 7500, 10000 , 15000, 20000, 25000, 30000 };
	int n3[] = { 100, 750000, 1250000, 2250000, 3500000, 5000000, 6500000, 7500000, 9000000, 10000000 };
	int n4[] = { 100, 27000000, 55000000, 83000000, 110000000, 140000000, 170000000 , 200000000, 220000000, 250000000 };
	int nSize = 10;
	ofstream file;
	file.open("data.csv");
	for (int c = 0; c < nSize; c++) {
		cout << "Loop: " << c + 1 << "/10" << endl;
		createArray(arr, n1[c], -100, 100);
		file << n1[c] << ", " << timeFuncArray(algorithm1, arr, n1[c]) << ", , ";
		delete arr;
		createArray(arr, n2[c], -100, 100);
		file << n2[c] << ", " << timeFuncArray(algorithm2, arr, n2[c]) << ", , ";
		delete arr;
		createArray(arr, n3[c], -100, 100);
		file << n3[c] << ", " << timeFuncArray(algorithm3, arr, n3[c]) << ", , ";
		delete arr;
		createArray(arr, n4[c], -100, 100);
		file << n4[c] << ", " << timeFuncArray(algorithm4, arr, n4[c]) << endl;
		delete arr;
	}
	file.close();
}


#else
// Code used in debugging
int main() {
	// TEST CONDITIONS
	bool changeListEachTime = true; 	// Set to true to randomly seed
	int seedValue;
	if (changeListEachTime) {
		seedValue = time(0);			// DO NOT CHANGE
		cout << "Array change each run and loop" << endl;
	}
	else {
		seedValue = 10;					// Constant seed value when false
		cout << "Array changes each loop BUT constant each run" << endl;
		cout << "The random number generator seed value is (constant): " << seedValue << endl;
	}
	// END TEST CONDITIONS

	srand(seedValue);
	int order;
	int size;
	int *arr;
	maxOutput max[4];
	for (int c = 0; c < 1000; c++) {
		order = pow(10, rand() % 3 + 1);
		size = rand() % (order - order / 10) + order / 10 + 1;
		cout << "Loop " << c << ", array size = " << size << endl;
		createArray(arr, size, -100, 100);
		max[0] = algorithm1(arr, size);
		max[1] = algorithm2(arr, size);
		max[2] = algorithm3(arr, size);
		max[3] = algorithm4(arr, size);
		for (int k = 1; k < 4; k++) {
			if (max[0].max != max[k].max) {
				cout << endl << "ERROR VALUES DO NOT MATCH" << endl;
				cout << "Review algorithm1 [max = " << max[0].max << "] or ";
				cout << "algorithm" << k + 1 << " [max = " << max[k].max << "]" << endl;
				cout << "[" << arr[0];
				for (int i = 1; i < size; i++)
					cout << ", " << arr[i];
				cout << "]" << endl << endl;
			}
 		}
		delete arr;
	}
	return 0;
}
#endif