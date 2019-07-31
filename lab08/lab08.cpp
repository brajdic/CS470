/***********************************************************************
* Program:
*    Lab 08, Vulnerabilities and Exploits
*    Brother Wilson, CS470
* Author:
*    Ty Brajdic
* Summary: 
*    This program is designed to demonstrate memory injection vulnerabilities.
*
************************************************************************/

#include <iostream>
#include <cassert>
#include <string>
#include <cstring>
using namespace std;

void stackExploit();
void heapExploit();
void arrayExploit();
void intExploit();
void arcExploit();
void subterfugeExploit();
void vtableExploit();
void ansiExploit();

/******************************************************
 * MAIN
 * This function take you to the various exploits
 *****************************************************/
int main()
{
   // prompt
   cout << "  1. Stack smashing\n"
        << "  2. Heap spraying\n"
        << "  3. Array index\n"
        << "  4. Integer overflow\n"
        << "  5. ARC injection\n"
        << "  6. Pointer subterfuge\n"
        << "  7. VTable spraying\n"
        << "  8. ANSI-Unicode conversion\n"
        << "> ";
   int select;
   cin >> select;

   switch (select)
   {
      case 1:
         stackExploit();
         break;
      case 2:
         heapExploit();
         break;
      case 3:
         arrayExploit();
         break;
      case 4:
         intExploit();
         break;
      case 5:
         arcExploit();
         break;
      case 6:
         subterfugeExploit();
         break;
      case 7:
         vtableExploit();
         break;
      case 8:
         ansiExploit();
         break;
      default:
         cout << "Unknown option " << select << endl;
   }

   return 0;
}

/**************************************************************
 **************************************************************
 *******************    SUBTERFUGE    *************************
 **************************************************************
 **************************************************************/

/****************************************
 * Pointer Subterfuge Vulnerability
 * 1. There must be a pointer used in the code.
 * 2. There must be a way to overwrite the pointer
 * 3. After the pointer is dereferenced, the pointer is dereferenced
 ****************************************/
void subterfugeVulnerability(long * array, int size)
{
   long buffer[2] = {0, 1};
   const char * message = "All is safe; nothing bad happened\n";  // 1. Pointer

   for (int i = 0; i < size; i++)  // 2. Overwrite
      buffer[i] = array[i];

   cout << message;                // 3. Dereference
   
}

/****************************************
 * Pointer Subterfuge Exploitation
 * 1. The attacker must exploit a vulnerability allowing
 *    unintended access to the pointer
 * 2. The attacker must be able to provide a new pointer
 *    referring to data altering the normal flow of the program
 ****************************************/
void subterfugeExploit()
{
   // an attacker's array
   long array[3] =             // 1. Exploit
   {
      0,
      1,
      (long)"!!!!!!!!!!!!! You've been hacked !!!!!!!!!!!!!\n"
      // 2. Provide pointer
   };

   // exploit it
   subterfugeVulnerability(array, 3);   
}

/**************************************************************
 **************************************************************
 ********************    STACK       **************************
 **************************************************************
 **************************************************************/
 
//function to point to for stackExploit() and arcExploit()
void unintended(){cout << "!!!!!!!!!!!!! You've been hacked !!!!!!!!!!!!!\n";}

/***********************************************
 * STACK VULNERABILITY
 * 1. There must be a buffer on the stack
 * 2. the buffer must be reachable from an external input
 * 3. The mechanism to fill the buffer must not check the correct buffersize
 **********************************************/
void stackVulnerability(/* feel free to add parameters */long * input, int size)
{
	long i = 0;
	long buffer[2];               //buffer on the stack
	for (i = 0; i < size; i++)   //wrong buffer size
		buffer[i] = input[i];	//buffer reachable
}

