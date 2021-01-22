#include <iostream>
#include <string>


#include "utilities.hpp"

int max(int first, int second) {
    return first > second ? first : second;
}

std::string id_to_path(std::string id, std::string folder){
    size_t p = -1; 
    while ((p = id.find("//")) != std::string::npos) 
            id.replace(p, 2, "/");
    return folder+"/"+id+".json";
}

std::string removeWord(std::string str, std::string word)  
{ 
    if (str.find(word) != std::string::npos) 
    { 
        size_t p = -1; 
  
        std::string tempWord = word + " "; 
        while ((p = str.find(word)) != std::string::npos) 
            str.replace(p, tempWord.length(), " "); 
  
        tempWord = " " + word; 
        while ((p = str.find(word)) != std::string::npos) 
            str.replace(p, tempWord.length(), " "); 
    }

    return str; 
} 

// A utility function to swap two elements  
void swap(float* a, float* b)  
{  
    float t = *a;  
    *a = *b;  
    *b = t;  
}  
  
/* This function takes last element as pivot, places  
the pivot element at its correct position in sorted  
array, and places all smaller (smaller than pivot)  
to left of pivot and all greater elements to right  
of pivot */
int partition (float arr[],std::string str_arr[], int low, int high)  
{  
    float pivot = arr[high]; // pivot  
    int i = (low - 1); // Index of smaller element  
  
    for (int j = low; j <= high - 1; j++)  
    {  
        // If current element is smaller than the pivot  
        if (arr[j] < pivot)  
        {  
            i++; // increment index of smaller element  
            swap(&arr[i], &arr[j]);
            std::swap(str_arr[i],str_arr[j]);
        }  
    }  
    swap(&arr[i + 1], &arr[high]);  
    return (i + 1);  
}  
  
/* The main function that implements QuickSort  
arr[] --> Array to be sorted,  
low --> Starting index,  
high --> Ending index */
void quickSort(float arr[],std::string str_arr[],int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
        at right place */
        int pi = partition(arr, str_arr, low, high);
  
        // Separately sort elements before
        // partition and after partition
        quickSort(arr, str_arr ,low, pi - 1);
        quickSort(arr, str_arr ,pi + 1, high);
    }
}

//   How to use
// int n = sizeof(arr) / sizeof(arr[0]);
// quickSort(arr, 0, n - 1);


// unit_testing::unit_testing(std::string x){
//         id = x;
//     };
    
// unit_testing::~unit_testing(){};
