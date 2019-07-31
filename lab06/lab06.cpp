/***********************************************************************
* Program:
* Lab 06, Sanitization
* Brother Wilson, CS 470
* Author:
* Ty Brajdic
* Summary:
* This program checks user input for HTML tags against a whitelist, only
* accepts valid tags, and escapes tags that are not.
*************************************************************************/
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;
void escapeTags(bool hasTag, bool invalidTag, string &input, string &target);

/***********************************************************************
* Get input from user
***********************************************************************/
bool getInput(string &input)
{
	bool continueLoop = true;
	cout << "> ";
	getline(cin, input);
	if (input == "quit")
		continueLoop = false;
	return continueLoop;
}

/***********************************************************************
* Display information in the console.
***********************************************************************/
void display(string input, bool quit)
{
	if(quit)
		cout << "\tquit\n";
	else
		cout << '\t' + input + '\n';
}

/***********************************************************************
* Filter the user input for valid HTML tags
***********************************************************************/
void validate(string input)
{
	string whitelist[256];
	whitelist[0] = "<a";
	whitelist[1] = "<abbr>";
	whitelist[2] = "<b>";
	whitelist[3] = "<blockquote>";
	whitelist[4] = "<cite>";
	whitelist[5] = "<code>";
	whitelist[6] = "<del>";
	whitelist[7] = "<em>";
	whitelist[8] = "<i>";
	whitelist[9] = "<q>";
	whitelist[10] = "<strike>";
	whitelist[11] = "<strong>";

	//get the user input as long as the user doesn't choose to "quit"
	while (getInput(input))
	{
		bool hasTag = false;
		bool invalidTag = true;
		string target = "<";
		for (int i = 0; i < 12; i++)
		{
			//check the user input if a tag matches the target
			if (input.find(target) != string::npos)
				hasTag = true;
			//check the user input if the target tag is on the whitelist
			if (input.find(whitelist[i]) != string::npos)
				invalidTag = false;
		}
		//now that we know our input isn't on our whitelist, let's escape it
		escapeTags(hasTag, invalidTag, input, target);
	}
	//tell display to quit
	display(input, true);
}

/***********************************************************************
* Escpase the HTML tags that are not on the whitelist
***********************************************************************/
void escapeTags(bool hasTag, bool invalidTag, string &input, string &target)
{
	if (hasTag == true && invalidTag == true)
	{
		for (int i = 0; i < 12; i++)
			if (input.find(target) != string::npos)
				input.replace(input.find(target), 1, "&lt;");
		//temporarily set our target to '>'
		target = ">";
		for (int i = 0; i < 12; i++)
			if (input.find(target) != string::npos)
				input.replace(input.find(target), 1, "&gt;");
		display(input, false);
	}
	else
		display(input, false);
}

/***********************************************************************
* Entry point
***********************************************************************/
int main()
{
	string input = "";
	validate(input);
}