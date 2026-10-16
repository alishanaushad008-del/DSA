#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

class Sort {
public:
    string bestTechnique;
    string worstTechnique;

    void bubbleSort(vector<int>& arr, long long& comp, long long& swaps) {
        int n = arr.size();
        comp = 0;
        swaps = 0;
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                comp++;
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                    swaps++;
                }
            }
        }
    }

    void selectionSort(vector<int>& arr, long long& comp, long long& swaps) {
        int n = arr.size();
        comp = 0;
        swaps = 0;
        for (int i = 0; i < n - 1; i++) {
            int minIdx = i;
            for (int j = i + 1; j < n; j++) {
                comp++;
                if (arr[j] < arr[minIdx]) {
                    minIdx = j;
                }
            }
            if (minIdx != i) {
                swap(arr[i], arr[minIdx]);
                swaps++;
            }
        }
    }

    void insertionSort(vector<int>& arr, long long& comp, long long& swaps) {
        int n = arr.size();
        comp = 0;
        swaps = 0;
        for (int i = 1; i < n; i++) {
            int key = arr[i];
            int j = i - 1;
            while (j >= 0) {
                comp++;
                if (arr[j] > key) {
                    arr[j + 1] = arr[j];
                    swaps++;
                    j--;
                } else {
                    break;
                }
            }
            arr[j + 1] = key;
        }
    }

    void sortArray(vector<int>& arr, string choice) {
        long long comp = 0, swaps = 0;
        if (choice == "bubble" || choice == "Bubble") {
            bubbleSort(arr, comp, swaps);
        } else if (choice == "selection" || choice == "Selection") {
            selectionSort(arr, comp, swaps);
        } else if (choice == "insertion" || choice == "Insertion") {
            insertionSort(arr, comp, swaps);
        } else {
            cout << "Invalid choice. Use bubble, selection or insertion." << endl;
            return;
        }
        cout << "Comparisons: " << comp << ", Swaps: " << swaps << endl;
    }

    void analyze(int size) {
        vector<int> original(size);
        for (int i = 0; i < size; i++) {
            original[i] = rand() % 1000;
        }

        vector<int> bArr = original;
        vector<int> sArr = original;
        vector<int> iArr = original;

        long long bComp, bSwaps;
        long long sComp, sSwaps;
        long long iComp, iSwaps;

        bubbleSort(bArr, bComp, bSwaps);
        selectionSort(sArr, sComp, sSwaps);
        insertionSort(iArr, iComp, iSwaps);

        cout << "Bubble Sort    - Comparisons: " << bComp << ", Swaps: " << bSwaps << endl;
        cout << "Selection Sort - Comparisons: " << sComp << ", Swaps: " << sSwaps << endl;
        cout << "Insertion Sort - Comparisons: " << iComp << ", Swaps: " << iSwaps << endl;

        long long bOps = bComp + bSwaps;
        long long sOps = sComp + sSwaps;
        long long iOps = iComp + iSwaps;

        bestTechnique = "Bubble Sort";
        worstTechnique = "Bubble Sort";
        long long minOps = bOps, maxOps = bOps;

        if (sOps < minOps) { minOps = sOps; bestTechnique = "Selection Sort"; }
        if (iOps < minOps) { minOps = iOps; bestTechnique = "Insertion Sort"; }

        if (sOps > maxOps) { maxOps = sOps; worstTechnique = "Selection Sort"; }
        if (iOps > maxOps) { maxOps = iOps; worstTechnique = "Insertion Sort"; }
    }
};

int main() {
    srand(time(0));
    Sort sorter;

    int n;
    if (!(cin >> n)) return 0;

    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    string choice;
    cin >> choice;

    sorter.sortArray(arr, choice);

    for (int x : arr) {
        cout << x << " ";
    }
    cout << endl << endl;

    sorter.analyze(100);

    cout << endl;
    cout << "*** BEST TECHNIQUE  : " << sorter.bestTechnique << " ***" << endl;
    cout << "*** WORST TECHNIQUE : " << sorter.worstTechnique << " ***" << endl;

    return 0;
}