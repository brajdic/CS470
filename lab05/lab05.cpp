/***********************************************************************
* Program:
* Lab 05, Homographs
* Brother Wilson, CS 470
* Author:
* Ty Brajdic
* Summary:
* This program checks to see whether the input are homographs or not.
*************************************************************************/
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

/***********************************************************************
* Get paths to compare from the user.
***********************************************************************/
string* getPaths() 
{
	string* input = new string[256];
	cout << "Specify the first filename:  ";
	cin >> input[0];
	cout << "Specify the second filename: ";
	cin >> input[1];
	return input;
}
/***********************************************************************
* Check if the usr input is a homograph by finding characters within 
* the two strings.
***********************************************************************/
bool isHomograph()
{
	string* path = getPaths();
	string found = "./";
	if(path[0] != path[1])
	{
		if(path[0].find(found) != string::npos || path[1].find(found) != string::npos)
			return true;
		else
			return false;
	}
	else
		return true;
}
/***********************************************************************
* Display information in the console.
***********************************************************************/
void display(bool isHomograph)
{
	if(!isHomograph)
		cout << "The paths are NOT homographs\n";
	else
		cout << "The paths are homographs\n";
}

int main()
{
	display(isHomograph());
}