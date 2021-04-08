//---------------------------------------------------------------------------------
// Graph.cpp
// Author: Brent Barrese
// Class Definitions
//----------------------------------------------------------------------------------
// Graph Class:	A graph class that reads data from a text file and stores it in 
//				a Vertex object, an EdgeNode, or a VertexNode. It uses this data to 
//				create a directed, weighted graph using an adjacency list implementation. 
//				The class is primarily used to run Dijkstra's Algorithm but also 
//				allows for other methods:
//   --	allows reading data in from a file and putting into a graph
//   --	computes the shortest path between every pair of vertices
//   --	outputs a shortest path table that includes all possible paths and their 
//		details from a source node to a destination node
//   --	outputs individual paths from a source node to a destination node
//   --	allows for inserting a directed edge when given a source vertex, destination
//		vertex, and a weight
//   --	allows for removing an edge when given the source and destination vertices
//   --	allows for constructing a new graph with a parameter of an already existing
//		graph
//   --	allows for assigning an existing graph another existing graph
//
// Assumptions:
//   -- all pointers are initialized to nullptr unless instructed otherwise
//   -- the file that buildGraph uses is in the same directory as the source file
//   -- the default constructor creates a graph object and sets the VertexNode 
//		variables to nullptr
//   -- the VertexNodes being stored in the graph have a pointer to a Vertex object
//		with a private variable holding a string
//   -- there are not more than 100 vertices
//   -- the destructor will release all dynamic memory
//   -- the file will have the correct vertices number in it to set size to the 
//		correct integer value
//----------------------------------------------------------------------------------


#include <fstream>
#include "Graph.h"
#include <iomanip>
using namespace std;


//----------------------------------- buildGraph -----------------------------------
// Builds a graph by reading data from an ifstream
// Preconditions:  infile has been successfully opened and the file contains
//                 properly formated data (according to the program specs)
// Postconditions: One graph is read from infile and stored in the object
void Graph::buildGraph(ifstream& infile) {
	infile >> size;						// data member stores array size
	if (infile.eof())
		return;
	infile.ignore();					// throw away '\n' to go to next line

	// get descriptions of vertices
	for (int v = 1; v <= size; v++) {
		vertices[v].data = new Vertex;
		infile >> *vertices[v].data;	// use Vertex::operator>> to read descriptions
		//Added to make nullptr
		vertices[v].edgeHead = nullptr;
	}

	// fill cost edge array
	int src = 1, dest = 1, weight = 1;
	for (;;) {
		infile >> src >> dest >> weight;
		if (src == 0 || infile.eof())
			break;
		insertEdge(src, dest, weight);
	}
}


//----------------------------------  constructor  ---------------------------------
//	Default constructor the Graph class
//	Preconditions:	there is enough space in memory for Graph object to be instantiated
//	Postconditions:	all elements in vertices array have been initialized to nullptr.
//					size of graph is initialized to 0
Graph::Graph() {
	for (int i = 0; i < MAX_VERTICES; i++) {
		vertices[i].edgeHead = nullptr;
		vertices[i].data = nullptr;
	}
	size = 0;
}


//--------------------------------  copy constructor  ------------------------------
//	Copy constructor for the Graph class that takes a parameter of another Graph
//	Preconditions:	there is enough memory available for new graph to be created.
//					graph being passed in as a parameter is a Graph object that has
//					previously been instantiated.
//	Postconditions:	a new Graph object is created that is identical to graph
Graph::Graph(const Graph& graph) {
	// set EdgeNode* and VertexNode* to nullptr initially
	for (int i = 0; i < MAX_VERTICES; i++) {
		vertices[i].edgeHead = nullptr;
		vertices[i].data = nullptr;
	}
	
	//copy size
	size = graph.size;

	//call copyGraph function
	copyGraph(graph);
}


//----------------------------------  destructor  ----------------------------------
//	Destructor for Graph class
//	Preconditions:	calling Graph object has previously been instantiated. edgeHead
//					points to memory that was dynamically allocated
//	Postconditions:	all memory that was dynamically allocated in Graph object is
//					deallocated. All edgeHeads are deleted.
Graph::~Graph() {
	// deallocate all memory - anything new'd up
	clearGraph();
}


//---------------------------------  clearGraph  -----------------------------------
//	Clears the Graph object and deallocates any dynamic memory
//	Preconditions:	the edgeHead EdgeNodes point to the first element in the list. 
//					the EdgeNodes have been dynamically allocated
//	Postconditions:	the Graph object is empty and has no EdgeNodes or dynamically
//					allocated memory
void Graph::clearGraph() {
	EdgeNode* current;
	EdgeNode* temp;
	if (size > 0) {
		// Start at 1 and walk through each list, deleting edgeNodes
		for (int i = 1; i <= size; i++) {
			current = vertices[i].edgeHead;
			while (current != nullptr) {
				temp = current;
				current = current->nextEdge;
				delete temp;
			}
			// set edgehead to nullptr to avoid loop in future
			vertices[i].edgeHead = nullptr;
			// delete dynamically allocated vertex object
			delete vertices[i].data;
			vertices[i].data = nullptr;
		}
	}
}


