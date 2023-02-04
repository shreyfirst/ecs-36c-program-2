// Quicksort
// 
// Author: Rob Gysel
// ECS60, UC Davis
// Adapted from: Lysecky & Vahid "Data Structures Essentials", zyBooks

#include <vector>

std::vector<int> QuickSort(std::vector<int>* numbers);
void QuickSortRecurse(std::vector<int>* numbers, int i, int k, int &comparisons, int &memaccesses);
int Partition(std::vector<int>* numbers, int i, int k, int &comparisons, int &memaccesses);