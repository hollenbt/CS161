/******************************************************
** Program: extrema_using_climits.cpp
** Author: Thomas Hollenberg
** Date: 1/10/2017
** Description: Prints signed and unsigned integer variable maximums and minimums using the climits library macros.
** Input: N/A
** Output:
**   unsigned short int max: 65535
**   signed short int max: 32767
**   signed short int min: -32768
**
**   unsigned int max: 4294967295
**   signed int max: 2147483647
**   signed int min: -2147483648
**
**   unsigned long int max: 18446744073709551615
**   signed long int max: 9223372036854775807
**   signed long int min: -9223372036854775808
******************************************************/

#include <iostream>
#include <climits>

using namespace std;

int main()  {

    cout << "unsigned short int max: " << USHRT_MAX << endl;
    cout << "signed short int max: " << SHRT_MAX << endl;
    cout << "signed short int min: " << SHRT_MIN << endl << endl;
    cout << "unsigned int max: " << UINT_MAX << endl;
    cout << "signed int max: " << INT_MAX << endl;
    cout << "signed int min: " << INT_MIN << endl << endl;
    cout << "unsigned long int max: " << ULONG_MAX << endl;
    cout << "signed long int max: " << LONG_MAX << endl;
    cout << "signed long int min: " << LONG_MIN << endl << endl;

    return 0;
}
