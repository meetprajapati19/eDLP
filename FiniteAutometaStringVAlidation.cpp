#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <string>

using namespace std;

int main() {
    int numSymbols, numStates, initialState, numAccepting;
    vector<char> symbols;
    map<pair<int, char>, int> transitionTable;
    set<int> acceptingStates;

    cout << "Number of input symbols: ";
    cin >> numSymbols;

    cout << "Input symbols: ";
    for (int i = 0; i < numSymbols; i++) {
        char sym;
        cin >> sym;
        symbols.push_back(sym);
    }

    cout << "Enter number of states: ";
    cin >> numStates;

    cout << "Initial state: ";
    cin >> initialState;

    cout << "Number of accepting states: ";
    cin >> numAccepting;

    cout << "Accepting states: ";
    for (int i = 0; i < numAccepting; i++) {
        int state;
        cin >> state;
        acceptingStates.insert(state);
    }

    cout << "Transition table:\n";
    for (int state = 1; state <= numStates; state++) {
        for (char sym : symbols) {
            int nextState;
            cout << "State " << state << " to " << sym << " -> ";
            cin >> nextState;
            transitionTable[{state, sym}] = nextState;
        }
    }

    string inputString;
    cout << "Input string: ";
    cin >> inputString;

    int currentState = initialState;
    bool valid = true;

    for (char ch : inputString) {
        if (transitionTable.find({currentState, ch}) != transitionTable.end()) {
            currentState = transitionTable[{currentState, ch}];
        } else {
            valid = false;
            break;
        }
    }

    if (acceptingStates.find(currentState) != acceptingStates.end() && valid) {
        cout << "Valid string\n";
    } else {
        cout << "Invalid string\n";
    }

    return 0;
}
