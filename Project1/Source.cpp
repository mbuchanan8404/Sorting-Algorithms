/*
* Matthew Buchanan
* CS340
* Project 1
*/

/* Includes */
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <Windows.h>
using namespace std;

/* Function Prototypes */
void userInterface(int &sortChoice, int &fileSize, int &sorted);
void buildArray(string *&s, string f, int z);
void insertionSort(string *&l, int s);
void mergeSort(string *&l, int p, int r);
void merge(string *&a, int p, int q, int r);
void heap(string *&a, int s);
void heapify(string *&a, int i, int s);
void heapSort(string *&a, int s);
void buildHeap(string *&s, string f, int z);
void output(string *&a, double f, int s, int t, double t2);

/***** Begin Main *****/
int main()
{
	int run = 1;
	while (run)
	{
		int sortType = 0;
		int size = 0;
		int sorted = 2;
		string fileName;
		string * wordList = NULL;
		double time, time2;

		userInterface(sortType, size, sorted);
		if (!(sorted))
			fileName = "perm";
		else fileName = "sorted";
		fileName += to_string(size);
		fileName += "k.txt";
		cout << "Reading from file: " << fileName << endl;
		if (sortType == 1 || sortType == 2)
		{
			wordList = new string[size * 1000];
			buildArray(wordList, fileName, size);
			if (sortType == 1)
			{
				DWORD start = GetTickCount();
				insertionSort(wordList, size);
				DWORD end = GetTickCount();
				time = (end - start) / 1000.0;
				output(wordList, time, size * 1000, sortType, 0);
			}
			if (sortType == 2)
			{
				DWORD start = GetTickCount();
				mergeSort(wordList, 0, size * 1000);
				DWORD end = GetTickCount();
				time = (end - start) / 1000.0;
				output(wordList, time, size * 1000, sortType, 0);
			}
		}
		else if (sortType == 3)
		{
			wordList = new string[(size * 1000) + 1];
			buildHeap(wordList, fileName, size);/// note that this just creates an array and doesn't build the heap
			DWORD heapStart = GetTickCount();
			heap(wordList, (size * 1000) + 1);// this builds the heap
			DWORD heapEnd = GetTickCount();
			time2 = (heapEnd - heapStart) / 1000.0;
			DWORD sortStart = GetTickCount();
			heapSort(wordList, size * 1000);
			DWORD sortEnd = GetTickCount();
			time = (sortEnd - sortStart) / 1000.0;
			output(wordList, time, size * 1000, sortType, time2);
		}
		delete[] wordList;
		cout << endl << "Enter 1 to run again, 0 to quit." << endl;
		cin >> run;
		while (run != 1 && run != 0)
		{
			cout << endl << "Only 1 or 0 are allowed. Please try again.";
			cout << endl << "Enter 1 to run again, 0 to quit." << endl;
			cin >> run;
		}
	}
}
/***** End Main *****/

/* Displays the final output and writes it to file output.txt in the project directory */
void output(string *&a, double f, int s, int t, double t2)
{
	if (t == 1 || t == 2)
	{
		cout << "Time elapsed: " << f << " seconds." << endl;
		ofstream out;
		out.open("output.txt");
		for (int i = 0; i < s; i++)
		{
			out << a[i] << endl;
		}
		out.close();
		cout << "Output file: output.txt is located in the Project1 directory" << endl;
	}
	if (t == 3)
	{
		cout << "Time elapsed to build the heap: " << t2 << " seconds." << endl;
		cout << "Time elapsed to perform heap-sort: " << f << " seconds" << endl;
		ofstream out;
		out.open("output.txt");
		for (int i = 0; i < s; i++)
		{
			out << a[i] << endl;
		}
		out.close();
		cout << "Output file: output.txt is located in the Project1 directory" << endl;
	}
}

/* Prepares an array to be built into a heap by heap() */
void buildHeap(string *&s, string f, int z)
{
	fstream in;
	in.open(f);
	assert(in.is_open());
	int o = (z * 1000) + 1;
	while (!in.eof())
	{
		for (int i = 1; i < o; i++)
		{
			in >> s[i];
		}
	}
	in.close();
	in.clear();
	return;
}

