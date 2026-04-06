#include <iostream>
#include <string>

using namespace std;

// write a function to print the contents of your array
//   bonus (+5% to your grade for this lab)! try to implement your function so that it can accept
//     many types of data (int, char, string, etc.)
template <typename flexibleType> // ???
// We make this a function template so it can print arrays of many element types.
// The function takes a raw array and its size, prints elements separated by spaces,
// and ends with a newline. Works for int, double, char, string, etc. as long as
// operator<< is defined for the element type.
void printArray(flexibleType arr[], int n) {
    for (int i = 0; i < n; ++i) {
        cout << arr[i];
        if (i < n - 1) {
            cout << " ";  // space-separate, but no trailing space after the last element
        }
    }
    cout << endl;
}

// Implement a sequential search algorithm
// your function should search for a target value (target)
//    within the indices of "start" to "end"
// return true if target exists in the array within this range,
//    return false otherwise
bool seqSearch(string target, string arr[], int start, int end) {
    // Linear scan from start to end. Return as soon as we find target.
    // Assumes start/end are valid indexes.
    for (int i = start; i <= end; i++) {
        if (arr[i] == target) {
            return true;
        }
    }
    return false;
}

// Implement an iterative binary search 
// Return true if target exists in the array with size n,
//    return false otherwise 
bool binSearch(float target, float arr[], int n) {
    // Classic binary search on ASCENDING array of size n.
    // We keep a low/high index window and narrow it until found or empty.
    int lo = 0;
    int hi = n - 1;
    while (lo <= hi) {
        // Midpoint computed this way avoids potential overflow: lo + (hi - lo)/2
        int mid = lo + (hi - lo) / 2;

        if (arr[mid] == target) {
            return true;  // We found the target
        } else if (arr[mid] < target) {
            lo = mid + 1; // target is in the right half
        } else {
            hi = mid - 1; // target is in the left half
        }
    }
    return false;	
}

// Implement a recursive binary search 
// Return true if target exists in the array with size n
//   return false otherwise
bool binSearchR(char target, char charray[], int n) {
    // base case
    // If the subarray length is 0, there is nothing to search.
    if (n <= 0) {
        return false;
    }

    // Compute middle index of current subarray.
    int mid = n / 2;

    //general case
    if (charray[mid] == target) {
        return true;                     // found at the middle
    } else if (target < charray[mid]) {
        // Recurse on the left half: same base pointer, length = mid
        return binSearchR(target, charray, mid);
    } else {
        // Recurse on the right half: advance pointer by mid+1, reduce length
        return binSearchR(target, charray + mid + 1, n - mid - 1);
    }
}

// Implement a brand new sorting algorithm
//    Don't use a reference for this!
//    Be sure to do your best to implement the algorithm's logical steps
// Step 1: implement a function swap() that will swap any two elements in an array,
//          given their indices
// Step 2: implement a function minFind() that will find and return 
//          the **index** of the smallest element in an array
/*
Step 3: Finally, use your two functions above to complete the following in newSort():
        1 - search through the array to find the location of the smallest value
        2 - swap this value with the value at the start of the array
        3 - the first element is now sorted! 
            Redo your sort on the remaining elements (from index 1 to end)
            On the next iteration, you should be swapping the smallest remaining value 
                with the second index value in the array
            ...then swapping the smallest remaining value with the third indexed value... etc.
            
        4 - continue iterating until you reach the end of the list
        *** You can make this recursive, if you wish!
*/

// Step 1: swap two elements in-place, given their indices i and j.
// Works on the provided double[] the tests use.
void swap(double darray[], int i, int j) {
    double tmp = darray[i];
    darray[i] = darray[j];
    darray[j] = tmp;
}

// Step 2: return the index (0..n-1) of the smallest element in darray.
// If n <= 0 we can return -1 to indicate no valid minimum.
int minFind(double darray[], int n) {
    if (n <= 0) {
        return -1;
    }
    int minIdx = 0;                  // start by assuming index 0 is the minimum
    for (int i = 1; i < n; ++i) {    // scan the rest and update when we find a smaller value
        if (darray[i] < darray[minIdx]) {
            minIdx = i;
        }
    }
    return minIdx;
}

// Step 3: Selection sort using minFind + swap.
// For each position i, find the smallest element in the suffix [i..n-1],
// then swap it into position i. After i advances, the prefix [0..i] is sorted.
void newSort(double darray[], int n) {
    for (int i = 0; i < n; ++i) {
        // Find index of smallest element in the remaining subarray darray[i..n-1].
        // We call minFind on the subarray that starts at darray + i and has length n - i.
        int relMin = minFind(darray + i, n - i); // index relative to i
        int absMin = i + relMin;                 // convert to absolute index in original array

        // Place that smallest element at position i.
        swap(darray, i, absMin);
    }
}


// Implement Exponential Search
//    Reuse your iterative binSearch(...) on a narrowed window
//    Assumes the array is sorted in ascending order
// Step 1: Handle edge cases (empty array) and quickly check the first element.
// Step 2: Grow a “bound”: start at 1, then repeatedly double it
//         while you are still inside the array and the current value is less than the target.
// Step 3: Define a search window:
//         - The left edge is halfway back from the current bound.
//         - The right edge is the smaller of (current bound) and (last valid index).
// Step 4: Run your existing binSearch on just that window and return its result.
//         You may pass a pointer to the start of the window and its length,
//         or copy that window to a temporary buffer if you prefer.
bool expSearch(float target, float arr[], int n) {
    // Guard against empty input: there isnothing to find.
    if (n <= 0) {
        return false;
    }

    // Quick hit: if the very first element matches, return immediately.
    if (arr[0] == target) {
        return true;
    }

    // Grow "bound" as 1,2,4,... while arr[bound] < target and still in range.
    // The array is ascending so anything before bound is proven less than target,
    // so we can skip ahead exponentially to quickly bracket the target.
    int bound = 1;
    while (bound < n && arr[bound] < target) {
        bound *= 2;
    }

    // The search window is [left, right]:
    // left = previous bound (bound / 2) where values were still < target.
    // right = min(bound, n-1) so we don’t run past the array.
    int left = bound / 2;
    int right;
    if (bound < n) {
        right = bound;
    } else {
        right = n - 1;
    }

    // arr + left points to the element at index left, so binSearch
    // runs only inside [left, right] with length (right - left + 1).
    // We basically search within the left and right bounds only
    // instead of the entire array. This is useful for very large sorted arrays.
    int windowLen = right - left + 1;
    bool found = binSearch(target, arr + left, windowLen);
    return found;
}