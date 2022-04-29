#include <iostream>
using namespace std;

int main() {
    int number;
    int *ptr;

    cout << "value of ptr and *ptr before initializing ptr" << endl;
    cout << "ptr = " << ptr << endl;
    cout << "*ptr = " << *ptr << endl << endl;

    number = 10;
    ptr = &number;
    cout << "number = " << number << endl;
    cout << "address of number = " << &number << endl;
    cout << "ptr = " << ptr << endl;
    cout << "*ptr = " << *ptr << endl;

    // Note that no memory was dynamically allocated

    return(EXIT_SUCCESS);
}