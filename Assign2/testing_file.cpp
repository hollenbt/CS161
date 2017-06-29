#include <iostream>
#include <cfloat>
#include <climits>
using namespace std;

int get_user_input(string prompt) {
    int x = -1;
    while (1) {
        cout << prompt;
        cin >> x;
        if (cin.fail()) {
            cin.clear();
            x = -1;
        }
        cin.ignore(32767, '\n');
        if (!x || (x == 1))
            return x;
    }
}

int main() {

//cout << "Lab " + string('0' + 1) + string(" score: ");
//cout << ((get_user_input("Enter 0 or 1: ") % 2)?"right":"left");
//cout << DBL_MAX << endl << INT_MAX << endl;
cout << '\'';

return 0;
}
