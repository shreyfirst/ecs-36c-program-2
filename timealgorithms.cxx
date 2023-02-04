#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include "insertionsort.h"
#include "mergesort.h"
#include "quicksort.h"
#include "json.hpp"
using json = nlohmann::json;
using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cerr << "Error: Please provide a file name." << std::endl;
		return 1;
	}

	std::string file_name = argv[1];
	std::ifstream file(file_name);
	if (!file.is_open())
	{
		std::cerr << "Error: Failed to open file " << file_name << std::endl;
		return 1;
	}

	json input;
	file >> input;

	std::vector<std::string > keys;
	for (json::iterator it = input.begin(); it != input.end(); ++it)
	{
		if (it.key() != "metadata")
		{
			keys.push_back(it.key());
		}
	}

	// Open CSV file
	ofstream csvFile("output.csv");
	csvFile << "Sample,InsertionSortTime,InsertionSortCompares,InsertionSortMemaccess,"
	"MergeSortTime,MergeSortCompares,MergeSortMemaccess,"
	"QuickSortTime,QuickSortCompares,QuickSortMemaccess" << endl;

	// Process each sample
	for (const auto &key: keys)
	{
		std::vector<int> InsertionSample = input[key];
		std::vector<int> MergeSample = input[key];
		std::vector<int> QuickSample = input[key];

		// Insertion sort
		clock_t start = clock();
		vector<int> resultInsertion = InsertionSort(&InsertionSample);
		clock_t end = clock();
		double insertionSortTime = double(end - start) / CLOCKS_PER_SEC;
		int insertionSortCompares = resultInsertion[0];
		int insertionSortMemaccess = resultInsertion[1];

		// Merge sort
		start = clock();
		vector<int> resultMerge = MergeSort(&MergeSample);
		end = clock();
		double mergeSortTime = double(end - start) / CLOCKS_PER_SEC;
		int mergeSortCompares = resultMerge[0];
		int mergeSortMemaccess = resultMerge[1];

		// Quick sort
		start = clock();
		vector<int> resultQuick = QuickSort(&QuickSample);
		end = clock();
		double quickSortTime = double(end - start) / CLOCKS_PER_SEC;
		int quickSortCompares = resultQuick[0];
		int quickSortMemaccess = resultQuick[1];

		// Write to CSV file
		std::cout << key << "," << std::to_string(insertionSortTime) << "," << insertionSortCompares <<
			"," << insertionSortMemaccess << "," << std::to_string(mergeSortTime) << "," <<
			mergeSortCompares << "," << mergeSortMemaccess << "," << std::to_string(quickSortTime) <<
			"," << quickSortCompares << "," << quickSortMemaccess << endl;
	}

	csvFile.close();
	return 0;
}