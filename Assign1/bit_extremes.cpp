/******************************************************
** Program: bit_extremes.cpp
** Author: Thomas Hollenberg
** Date: 1/10/2017
** Description: Calculates and outputs the maximum signed and unsigned
**     and minimum signed values that can be stored in numBits bits. numBits
**     must be a positive integer less than or equal to 64.
** Input: 8
** Output:
**    Enter the number of bits (must be a positive integer less than or equal to 64): 8
**
**    For a(n) 8 bit variable,
**            the maximum unsigned number is 255
**            the maximum signed number is 127
**            the minimum signed number is -128
******************************************************/

#include <iostream>

using namespace std;

int main() {

    int numBits = 65;
    unsigned long long twoToTheNumBits, oneLessPowerOfTwo, maxUnsigned;
    long long maxSigned, minSigned;
    twoToTheNumBits = 1;

    // Get numBits from user.
    cout << "Enter the number of bits (must be a positive integer less than or equal to 64): ";
    while (1) {
        cin >> numBits;
        if ((numBits > 0) && (numBits <= 64))
            break;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(32767, '\n');
        }
        cout << "Please enter a positive integer less than or equal to 64: ";
    }

    // A temp variable is used because the value of numBits must be preserved for output.
    int temp = numBits;

    // Calculate 2^numBits and 2^(numBits - 1), which are stored in
    // twoToTheNumBits and oneLessPowerOfTwo, respectively.
    while (temp--) {
        if (!temp)
            oneLessPowerOfTwo = twoToTheNumBits;
        twoToTheNumBits *= 2;
    }

    // Calculate the maximum and minimum values.
    maxUnsigned = twoToTheNumBits - 1;
    maxSigned = oneLessPowerOfTwo - 1;
    minSigned = -oneLessPowerOfTwo;

    // Output the calculation results.
    cout << "\nFor a(n) " << numBits << " bit variable," << endl;
    cout << "\tthe maximum unsigned number is " << maxUnsigned << endl;
    cout << "\tthe maximum signed number is " << maxSigned << endl;
    cout << "\tthe minimum signed number is " << minSigned << endl;

    return 0;
}
