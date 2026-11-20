#include <iostream>
#include <stack>
using namespace std;

int partitionArray(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortIterative(int arr[], int n) {
    stack<int> st;
    st.push(0);
    st.push(n - 1);

    while (!st.empty()) {
        int high = st.top();
        st.pop();
        int low = st.top();
        st.pop();

        int p = partitionArray(arr, low, high);

        if (p - 1 > low) {
            st.push(low);
            st.push(p - 1);
        }
        if (p + 1 < high) {
            st.push(p + 1);
            st.push(high);
        }
    }
}

int main() {
    int arr[] = {10, 7, 8, 9, 1, 5, 3};
    int n = sizeof(arr) / sizeof(arr[0]);

    cout << "Before sorting: ";
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << endl;

    quickSortIterative(arr, n);

    cout << "After sorting: ";
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << endl;

    return 0;
}