//----------------------------------  operator =  ----------------------------------
//	Overloaded assignment operator that assigns a Graph object to a pre-existing
//	Graph object
//	Preconditions:	enough memory for *this to be created. the calling object is not
//					the same object that it is trying to assign to. clearGraph function
//					works to clear a Graph object of any data or allocated memory. 
//	Postconditions:	fromGraph is not changed.  the Graph object this is now exactly
//					the same as fromGraph. a deep copy was made
Graph& Graph::operator=(const Graph& fromGraph) {
	// check for self assignment
	if (this != &fromGraph) {
		// clear the current graph
		clearGraph();
		
		//copy private members
		size = fromGraph.size;

		// call copyGraph function
		copyGraph(fromGraph);
	}

	return *this;
}


//----------------------------------  insertEdge  ----------------------------------
//	Inserts an edge into the Graph object. The edge is weighted and directed
//	Preconditions:	the parameters must all be integer values. the weight must be
//					an integer value that is greater than 0. sourceVertex and 
//					destinationVertex must be pre-established vertices in the graph
//	Postconditions:	a directed and weighted edge is inserted into the graph from a 
//					source vertex to a destination vertex. If the edge from the source
//					vertex to the destination vertex already existed, the weight is 
//					updated and the EdgeNode is not dynamically allocated again. If 
//					the edge is inserted successfully or the edge already existed 
//					resulting in an updated weight, true is returned. Otherwise, false
//					is returned. 
bool Graph::insertEdge(int sourceVertex, int destinationVertex, int weight) {
	// Only insert weights >= 0
	if (weight < 0) {
		return false;
	}
	
	//Create the edgeNode
	EdgeNode* newEdgeNode = new EdgeNode;
	newEdgeNode->adjVertex = destinationVertex;
	newEdgeNode->weight = weight;
	newEdgeNode->nextEdge = nullptr;

	// What if edge already exists
	//Check if any nodes already in list
	if (vertices[sourceVertex].edgeHead == nullptr) {
		vertices[sourceVertex].edgeHead = newEdgeNode;
		return true;
	}
	else {
		// Add edge to list OR replace any previous edge that existed between the two vertices
		EdgeNode* current = vertices[sourceVertex].edgeHead;
		// loop until finding the edge already in list or hitting nullptr
		while (current->nextEdge != nullptr) {
			//Check if the edge exists and replace weight
			if (current->adjVertex == destinationVertex) {
				current->weight = weight;
				delete newEdgeNode;
				return true;
			}
			current = current->nextEdge;			
		}
		current->nextEdge = newEdgeNode;
	}
	return true;
}


//----------------------------------  removeEdge  ----------------------------------
//	Removes an edge from the Graph object
//	Preconditions:	the parameters being passed into the function must be integer values.
//					the vertices being removed must exist for them to be removed. edgeHead
//					must point to the beginning of the linked list.
//	Postconditions:	the requested edge from the given source vertex to the given 
//					destination vertex is removed from the graph and the edgeNode is 
//					deleted. If the edgeNode is successfully removed, true is returned.
//					otherwise, false is returned.
bool Graph::removeEdge(int sourceVertex, int destinationVertex) {
	// nothing in list
	if (vertices[sourceVertex].edgeHead == nullptr) {
		return false;
	}
	else {
		EdgeNode* current = vertices[sourceVertex].edgeHead;
		EdgeNode* previous = nullptr;
		
		// Handle case where head of list needs to be deleted
		if (vertices[sourceVertex].edgeHead->adjVertex == destinationVertex) {
			// delete and change head
			vertices[sourceVertex].edgeHead = vertices[sourceVertex].edgeHead->nextEdge;
			delete current;
			return true;
		}
		// start walking through adjacency list
		else {
			while (current != nullptr && current->adjVertex != destinationVertex) {
				previous = current;
				current = current->nextEdge;
				}
			if (current == nullptr) {
				return false;
			}
			previous->nextEdge = current->nextEdge;
			delete current;
			return true;
			}
		}
}


