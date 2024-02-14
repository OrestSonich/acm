#include <iostream>
#include "timer.h"
#include <Windows.h>

void insertSort(int* array, int leftIndex, int rightIndex);

void shellSort(int* array, int leftIndex, int rightIndex);

void printVector(void* data, int count, int DATA_SIZE);

using namespace std;

int main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	const int DATA_SIZE = 32;
	const int ITERATION = 100000;
	int data[DATA_SIZE] = { 1,20,29,28,10,26,25,24,23,22,21,
		30,19,18,17,16,15,14,13,12,11,
		27,9,8,7,6,5,4,3,2,0,31 };

	Timer timer;

	timer.start();
	for (int i = 0; i < ITERATION; i++)
	{
	insertSort(data, 0, DATA_SIZE-1);
	}
	timer.stop();

	double insertionSortTime = timer.getResultInMillis();

	cout << "Час виконання алгоритму \'Сортування включенням': " << insertionSortTime << " мс." << endl;
	printVector(data, 0, DATA_SIZE);


	timer.start();
	for (int i = 0; i < ITERATION; i++)
	{
	shellSort(data, 0, DATA_SIZE - 1);
	}
	timer.stop();

	double shellSortTime = timer.getResultInMillis();

	cout << "Час виконання алгоритму \'Сортування Шелла': " << shellSortTime << " мс." << endl;
	printVector(data, 0, DATA_SIZE);

	cout << "Час сортування включенням - час сортування Шелла = " << insertionSortTime - shellSortTime << " мс." << endl;
	
	return 0;
}

void insertSort(int* array, int leftIndex, int rightIndex) {
	int iIndex, jIndex;

	for (iIndex = leftIndex + 1; iIndex <= rightIndex; ++iIndex) {
		int temp = array[iIndex];
		jIndex = iIndex - 1;
		for (; jIndex >= leftIndex && array[jIndex] > temp; --jIndex) {
			array[jIndex + 1] = array[jIndex];
		}
		array[jIndex + 1] = temp;
	}
}

void shellSort(int* array, int leftIndex, int rightIndex) {
	int dIndex, iIndex, jIndex;

	int sortingSize = rightIndex - leftIndex + 1;
	for (dIndex = leftIndex + sortingSize / 2; dIndex >= leftIndex + 1; dIndex /= 2) {
		for (iIndex = dIndex; iIndex < sortingSize; iIndex++) {
			for (jIndex = iIndex; jIndex >= dIndex && array[jIndex - dIndex] >
				array[jIndex]; jIndex -= dIndex) {
				int temp = array[jIndex];
				array[jIndex] = array[jIndex - dIndex];
				array[jIndex - dIndex] = temp;
			}
		}
	}
}

void printVector(void* data, int count, int DATA_SIZE) {
	int index = 0;
	for (index = 0; (!count || index < count) && index < DATA_SIZE; index++) {
		printf("%d ", ((int*)data)[index]);
	}
	printf("\n");
}
