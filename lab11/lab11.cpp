/***********************************************************************
* Program:
*    Lab 11, name
*    Brother Wilson, CS470
* Author:
*    Ty Brajic
* Summary: 
*    This program checks the strength of passwords entered
************************************************************************/
#include <string>
#include <iostream>      
using namespace std;

int main() {
   
   string p = "";
   int combinations = 0;
   int bits = 0;
   cout << "Please enter the password: ";
   cin >> p;
   cout << "There are " << combinations << " combinations" << endl;
   cout << "That is equivalent to a key of " << bits << " bits" << endl;
 
}