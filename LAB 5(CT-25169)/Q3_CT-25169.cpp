#include <iostream>
#include <queue>
#include <vector>
#include <string>

using namespace std;

class Call {
public:
    int id;
    int arrivalTime;
    string customerName;
    int duration;

    Call(int i, int arrival, string name, int dur) {
        id = i;
        arrivalTime = arrival;
        customerName = name;
        duration = dur;
    }
};

class CallCenter {
private:
    queue<Call> callQueue;
    int numCSRs;
    vector<bool> csrAvailable;
    vector<int> csrEndTime;
    vector<int> csrCallId;

    bool anyBusy() {
        for (int i = 0; i < numCSRs; i++) {
            if (!csrAvailable[i]) return true;
        }
        return false;
    }

public:
    CallCenter(int csrs) {
        numCSRs = csrs;
        csrAvailable.assign(csrs, true);
        csrEndTime.assign(csrs, 0);
        csrCallId.assign(csrs, -1);
    }

    void addCall(Call call) {
        callQueue.push(call);
        cout << "Call " << call.id << " (" << call.customerName << ") added to queue." << endl;
    }

    void processCalls() {
        int time = 0;
        while (!callQueue.empty() || anyBusy()) {
            for (int i = 0; i < numCSRs; i++) {
                if (!csrAvailable[i] && csrEndTime[i] == time) {
                    csrAvailable[i] = true;
                    cout << "[Minute " << time << "] CSR " << (i + 1)
                         << " completed Call " << csrCallId[i] << endl;
                    csrCallId[i] = -1;
                }
            }

            for (int i = 0; i < numCSRs; i++) {
                if (csrAvailable[i] && !callQueue.empty() && callQueue.front().arrivalTime <= time) {
                    Call c = callQueue.front();
                    callQueue.pop();
                    csrAvailable[i] = false;
                    csrEndTime[i] = time + c.duration;
                    csrCallId[i] = c.id;
                    cout << "[Minute " << time << "] CSR " << (i + 1)
                         << " picked up Call " << c.id << " (" << c.customerName << ")" << endl;
                }
            }

            time++;
        }
    }
};

int main() {
    CallCenter center(2);

    center.addCall(Call(101, 0, "Alice", 5));
    center.addCall(Call(102, 1, "Bob", 3));
    center.addCall(Call(103, 2, "Charlie", 4));
    center.addCall(Call(104, 3, "David", 2));

    center.processCalls();

    return 0;
}