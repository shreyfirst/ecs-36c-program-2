// Insertion Sort
// 
// Author: Rob Gysel
// ECS60, UC Davis
// Adapted from: Lysecky &Vahid "Data Structures Essentials", zyBooks
#include <vector>

std::vector<int> InsertionSort(std::vector<int> *numbers)
{
	int comparisons = 0;
	int memaccesses = 0;

	int i = 0;
	int j = 0;
	int temp = 0;	// Temporary variable for swap

	for (i = 1; i < numbers->size(); ++i)
	{
		++comparisons;
		j = i;
		// Insert numbers[i] into sorted part
		// stopping once numbers[i] in correct position
		while (j > 0 && (*numbers)[j]<(*numbers)[j - 1])
		{
			// increment memory access count (writing to j-1)

			// Swap numbers[j] and numbers[j - 1]
			++memaccesses;
			++memaccesses;
			temp = (*numbers)[j];
			++memaccesses;
			(*numbers)[j] = (*numbers)[j - 1];
			++memaccesses;
			++memaccesses;
			(*numbers)[j - 1] = temp;
			++memaccesses;

			--j;
			++comparisons;	// increment comparisons count
		}

		++memaccesses;
		++memaccesses;
		if (j <= 0)
		{ --comparisons;	// increment comparisons count
			--memaccesses;
			--memaccesses;	// increment memory access count (reading j)
		}
	}

	return {
		comparisons, memaccesses
	};

}