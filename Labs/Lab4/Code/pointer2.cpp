#include <iostream>
using namespace std;

int main() {
        
        int *ptr;

        // cout << "ptr before assigning it a block of memory" << endl;
        // cout << "ptr = " << ptr << endl;
        // cout << "*ptr = " << *ptr << endl << endl;
        
        ptr = new int;
        cout << "ptr after assigning it a block of memory but before" << endl
                << "assigning the block of memory a value" << endl;
        cout << "ptr = " << ptr << endl;
        cout << "*ptr = " << *ptr << endl << endl;

        // ptr = new int; // new line number 18
        *ptr = 30;
        cout << "ptr after assigning it a block of memory and after" << endl
               << "assigning the block of memory a value" << endl;
        cout << "ptr = " << ptr << endl;
        cout << "*ptr = " << *ptr << endl << endl;

        delete ptr;
        ptr = nullptr;

        return(EXIT_SUCCESS);
}
    