//-------------------------------  findShortestPath  -------------------------------
//	A method that finds the shortest path from all source vertices to all destination
//	vertices
//	Preconditions:	Table T exists in the Graph object. all vertices that the algorithm
//					is asked to run for have been added to the graph. MAX_VERTICES is
//					an accurate value of the maximum amount of vertices allowed in
//					the graph
//	Postconditions:	the algorithm for finding the shortest path is ran with every
//					requested vertex as the source vertex. the private member Table T is 
//					updated with results from the algorithm. 
void Graph::findShortestPath() {
	// Initialize all valid vertices in table (start at 1,1?) or at(0,0)
	for (int row = 0; row < MAX_VERTICES; row++) {
		for (int column = 0; column < MAX_VERTICES; column++) {
			T[row][column].dist = INT_MAX;
			T[row][column].path = 0;
			T[row][column].visited = false;
		}
	}
	// Call helper that passes source vertex and runs for all vertex in range
	for (int i = 1; i <= size; i++) {
		findShortestPathHelper(i);
	}
}


//--------------------------  findShortestPathHelper  ------------------------------
//	A helper function to findShortestPath that runs Dijkstra's Algorithm from a 
//	source vertex
//	Preconditions:	the value being passed in as a parameter to the function is an 
//					integer. the private member Table T has been initialized.
//	Postconditions:	Dijkstra's Algorithm is ran to every available destination vertex
//					from the source vertex. if a shorter path is found to the 
//					destination vertex then Table T is updated to reflect the newly
//					discovered lowest weight and the path it came from.
//
void Graph::findShortestPathHelper(int source) {
	// Set sourceVertex = 0
	T[source][source].dist = 0;
	T[source][source].path = source;

	// repeat n-1 times
	for (int i = 1; i < size; i++) {
		// let v be the unvisited vertex with minimum Dv
		int vertex = lowestWeightVertex(source);
		// what if vertex 0 returned?
		if (vertex > 0) {
			// mark v as visited
			T[source][vertex].visited = true;

			// Go to head of vertex			
			EdgeNode* current = vertices[vertex].edgeHead;

			// for each vertex w adjacent to v
			while (current != nullptr) {
				// if w is not visited and Dw > Dv +Dv->w
				if (T[source][current->adjVertex].visited == false) {
					
					if (T[source][current->adjVertex].dist > (T[source][vertex].dist + current->weight)) {

						// set Dw = Dv + dv,w
						T[source][current->adjVertex].dist = T[source][vertex].dist + current->weight;

						// set pathw = v
						T[source][current->adjVertex].path = vertex;
					}
				}
				current = current->nextEdge;
			}
		}
		else {
			return;
		}
	}
}


//----------------------------  lowestWeightVertex  --------------------------------
//	A helper method that finds the next lowest weight vertex that has not been 
//	visited yet
//	Preconditions:	the parameter passed in is an integer. the private data member 
//					of the calling Graph object, Table T, has been being kept updated
//					by the findShortestPath algorithm.
//	Postconditions: no changes made to the Graph object. the vertex that has not been
//					visited and has the lowest weight is returned to the calling
//					object. If no vertex is found, then 0 is returned.
int Graph::lowestWeightVertex(int source) const {
	int lowestWeight = INT_MAX;
	int returnVertex = 0;
	for (int i = 1; i <= size; i++) {
		if (T[source][i].visited == false && T[source][i].dist < lowestWeight) {
			lowestWeight = T[source][i].dist;
			returnVertex = i;
		}
	}
	return returnVertex;
}


//----------------------------------  displayAll  ----------------------------------
//	Displays the shortest path from all vertices in the graph to all vertices in
//	the graph
//	Preconditions:	the private data member of Graph, Table T, is accurate and up to
//					data. all vertex pointers in VertexNode point to a vertex object
//	Postconditions:	the findShortestPath function is called to update the 
//					private data member Table T. for each vertex, all data from the 
//					vertex to a destination vertex is printed in a table format. the 
//					source vertex, destination vertex, distance travelled, and path
//					travelled are printed in a table format.
void Graph::displayAll() {
	// Update graph by calling findShortestPath
	findShortestPath();

	cout << "Description" << setw(16) << "From" << setw(6) << "To" << setw(6) << "Dist" << setw(6) << "Path" << endl;
	//cout << "Description\t\t" << "From\t" << "To\t" << "Dist\t" << "Path" << endl;

	for (int i = 1; i <= size; i++) {
		cout << *vertices[i].data << endl;
		for (int j = 1; j <= size; j++) {
			if (i != j) {
				cout << setw(26) << i;
				cout << setw(6) << j;
				// Print distance stored in table array
				// Check if valid by looking for distance < INT_MAX (infinity)
				if (T[i][j].dist < INT_MAX) {
					cout << setw(6) << T[i][j].dist;
				}
				else {
					cout << setw(6) << "--";
				}

				// Recursive helper for path
				cout << setw(4);
				printPath(i, j);
				cout << endl;
			}
		}
		cout << endl;
	}
}


