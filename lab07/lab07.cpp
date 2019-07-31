/***********************************************************************
* Program:
*    Lab 07, Stack Probe
*    Brother Wilson, CS470
* Author:
*    Ty Brajdic
* Summary: 
*    This program is designed to illustrate weaknesses in the stack.
*
************************************************************************/

#include <stdlib.h>    // for malloc()
#include <iostream>
#include <iomanip>
#include <cassert>
using namespace std;


void partOne(int); // Find the address of the stack, heap, and code segments
void partTwo(char *, long);    // Manipulate the stack to find a weakness
void partThree();  // Demonstrate a variety of vulnerabilities

/******************************************************
 * MAIN
 * This function will only take you to one of the three parts
 * of Lab 07
 *****************************************************/
int main()
{
   char text[] = "%%main%%";
   long number = 1234567;
   &number;
   
   // prompt
   cout << "Please select an option:\n";
   cout << "  1.  Find the address\n";
   cout << "  2.  Display the contents of the stack\n";
   cout << "  3.  Manipulate the stack\n";
   cout << "> ";

   int selection;
   cin >> selection;

   switch (selection)
   {
      case 1:
         partOne(1);
         break;
      case 2:
         partTwo(text, number + 1111111);
         break;
      case 3:
         partThree();
         break;
      default:
         cout << "Unknown option\n";
         return 1;
   }

   return 0;
}

/**************************************************************
 **************************************************************
 **************************** PART 1 **************************
 **************************************************************
 **************************************************************/


/**********************************************
 * Part 1
 *
 * Find the address of the stack, heap, and code.
 * Note that it will be in a different location with
 * every execution
 **********************************************/
void partOne(int byValueParameter)
{
   void *pStack = NULL;
   void *pHeap  = NULL;
   void *pCode  = NULL;

   ////////////////////////////////////////////////
   // put your magic code between here...
	pStack = &pStack;
	pHeap = malloc(sizeof(int));
	pCode = (void*) &partOne;
   // ... and here
   ////////////////////////////////////////////////

   // display the results
   cout << "Stack: " << pStack << endl;
   cout << "Heap:  " << pHeap  << endl;
   cout << "Code:  " << pCode  << endl;

   return;
}

/**************************************************************
 **************************************************************
 **************************** PART 2 **************************
 **************************************************************
 **************************************************************/

/************************************************
 * CONVERT TO STRING
 * Convert the data from p into a human-readable string
 * by removing all the unprintable characters and replacing
 * them with a dot
 ***********************************************/
string displayCharArray(const char * p)
{
   string output;
   for (int i = 0; i < 8; i++)
      output += string(" ") + (p[i] >= ' ' && p[i] <= 'z' ? p[i] : '.');
   return output;
}

/*************************************************
 * PART 2
 *
 * Display the contents of the stack. This will be from
 * slot -4 to slot 20.
 *************************************************/
void partTwo(char *text, long number)
{
   // start your display of the stack from this point
   long bow = number + 1111111;

   // header for our table. Use these setw() offsets in your table
   cout << "[" << setw(2) << 'i' << ']'
        << setw(15) << "address"
        << setw(20) << "hexadecimal"
        << setw(20) << "decimal"
        << setw(18) << "characters"
        << endl;
   cout << "----+"
        << "---------------+"
        << "-------------------+"
        << "-------------------+"
        << "-----------------+\n";
   for (int i = 20; i >= -4; i--) //this was written incorrectly as ">="!
   {
	////////////////////////////////////////////////
	// put your magic code between here...
      cout << "[" << setw(2) << i << ']'
		   << setw(15) << (&bow + i) //display numbers
		   << setw(20) << hex << (long)*(&bow + i)// uses the standard library for hexadecimal and casts bow varibles address plus i into long pointer
           << setw(20) << dec << (long)*(&bow + i)// uses the standard library for decimal and casts bow varibles address plus i into long pointer
           << setw(18) << displayCharArray(reinterpret_cast<char*>(&bow + i)) // uses reinterpret_cast to cast the long bow variable address plus i to the char* pointer
		   << endl;
	// ... and here
   ////////////////////////////////////////////////
   }
}

/**************************************************************
 **************************************************************
 **************************** PART 3 **************************
 **************************************************************
 **************************************************************/

void fail(char grade);
void pass(char grade);

/**********************************************************************
 * All your code needs to exist in this function below the comment
 *********************************************************************/
void readStack()
{
   int   number;
   char  text[16];
   
   ////////////////////////////////////////////////
   // put your magic code between here...
   int findMe = 0; //time to find offsets...
   /*for (int i = 0; i <= 40; i++)
   {
	   
      cout << "[" << setw(2) << i << "]" // offset is 21
      << setw(20) << dec << (long)*(&findMe + i) // offset is 25
      << setw(18) << displayCharArray(reinterpret_cast<char*>(&findMe + i)) // offset is 29
	  << endl;
	  
   }**/
   number = * (&findMe + 17); // get number
   for (int i = 0; i < 16; i++) 
   {
      text[i] = * (((char*)(&findMe + 13)) + i); // change text
   }
   * (int*)(&findMe + 19) = (size_t) &pass; // change fail to pass
   * (((char *)(&findMe + 18) + 3)) = 'A'; // change grade from 'B' to 'A'
   // ... and here
   ////////////////////////////////////////////////

   // output the goods
   cout << "\tnumber: "
        << number
        << endl;
   cout << "\ttext:   "
        << text
        << endl;

   return;
}

/*******************************************************************
 * Part 3
 *
 * This function contains the data that you are trying to manipulate.
 * Please don't modify partTwo() in any way
 ********************************************************************/
void partThree()
{
   int   number;   // first local variable that you need to read
   char  text[16]; // second

   char  letterGrade;
   void (*pointerFunction)(char);

   // get the values
   cout << "number: ";
   cin  >> number;
   cout << "text:   ";
   cin  >> text;
   
   // Call the function and display the results.
   pointerFunction = fail;
   letterGrade = 'B';
   readStack();            // vulnerability is here 
   (*pointerFunction)(letterGrade);

   return;
}


/**********************************************************************
 * The failure case. Don't touch this function
 *********************************************************************/
void fail(char grade)
{
   cout << "\tI am sorry, but you got a 'F'\n";
}

/**********************************************************************
 * I think you would really rather call this function.  This
 * is read-only also
 *********************************************************************/
void pass(char grade)
{
   cout << "\tGood job, you got a(n) '" << grade << "'\n";
}