/* Performs heap sort on the array a */
void heapSort(string *&a, int s)
{
	for (int i = s; i >= 1; i--)
	{
		string temp = a[1];
		a[1] = a[i];
		a[i] = temp;
		heapify(a, 1, i - 1);
	}
}

/* Maintains the heap parameters */
void heapify(string *&a, int i, int s)
{
	int left = 2 * i;
	string temp;
	temp = a[i];
	while (left <= s)
	{
		if (left < s && a[left + 1] > a[left])
			left++;
		if (temp > a[left])
			break;
		else if (temp <= a[left])
		{
			a[left / 2] = a[left];
			left *= 2;
		}
	}
	a[left / 2] = temp;
}

/* Builds a heap from the array a */
void heap(string *&a, int s)
{
	for (int i = s / 2; i >= 1; i--)
		heapify(a, i, s);
}

/* Merges the sub-arrays as part of heap sort */
void merge(string *&a, int p, int q, int r)
{
	int n1 = q - p + 1;
	int n2 = r - q;
	string * left = new string[n1];
	string * right = new string[n2];
	for (int i = 0; i < n1; i++)
		left[i] = a[p + i];
	for (int j = 0; j < n2; j++)
		right[j] = a[j + q + 1];
	int i = 0;
	int j = 0;
	int k = p;
	while (i < n1 && j < n2)
	{
		if (left[i] < right[j])
		{
			a[k] = left[i];
			i++;
		}
		else
		{
			a[k] = right[j];
			j++;
		}
		k++;
	}
	while (i < n1)
	{
		a[k] = left[i];
		i++;
		k++;
	}
	while (j < n2)
	{
		a[k] = right[j];
		j++;
		k++;
	}
	delete[] left;
	delete[] right;
}

/* Drives the merge sort algorithm */
void mergeSort(string *&a, int p, int r)
{
	if (p < r)
	{
		int q = ((p + r) / 2);
		mergeSort(a, p, q);
		mergeSort(a, q + 1, r);
		merge(a, p, q, r);
	}
}

/* Performs the insertion sort algorithm on array l */
void insertionSort(string *&l, int s)
{
	string key;
	int size = s * 1000;
	for (int i = 1; i < size; i++)
	{
		key = l[i];
		int j = i - 1;
		while (j >= 0 && l[j] > key)
		{
			l[j + 1] = l[j];
			j--;
		}
		l[j + 1] = key;
	}
}

/* Builds an array from the selected input file */
void buildArray(string *&s, string f, int z)
{
	ifstream in;
	in.open(f);
	assert(in.is_open());
	int o = z * 1000;
	while (!in.eof())
	{
		for (int i = 0; i < o; i++)
		{
			in >> s[i];
		}
	}
	in.close();
	in.clear();
	return;
}

/* User interface to allow user input */
void userInterface(int &sortChoice, int &fileSize, int &sorted)
{
	while (sortChoice > 3 || sortChoice < 1)
	{
		cout << endl << "Please select a sorting alogrithm by number." << endl;
		cout << "1: Insertion Sort" << endl;
		cout << "2: Merge Sort" << endl;
		cout << "3: Heap Sort" << endl;
		cin >> sortChoice;
		if (sortChoice > 3 || sortChoice < 1)
			cout << "Please try again." << endl;
	}
	while ((fileSize > 150 || fileSize < 30) || ((fileSize % 30) != 0))
	{
		cout << endl << "Please select a file size in thousands. ie: 30 , 60 , 90 , 120 , 150" << endl;
		cin >> fileSize;
		if ((fileSize > 150 || fileSize < 30) || ((fileSize % 30) != 0))
			cout << "Please try again." << endl;
	}
	while (sorted < 0 || sorted > 1)
	{
		cout << endl << "Enter 0 to process an unsorted list, and 1 for a sorted list." << endl;
		cin >> sorted;
		if (sorted < 0 || sorted > 1)
			cout << "Please try again" << endl;
	}
	return;
}