//----------------------------  printPath  -------------------------------------------
//	A helper method that recursively finds the path from a source vertex to a destination
//	vertex
//	Preconditions:	the parameters passed in are integer values. Table T is updated.		
//	Postconditions:	the destination is printed recursively for each source vertex, 
//					resulting in a path from a source vertex to a destination vertex.
//					graph object is not changed.
void Graph::printPath(const int& source, const int& destination) const{
	// IF path is 0, skip loop and do not print anything (there is no path)
	if (T[source][destination].path > 0) {
		if (source != destination) {
			printPath(source, T[source][destination].path);
		}
		cout << " " << destination;
	}
}


//------------------------------------  display  -----------------------------------
//	Displays a single, detailed path from a source vertex to a destination vertex
//	Preconditions:	the parameters passed in are integer values. all vertex pointers 
//					in VertexNode point to a vertex object		
//	Postconditions:	the findShortestPath function is called to update the 
//					private data member Table T. The specific data from the requested
//					source vertex to destination vertex is printed including the 
//					source vertex, destination vertex, distance travelled, exact path,
//					and the name of the vertex. 
void Graph::display(int source, int destination) {
	// Update graph by calling findShortestPath
	findShortestPath();
	
	cout << source;
	cout << setw(6) << destination;
	if (T[source][destination].dist < INT_MAX) {
		cout << setw(6) << T[source][destination].dist;
	}
	else {
		cout << setw(6) << "--";
	}

	cout << setw(6);
	printPath(source, destination);
	cout << endl;
	printLocationDescriptions(source, destination);
}


//------------------------  printLocationDescriptions  -----------------------------------
//	A helper method that recursively finds the path location descriptions from a 
//	source vertex to a destination vertex
//	Preconditions:	the parameters passed in are integer values. all vertex pointers
//					in vertexNode point to a vertex object. Table T is updated
//	Postconditions:	the destination path is travelled recursively and once done, prints
//					vertex data in recursive fashion. this results in a detailed path
//					description printed in order of travel. graph object is not changed
void Graph::printLocationDescriptions(const int& source, const int& destination) const {
	// IF path is 0, skip loop and do not print anything (there is no path)
	if (T[source][destination].path > 0) {
		if (source != destination) {
			printLocationDescriptions(source, T[source][destination].path);
		}
		cout << *vertices[destination].data << endl;
	}
}


//--------------------------------  copyGraph  -----------------------------------
//	Helper method that copy constructor and assignment operator use to copy a graph
//	Preconditions:	fromGraph is a previously instantiated Graph object. there is
//					enough memory for the copy to take place. vertices.edgeHead 
//					points to the head of the list. size should have been updated
//					to match fromGraph before the copyGraph function was called.
//	Postconditions:	a deep copy is made of the fromGraph into this Graph object.
//					this should now be identical to fromGraph. the table is updated
//					to match the table in fromGraph
void Graph::copyGraph(const Graph& fromGraph) {
	
	for (int i = 1; i <= size; i++) {
		vertices[i].data = new Vertex;
		*vertices[i].data = *fromGraph.vertices[i].data;
		vertices[i].edgeHead = nullptr;

		// EdgeNode pointer to head of graph.vertices list
		EdgeNode* fromGraphEdgeNode = fromGraph.vertices[i].edgeHead;

		// copy the edgeHead 
		if (fromGraphEdgeNode != nullptr) {
			vertices[i].edgeHead = new EdgeNode;
			vertices[i].edgeHead->adjVertex = fromGraphEdgeNode->adjVertex;
			vertices[i].edgeHead->weight = fromGraphEdgeNode->weight;
			vertices[i].edgeHead->nextEdge = nullptr;
			fromGraphEdgeNode = fromGraphEdgeNode->nextEdge;
		}

		// walk through each list and copy edgeNodes
		EdgeNode* current = vertices[i].edgeHead;
		EdgeNode* temp;

		// move fromGraphEdgeNode to the next edge
		while (fromGraphEdgeNode != nullptr) {
			temp = new EdgeNode;
			temp->adjVertex = fromGraphEdgeNode->adjVertex;
			temp->weight = fromGraphEdgeNode->weight;
			temp->nextEdge = nullptr;
			current->nextEdge = temp;
			current = current->nextEdge;
			fromGraphEdgeNode = fromGraphEdgeNode->nextEdge;
		}
	}
	// copy the table T
	for (int row = 0; row < MAX_VERTICES; row++) {
		for (int column = 0; column < MAX_VERTICES; column++) {
			T[row][column].dist = fromGraph.T[row][column].dist;
			T[row][column].path = fromGraph.T[row][column].path;
			T[row][column].visited = fromGraph.T[row][column].visited;
		}
	}	
}
