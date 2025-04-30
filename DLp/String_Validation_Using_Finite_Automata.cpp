#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    int inp_sta;
    cout << "Enter the number of input symbols: ";
    cin >> inp_sta;

    vector<char> symbols(inp_sta);
    cout << "Enter the input symbols: ";
    for (int i = 0; i < inp_sta; i++) {
        cin >> symbols[i];
    }
    int tot_state;
    cout << "Enter the number of states: ";
    cin >> tot_state;

    int intstate;
    cout << "Enter the initial state: ";
    cin >> intstate;

    int totalAccState;
    cout << "Enter the number of accepting states: ";
    cin >> totalAccState;

    vector<int> accsates(totalAccState);
    cout << "Enter the accepting states: ";
    for (int i = 0; i < totalAccState; i++) {
        cin >> accsates[i];
    }

   
    vector<vector<int>> trans(tot_state + 1, vector<int>(inp_sta));
    cout << "Enter the transition table \n";
    for (int state = 1; state <= tot_state; state++) {
        for (int sym = 0; sym < inp_sta; sym++) {
            cout << "State " << state << " to symbol '" << symbols[sym] << "': ";
            cin >> trans[state][sym];
        }
    }
    string input;
    cout << "Enter an input string: ";
    cin >> input;

    int curstate = intstate;
    for (char c : input) {
        int symbolIndex = -1;
        for (int i = 0; i < inp_sta; i++) {
            if (c == symbols[i]) {
                symbolIndex = i;
                break;
            }
        }

        if (symbolIndex == -1) {
            cout << "The string is not valid (invalid symbol encountered).\n";
            return 0;
        }
        curstate = trans[curstate][symbolIndex];
    }

    bool isaccp = false;
    for (int i = 0; i < totalAccState; i++) {
        if (curstate == accsates[i]) {
            isaccp = true;
            break;
        }
    }

    if (isaccp) {
        cout << "string is accept\n";
    } else {
        cout << "string is not accept";
    }

    return 0;
}
