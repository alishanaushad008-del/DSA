#include <iostream>

using namespace std;

class DataStream {
private:
    int targetValue;
    int targetK;
    int count;

public:
    DataStream(int value, int k) {
        targetValue = value;
        targetK = k;
        count = 0;
    }

    bool consec(int num) {
        if (num == targetValue) {
            count++;
        } else {
            count = 0;
        }
        return count >= targetK;
    }
};

int main() {
    DataStream dataStream(4, 3);

    cout << boolalpha << dataStream.consec(4) << endl;
    cout << boolalpha << dataStream.consec(4) << endl;
    cout << boolalpha << dataStream.consec(4) << endl;
    cout << boolalpha << dataStream.consec(3) << endl;

    return 0;
}