/*********************************************
 * STACK EXPLOIT
 * 1. The attacker must provide more data into the
 *    outwardly facing buffer than the buffer is designed to hold
 * 2. The attacker must know where the the stack pointer resides
 *    on the stack. This should be just beyond the end of the buffer
 * 3. The attacker must insert machine language instructions in the buffer.
 *    This may occur before, after, or even around the stack pointer. The
 *    machine language could be already compiled code in the program
 * 4. The attacker must overwrite the stack pointer. The old value,
 *    directing the flow of the program after the function is returned,
 *    must be changed from the calling function to the provided
 *    machine language in step 3.
 *********************************************/
void stackExploit()
{
	long buffer[6] = { 0, 1, 2, 3, 4, (long)&unintended }; 	 //function pointer to unintended
	stackVulnerability(buffer, 8); 							//overwrite stack pointer
	cout << "This will not print.\n"; 					   //Proof of concept (POC)
}


/**************************************************************
 **************************************************************
 ********************      HEAP      **************************
 **************************************************************
 **************************************************************/
 
/*************************************
 * HEAP VULNERABILITY
 * 1. There must be two adjacent heap buffers.
 * 2. The first buffer must be reachable through external input.
 * 3. The mechanism to fill the buffer from the external input must
 *    not correctly check for the buffer size.
 * 4. The second buffer must be released before the first.
 *************************************/
void heapVulnerability(/* feel free to add parameters */long * input, int size)
{
	long *buffer1 = new long[2]; 
	long *buffer2 = new long[2];
	for (int i = 0; i < size; i++) 	  //external input/no buffer check
		buffer1[i] = input[i];       //fill buffer
	delete [] buffer2; 				//released first
	delete [] buffer1;
}

/*************************************
 * HEAP EXPLOIT
 * 1. The attacker must provide more data into the outwardly facing
 *    heap buffer than the buffer is designed to hold
 * 2. The attacker must know the layout of the Memory Control Block (MCB)
 *    (essentially a linked list) residing just after the buffer
 * 3. The attacker must provide a new MCB containing both the location
 *    of the memory overwrite and the new data to be overwritten
 ***********************************/
void heapExploit()
{
	long buffer[4] = {0, 1, 2, 3}; 									//MCB
	heapVulnerability(buffer, sizeof(buffer) / sizeof(buffer[0])); //pass a greater buffer size than intended (4)
	cout << "This will not print.\n"; 							  //POC
}

/**************************************************************
 **************************************************************
 ********************     ARRAY      **************************
 **************************************************************
 **************************************************************/

/*************************************
 * ARRAY VULNERABILTY
 * 1. There must be an array and an array index variable
 * 2. The array index variable must be reachable through external input.
 * 3. There must not be bounds checking on the array index variable.
 ************************************/
void arrayVulnerability(/* feel free to add parameters */int index, int input)  
{
	int array[4] = {0, 1, 2, 3};
	array[index] = input; //no bounds check
}

/**************************************
 * ARRAY EXPLOIT
 * 1. The attacker provides an array index value outside the expected range
 * 2. The attacker must be able to provide input or redirect
 *    existing input into the array at the index he provided
 * 3. The injected value must alter program state in a way
 *    that is desirable to the attacker
 *************************************/
void arrayExploit()
{
	int authenticated = false;
	arrayVulnerability(11, true); 									  //local variable we are replacing true with in memory
	if(authenticated)												 //this should be false
		cout << "!!!!!!!!!!!!! You've been hacked !!!!!!!!!!!!!\n"; //should not work but does because of vuln
	else														   		
		cout << "This will not print.\n";						  //POC
}


/**************************************************************
 **************************************************************
 *******************    INTEGER      **************************
 **************************************************************
 **************************************************************/

/********************************************
 * INTEGER VULNERABILITY
 * 1. There must be a security check represented by an expression.
 * 2. The expression must have the potential for overflow.
 * 3. At least one of the numbers used to compute the sentinel
 *    must be reachable through external input.
 *********************************************/
