// Quicksort
// 
// Author: Rob Gysel
// ECS60, UC Davis
// Adapted from: Lysecky &Vahid "Data Structures Essentials", zyBooks
#include "quicksort.h"

std::vector<int> QuickSort(std::vector<int> *numbers)
{
	int comparisons = 0;
	int memaccesses = 0;
	QuickSortRecurse(numbers, 0, numbers->size() - 1, comparisons, memaccesses);
	return {
		comparisons, memaccesses
	};

}

void QuickSortRecurse(std::vector<int> *numbers, int i, int k, int &comparisons, int &memaccesses)
{
	int j = 0;
	if (i >= k)
	{
		return;
	}

	j = Partition(numbers, i, k, comparisons, memaccesses);
	QuickSortRecurse(numbers, i, j, comparisons, memaccesses);
	QuickSortRecurse(numbers, j + 1, k, comparisons, memaccesses);
}

int Partition(std::vector<int> *numbers, int i, int k, int &comparisons, int &memaccesses)
{
	int l = 0, h = 0, midpoint = 0, pivot = 0, temp = 0;
	bool done = false;
	midpoint = i + (k - i) / 2;
   ++memaccesses;
	pivot = (*numbers)[midpoint];
	l = i;
	h = k;
	while (!done)
	{
      
		while ((*numbers)[l] < pivot)
		{ ++l;
			++comparisons;
			++memaccesses;
		}
      			++comparisons;
			++memaccesses;

		while (pivot<(*numbers)[h])
		{ --h;
			++comparisons;
			++memaccesses;
		}
      			++comparisons;
			++memaccesses;

		if (l >= h)
		{
         
			done = true;
		}
		else
		{
			temp = (*numbers)[l];
			(*numbers)[l] = (*numbers)[h];
			(*numbers)[h] = temp;
			++l;
			--h;
			memaccesses += 4;
		}
	}

	return h;
}