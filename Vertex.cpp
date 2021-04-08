//----------------------------------------------------------------------------------------
// Vertex.cpp
// Author: Brent Barrese
// Class Definitions
//---------------------------------------------------------------------------------
// Vertex Class:	Takes in a string from istream and stores in a Vertex object
//
//   --	allows encapsulating of a string
//   --	allows istream insertion of a string 
//   -- allows printing of Vertex object member variable
//
// Assumptions:
//   -- a string will come in via istream
//   -- uses default constructor and destructor provided by compiler
//	 -- the string has a 50 chars max length
//---------------------------------------------------------------------------------



#include "Vertex.h"
#include <iostream>
#include <string>


//-----------------------------  >>  -----------------------------------------
//	Overloaded input operator for class Vertex
//	Preconditions:	vertex.data is ready for a char
//	Postconditions:	private member variable "data" is populated with the string 
//					that was passed into the istream
istream& operator>>(istream& inStream, Vertex& vertex) {
	getline(inStream, vertex.data);	
	return inStream;
}


//-----------------------------  <<  -----------------------------------------
//	Overloaded output operator for class Vertex
//	Preconditions:	vertex object has been instantiated
//	Postconditions:	the vertex object's private member variable "data" is sent
//					to ostream
ostream& operator<<(ostream& outStream, Vertex& vertex) {
	outStream << vertex.data;
	return outStream;
}
