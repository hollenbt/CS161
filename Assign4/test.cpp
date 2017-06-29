#include <iostream>
#include <sys/time.h>
#include <cstdlib>

using namespace std;

int main() {
    /*typedef struct timeval time;
    time stop, start;
    gettimeofday(&start, NULL);
    for (int i = 0; i < 1000; ++i)
        cout << i;
    gettimeofday(&stop, NULL);
    if(stop.tv_sec > start.tv_sec)
        cout << "\nSeconds: " << stop.tv_sec-start.tv_sec << endl;
    else
        cout << "\nMicro: " << stop.tv_usec-start.tv_usec << endl;*/

    char enter_char[256];
    cout << "Enter";
    cin.getline(enter_char, 256);
    cout << "Good job" << endl;

    return 0;
}
