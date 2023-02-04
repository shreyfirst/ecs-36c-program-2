// Merge Sort
// 
// Author: Rob Gysel
// ECS60, UC Davis
// Adapted from: Lysecky &Vahid "Data Structures Essentials", zyBooks
#include "mergesort.h"

std::vector < int > MergeSort(std::vector < int > * numbers) {
  int comparisons = 0;
  int memaccesses = 0;
  MergeSortRecurse(numbers, 0, numbers -> size() - 1, comparisons, memaccesses);
  return {
    comparisons,
    memaccesses
  };

}

void MergeSortRecurse(std::vector < int > * numbers, int i, int k, int & comparisons, int & memaccesses) {
  int j = 0;

  if (i < k) {
    j = (i + k) / 2; // Find the midpoint in the partition

    // Recursively sort left and right partitions
    MergeSortRecurse(numbers, i, j, comparisons, memaccesses);
    MergeSortRecurse(numbers, j + 1, k, comparisons, memaccesses);

    // Merge left and right partition in sorted order
    Merge(numbers, i, j, k, comparisons, memaccesses);
  }
}

void Merge(std::vector < int > * numbers, int i, int j, int k, int & comparisons, int & memaccesses) {
  int mergedSize = k - i + 1; // Size of merged partition
  int mergePos = 0; // Position to insert merged number
  int leftPos = 0; // Position of elements in left partition
  int rightPos = 0; // Position of elements in right partition
  std::vector < int > mergedNumbers;
  mergedNumbers.resize(mergedSize); // Dynamically allocates temporary array
  // for merged numbers

  leftPos = i; // Initialize left partition position
  rightPos = j + 1; // Initialize right partition position

  // Add smallest element from left or right partition to merged numbers
  while (leftPos <= j && rightPos <= k) {
    if (( * numbers)[leftPos] < ( * numbers)[rightPos]) {
      mergedNumbers[mergePos] = ( * numbers)[leftPos];
      memaccesses++;
      memaccesses++;
      ++leftPos;
    } else {
      mergedNumbers[mergePos] = ( * numbers)[rightPos];
      memaccesses++;
      memaccesses++;
      ++rightPos;
    }

    memaccesses++;
    memaccesses++;
    comparisons++;
    ++mergePos;
  }

  // If left partition is not empty, add remaining elements to merged numbers
  while (leftPos <= j) {
    mergedNumbers[mergePos] = ( * numbers)[leftPos];
    memaccesses++;
    memaccesses++;
    ++leftPos;
    ++mergePos;
  }

  // If right partition is not empty, add remaining elements to merged numbers
  while (rightPos <= k) {
    mergedNumbers[mergePos] = ( * numbers)[rightPos];
    memaccesses++;
    memaccesses++;
    ++rightPos;
    ++mergePos;
  }

  // Copy merge number back to numbers
  for (mergePos = 0; mergePos < mergedSize; ++mergePos) {
    ( * numbers)[i + mergePos] = mergedNumbers[mergePos];
    memaccesses++;
    memaccesses++;
  }
}