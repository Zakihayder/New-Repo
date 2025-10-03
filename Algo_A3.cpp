#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <sstream>
#include <random>
#include <limits>
#include <iomanip>

class SortingAnalyzer {
private:
    std::vector<int> data;

    // Utility function to swap two elements
    void swap(int& a, int& b) {
        int temp = a;
        a = b;
        b = temp;
    }

    // Merge function for merge sort
    void merge(std::vector<int>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        std::vector<int> L(n1), R(n2);

        for (int i = 0; i < n1; i++)
            L[i] = arr[left + i];
        for (int j = 0; j < n2; j++)
            R[j] = arr[mid + 1 + j];

        int i = 0, j = 0, k = left;

        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) {
                arr[k] = L[i];
                i++;
            }
            else {
                arr[k] = R[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            arr[k] = L[i];
            i++;
            k++;
        }

        while (j < n2) {
            arr[k] = R[j];
            j++;
            k++;
        }
    }

    // Heapify function for heap sort
    void heapify(std::vector<int>& arr, int n, int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && arr[left] > arr[largest])
            largest = left;

        if (right < n && arr[right] > arr[largest])
            largest = right;

        if (largest != i) {
            swap(arr[i], arr[largest]);
            heapify(arr, n, largest);
        }
    }

    // Partition function for quick sort
    int partition(std::vector<int>& arr, int low, int high) {
        int pivot = arr[high];
        int i = (low - 1);

        for (int j = low; j <= high - 1; j++) {
            if (arr[j] < pivot) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        return (i + 1);
    }

    // FIXED: Counting Sort for Radix Sort
    void countingSortForRadix(std::vector<int>& arr, int exp) {
        int n = arr.size();
        std::vector<int> output(n);
        int count[10] = { 0 };

        // Store count of occurrences
        for (int i = 0; i < n; i++) {
            int digit = (arr[i] / exp) % 10;
            if (digit >= 0 && digit < 10) { // Safety check
                count[digit]++;
            }
        }

        // Change count[i] so that it contains actual position
        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        // Build the output array (working backwards for stability)
        for (int i = n - 1; i >= 0; i--) {
            int digit = (arr[i] / exp) % 10;
            if (digit >= 0 && digit < 10) { // Safety check
                output[count[digit] - 1] = arr[i];
                count[digit]--;
            }
        }

        // Copy the output array to arr[]
        for (int i = 0; i < n; i++) {
            arr[i] = output[i];
        }
    }

public:
    SortingAnalyzer(const std::vector<int>& inputData) : data(inputData) {}

    // Insertion Sort
    std::vector<int> insertionSort(std::vector<int> arr) {
        int n = arr.size();
        for (int i = 1; i < n; i++) {
            int key = arr[i];
            int j = i - 1;

            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
        return arr;
    }

    // Bubble Sort
    std::vector<int> bubbleSort(std::vector<int> arr) {
        int n = arr.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
        return arr;
    }

    // Merge Sort
    void mergeSortHelper(std::vector<int>& arr, int left, int right) {
        if (left >= right) return;

        int mid = left + (right - left) / 2;
        mergeSortHelper(arr, left, mid);
        mergeSortHelper(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }

    std::vector<int> mergeSort(std::vector<int> arr) {
        if (arr.empty()) return arr;
        mergeSortHelper(arr, 0, arr.size() - 1);
        return arr;
    }

    // Heap Sort
    std::vector<int> heapSort(std::vector<int> arr) {
        if (arr.empty()) return arr;
        
        int n = arr.size();

        for (int i = n / 2 - 1; i >= 0; i--)
            heapify(arr, n, i);

        for (int i = n - 1; i > 0; i--) {
            swap(arr[0], arr[i]);
            heapify(arr, i, 0);
        }
        return arr;
    }

    // Quick Sort
    void quickSortHelper(std::vector<int>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSortHelper(arr, low, pi - 1);
            quickSortHelper(arr, pi + 1, high);
        }
    }

    std::vector<int> quickSort(std::vector<int> arr) {
        if (arr.empty()) return arr;
        quickSortHelper(arr, 0, arr.size() - 1);
        return arr;
    }

    // Counting Sort
    std::vector<int> countingSort(std::vector<int> arr) {
        if (arr.empty()) return arr;

        int max = *std::max_element(arr.begin(), arr.end());
        int min = *std::min_element(arr.begin(), arr.end());
        int range = max - min + 1;

        std::vector<int> count(range), output(arr.size());

        for (int i = 0; i < arr.size(); i++)
            count[arr[i] - min]++;

        for (int i = 1; i < count.size(); i++)
            count[i] += count[i - 1];

        for (int i = arr.size() - 1; i >= 0; i--) {
            output[count[arr[i] - min] - 1] = arr[i];
            count[arr[i] - min]--;
        }

        return output;
    }

    // FIXED: Radix Sort
    std::vector<int> radixSort(std::vector<int> arr) {
        if (arr.empty()) return arr;

        // Handle negative numbers by finding absolute maximum
        int max = *std::max_element(arr.begin(), arr.end());
        
        // If there are negative numbers, use a different approach
        int min = *std::min_element(arr.begin(), arr.end());
        
        if (min < 0) {
            // For negative numbers, we need a more sophisticated approach
            // For simplicity, we'll separate positive and negative
            std::vector<int> positive, negative;
            for (int num : arr) {
                if (num >= 0) positive.push_back(num);
                else negative.push_back(-num); // Make positive for sorting
            }
            
            // Sort negative numbers (as positive)
            if (!negative.empty()) {
                int max_neg = *std::max_element(negative.begin(), negative.end());
                for (int exp = 1; max_neg / exp > 0; exp *= 10)
                    countingSortForRadix(negative, exp);
                
                // Reverse and make negative again
                std::reverse(negative.begin(), negative.end());
                for (int& num : negative) num = -num;
            }
            
            // Sort positive numbers
            if (!positive.empty()) {
                int max_pos = *std::max_element(positive.begin(), positive.end());
                for (int exp = 1; max_pos / exp > 0; exp *= 10)
                    countingSortForRadix(positive, exp);
            }
            
            // Combine results
            arr.clear();
            arr.insert(arr.end(), negative.begin(), negative.end());
            arr.insert(arr.end(), positive.begin(), positive.end());
        }
        else {
            // Only positive numbers - use standard radix sort
            for (int exp = 1; max / exp > 0; exp *= 10)
                countingSortForRadix(arr, exp);
        }
        
        return arr;
    }

    // Bucket Sort
    std::vector<int> bucketSort(std::vector<int> arr) {
        if (arr.empty()) return arr;

        int max = *std::max_element(arr.begin(), arr.end());
        int min = *std::min_element(arr.begin(), arr.end());
        
        // Handle case where all elements are the same
        if (max == min) return arr;
        
        int bucketRange = std::max(1, (max - min) / (int)arr.size() + 1);
        int bucketCount = (max - min) / bucketRange + 1;

        std::vector<std::vector<int>> buckets(bucketCount);

        for (int i = 0; i < arr.size(); i++) {
            int index = (arr[i] - min) / bucketRange;
            // Ensure index is within bounds
            if (index >= 0 && index < bucketCount) {
                buckets[index].push_back(arr[i]);
            }
        }

        arr.clear();
        for (auto& bucket : buckets) {
            if (!bucket.empty()) {
                std::vector<int> sortedBucket = insertionSort(bucket);
                arr.insert(arr.end(), sortedBucket.begin(), sortedBucket.end());
            }
        }

        return arr;
    }

    // Measure execution time with safety checks
    double measureTime(std::vector<int>(SortingAnalyzer::* sortFunction)(std::vector<int>), std::vector<int> arr) {
        if (arr.empty()) return 0.0;
        
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<int> result = (this->*sortFunction)(arr);
        auto end = std::chrono::high_resolution_clock::now();
        
        // Verify the result is sorted
        if (!std::is_sorted(result.begin(), result.end())) {
            std::cout << "WARNING: Algorithm may not have sorted correctly!\n";
        }
        
        std::chrono::duration<double, std::milli> duration = end - start;
        return duration.count();
    }

    void analyzeAll() {
        std::vector<std::pair<std::string, std::vector<int>(SortingAnalyzer::*)(std::vector<int>)>> algorithms = {
            {"Insertion Sort", &SortingAnalyzer::insertionSort},
            {"Bubble Sort", &SortingAnalyzer::bubbleSort},
            {"Merge Sort", &SortingAnalyzer::mergeSort},
            {"Heap Sort", &SortingAnalyzer::heapSort},
            {"Quick Sort", &SortingAnalyzer::quickSort},
            {"Counting Sort", &SortingAnalyzer::countingSort},
            {"Radix Sort", &SortingAnalyzer::radixSort},
            {"Bucket Sort", &SortingAnalyzer::bucketSort}
        };

        std::cout << "Analyzing " << data.size() << " elements\n";
        if (!data.empty()) {
            std::cout << "Data range: " << *std::min_element(data.begin(), data.end()) 
                      << " to " << *std::max_element(data.begin(), data.end()) << "\n";
        }
        std::cout << "\n";

        std::cout << "================================================================================\n";
        std::cout << std::left << std::setw(18) << "Algorithm" 
                  << std::setw(30) << "Theoretical Complexity" 
                  << "Execution Time (ms)\n";
        std::cout << "================================================================================\n";

        for (const auto& algo : algorithms) {
            try {
                double timeTaken = measureTime(algo.second, data);
                
                std::string complexity;
                if (algo.first == "Insertion Sort" || algo.first == "Bubble Sort")
                    complexity = "O(n^2)";
                else if (algo.first == "Merge Sort" || algo.first == "Heap Sort")
                    complexity = "O(n log n)";
                else if (algo.first == "Quick Sort")
                    complexity = "O(n log n) avg";
                else if (algo.first == "Counting Sort")
                    complexity = "O(n + k)";
                else if (algo.first == "Radix Sort")
                    complexity = "O(d(n + k))";
                else if (algo.first == "Bucket Sort")
                    complexity = "O(n + k) avg";

                std::cout << std::left << std::setw(18) << algo.first 
                          << std::setw(30) << complexity 
                          << std::fixed << std::setprecision(4) << timeTaken << "\n";
            }
            catch (const std::exception& e) {
                std::cout << std::left << std::setw(18) << algo.first 
                          << std::setw(30) << "ERROR" 
                          << "Failed: " << e.what() << "\n";
            }
        }
        std::cout << "================================================================================\n";
    }

    void verifyAlgorithms() {
        std::vector<int> testData = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
        std::vector<int> expected = {1, 1, 2, 3, 3, 4, 5, 5, 6, 9};

        std::vector<std::pair<std::string, std::vector<int>(SortingAnalyzer::*)(std::vector<int>)>> algorithms = {
            {"Insertion Sort", &SortingAnalyzer::insertionSort},
            {"Bubble Sort", &SortingAnalyzer::bubbleSort},
            {"Merge Sort", &SortingAnalyzer::mergeSort},
            {"Heap Sort", &SortingAnalyzer::heapSort},
            {"Quick Sort", &SortingAnalyzer::quickSort},
            {"Counting Sort", &SortingAnalyzer::countingSort},
            {"Radix Sort", &SortingAnalyzer::radixSort},
            {"Bucket Sort", &SortingAnalyzer::bucketSort}
        };

        std::cout << "Verifying all algorithms...\n";
        for (const auto& algo : algorithms) {
            try {
                std::vector<int> result = (this->*algo.second)(testData);
                std::string status = (result == expected) ? "PASS" : "FAIL";
                std::cout << std::left << std::setw(15) << algo.first << " : " << status << "\n";
            }
            catch (const std::exception& e) {
                std::cout << std::left << std::setw(15) << algo.first << " : ERROR - " << e.what() << "\n";
            }
        }
        std::cout << "\n";
    }
};

// Utility functions
std::vector<int> loadDataFromCSV(const std::string& filename) {
    std::vector<int> data;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cout << "Error: Could not open file " << filename << "\n";
        return data;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        
        while (std::getline(ss, cell, ',')) {
            try {
                // Remove any whitespace
                cell.erase(remove_if(cell.begin(), cell.end(), isspace), cell.end());
                if (!cell.empty()) {
                    data.push_back(std::stoi(cell));
                }
            }
            catch (const std::exception& e) {
                // Ignore non-integer values
            }
        }
    }
    
    file.close();
    return data;
}