void intVulnerability(/* feel free to add parameters */const char * input)
{
	char buffer[64];
	bool authenticated = false;
	strcpy(buffer, input);                                           //the attacker can send a larger input than expected resulting in 'if(authenticated)' being skipped
	if(authenticated)
		cout << "!!!!!!!!!!!!! You've been hacked !!!!!!!!!!!!!\n"; //this should not work but does because of vuln
	else
		cout << "This will not print.\n";                          //this should work but does not because of vuln
}

/**********************************************
 * INTEGER EXPLOIT
 * 1. Provide input, either a buffer size or a single value,
 *    that is directly or indirectly used in the vulnerable expression
 * 2. The input must exceed the valid bounds of the data-type,
 *    resulting in an overflow or underflow condition
 **********************************************/
void intExploit()
{
	char text[] = "Now this is the story all about how My life got flipped, turned upside down And I'd like to take a minute just sit right there I'll tell you how I became the prince of a town called Bel-air";
	intVulnerability(text); //this will send the above text which is larger than the char can hold in intVulnerability()
}

/**************************************************************
 **************************************************************
 ********************      ARC       **************************
 **************************************************************
 **************************************************************/
 
//function for ARC vulnerability 
void intended(){cout << "This should always be called.\n";}

/*********************************
 * ARC VULNERABILITY
 * 1. There must be a function pointer used in the code.
 * 2. Through some vulnerability, there must be a way for user input
 *    to overwrite the function pointer. This typically happens through
 *    a stack buffer vulnerability.
 * 3. After the memory is overwritten, the function pointer must
 *    be dereferenced
 ********************************/
void arcVulnerability(/* feel free to add parameters */long * harmlessArray, int size)
{
	int buffer[4];
	void (* funtionPointer)() = intended; //function pointer vuln
	for(int i = 0; i < size; i++)
		buffer[i] = harmlessArray[i]; 
	funtionPointer();                    //dereference the function pointer. This should call the safe function, but doesn't because of the vuln.
}

/*********************************
 * ARC EXPLOIT
 * 1. The attacker must exploit a vulnerability allowing
 *    unintended access to the function pointer
 * 2. The attacker must have the address to another function
 *    which is to be used to replace the existing function pointer
  ********************************/
void arcExploit()
{
	long payload[5] = {0, 1, 2, 3, (long)&unintended}; //this fuction is all the way back in the stack vulnerability. I decided to reuse my code.
	arcVulnerability(payload, 5);                     //use the payload to access the funtionPointer in arcVulnerability()
}

/**************************************************************
 **************************************************************
 ********************     VTABLE     **************************
 **************************************************************
 **************************************************************/

/***********************************
 * VULNERABILITY
 * 1. The vulnerable class must be polymorphic.
 * 2. The class must have a buffer as a member variable.
 * 3. Through some vulnerability, there must be a way for user input
 *    to overwrite parts of the VTable.
 * 4. After a virtual function pointer is overwritten,
 *    the virtual function must be called.
 **********************************/
class Vulnerability
{
};

/************************************
 * VTABLE EXPLOIT
 * 1. Through some vulnerability, the VTable pointer
 *     or a function pointer within the VTable must be overwritten
 * 2. The attacker must have the address to another VTable pointer
 *     or a function pointer
 ***********************************/
void vtableExploit()
{
}


/**************************************************************
 **************************************************************
 ******************    ANSI-Unicode    ************************
 **************************************************************
 **************************************************************/

/*********************************************************
 * ANSI - UNICODE VULNERABILITY
 * 1. There must be a buffer where the basetype is greater than one.
 * 2. Validation of the buffer must check the size of the buffer
 *    rather than the number of elements in the buffer.
 ********************************************************/
void ansiVulnerability(/* feel free to add parameters */)
{
}

/***********************************************
 * ASCI - UNICODE EXPLOIT
 * 1. The attacker must provide more than half as much data
 *    into the outwardly facing buffer as it is designed to hold
 **********************************************/
void ansiExploit()
{
}
