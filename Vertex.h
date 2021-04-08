//---------------------------------------------------------------------------------
// Vertex.h
// Author: Brent Barrese
// Class Declarations
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



#pragma once
#include <iostream>

using namespace std;

class Vertex {
	//-----------------------------  >>  -----------------------------------------
	//	Overloaded input operator for class Vertex
	//	Preconditions:	vertex.data is ready for a char
	//	Postconditions:	private member variable "data" is populated with the string 
	//					that was passed into the istream
	friend istream& operator >> (istream& inStream, Vertex& vertex);


	//-----------------------------  <<  -----------------------------------------
	//	Overloaded output operator for class Vertex
	//	Preconditions:	vertex object has been instantiated
	//	Postconditions:	the vertex object's private member variable "data" is sent
	//					to ostream
	friend ostream& operator << (ostream& outStream, Vertex& vertex);
public:

private:
	string data;
};