std::vector<int> createSampleData(int size) {
    std::vector<int> data;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);

    for (int i = 0; i < size; i++) {
        data.push_back(dis(gen));
    }

    // Save sample data to file
    std::ofstream file("sample_data.csv");
    for (size_t i = 0; i < data.size(); i++) {
        file << data[i];
        if (i != data.size() - 1) file << ",";
    }
    file.close();

    std::cout << "Created sample_data.csv with " << size << " random numbers\n";
    return data;
}

int main() {
    std::cout << "================================================================================\n";
    std::cout << "SORTING ALGORITHMS ANALYSIS - CS2009 ASSIGNMENT 3 (C++ Version - FIXED)\n";
    std::cout << "================================================================================\n\n";

    // Load data from CSV
    std::string filename = "data.csv";
    std::vector<int> data = loadDataFromCSV(filename);

    if (data.empty()) {
        std::cout << "No data loaded from " << filename << ". Creating sample data...\n";
        data = createSampleData(500); // Smaller sample for testing
    }
    else {
        std::cout << "Successfully loaded " << data.size() << " numbers from " << filename << "\n";
    }

    // Create analyzer and run analysis
    SortingAnalyzer analyzer(data);

    // Verify algorithms
    analyzer.verifyAlgorithms();

    // Run performance analysis
    analyzer.analyzeAll();

    std::cout << "\nAnalysis complete! Results are shown above.\n";
    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}