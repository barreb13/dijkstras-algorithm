//--------------------------------------------------------------------------------------
// Graph.h
// Author: Brent Barrese
// Class Declarations
//--------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------


#include "Vertex.h"

class Graph {
public:
	//----------------------------------- buildGraph ---------------------------------------
	// Builds a graph by reading data from an ifstream
	// Preconditions:  infile has been successfully opened and the file contains
	//                 properly formated data (according to the program specs)
	// Postconditions: One graph is read from infile and stored in the object			
	void buildGraph(ifstream& infile);

	
	//----------------------------------  constructor  -------------------------------------
	//	Default constructor the Graph class
	//	Preconditions:	there is enough space in memory for Graph object to be instantiated
	//	Postconditions:	all elements in vertices array have been initialized to nullptr.
	//					size of graph is initialized to 0
	Graph();

	
	//--------------------------------  copy constructor  ----------------------------------
	//	Copy constructor for the Graph class that takes a parameter of another Graph
	//	Preconditions:	there is enough memory available for new graph to be created.
	//					graph being passed in as a parameter is a Graph object that has
	//					previously been instantiated.
	//	Postconditions:	a new Graph object is created that is identical to graph
	Graph(const Graph& graph);

	
	//----------------------------------  destructor  --------------------------------------
	//	Destructor for Graph class
	//	Preconditions:	calling Graph object has previously been instantiated. edgeHead
	//					points to memory that was dynamically allocated
	//	Postconditions:	all memory that was dynamically allocated in Graph object is
	//					deallocated. All edgeHeads are deleted.
	virtual ~Graph();

	
	//----------------------------------  operator =  --------------------------------------
	//	Overloaded assignment operator that assigns a Graph object to a pre-existing
	//	Graph object
	//	Preconditions:	enough memory for *this to be created. the calling object is not
	//					the same object that it is trying to assign to. clearGraph function
	//					works to clear a Graph object of any data or allocated memory. 
	//	Postconditions:	fromGraph is not changed.  the Graph object this is now exactly
	//					the same as fromGraph. a deep copy was made
	Graph& operator=(const Graph& fromGraph);

	
	//----------------------------------  insertEdge  --------------------------------------
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
	bool insertEdge(int sourceVertex, int destinationVertex, int weight);

	
	//----------------------------------  removeEdge  --------------------------------------
	//	Removes an edge from the Graph object
	//	Preconditions:	the parameters being passed into the function must be integer values.
	//					the vertices being removed must exist for them to be removed. edgeHead
	//					must point to the beginning of the linked list.
	//	Postconditions:	the requested edge from the given source vertex to the given 
	//					destination vertex is removed from the graph and the edgeNode is 
	//					deleted. If the edgeNode is successfully removed, true is returned.
	//					otherwise, false is returned.
	bool removeEdge(int sourceVertex, int destinationVertex);

	
	//-------------------------------  findShortestPath  -----------------------------------
	//	A method that finds the shortest path from all source vertices to all destination
	//	vertices
	//	Preconditions:	Table T exists in the Graph object. all vertices that the algorithm
	//					is asked to run for have been added to the graph. MAX_VERTICES is
	//					an accurate value of the maximum amount of vertices allowed in
	//					the graph
	//	Postconditions:	the algorithm for finding the shortest path is ran with every
	//					requested vertex as the source vertex. the private member Table T is 
	//					updated with results from the algorithm. 
	void findShortestPath();

	
	//----------------------------------  displayAll  --------------------------------------
	//	Displays the shortest path from all vertices in the graph to all vertices in
	//	the graph
	//	Preconditions:	the private data member of Graph, Table T, is accurate and up to
	//					data. all vertex pointers in VertexNode point to a vertex object
	//	Postconditions:	the findShortestPath function is called to update the 
	//					private data member Table T. for each vertex, all data from the 
	//					vertex to a destination vertex is printed in a table format. the 
	//					source vertex, destination vertex, distance travelled, and path
	//					travelled are printed in a table format.
	void displayAll();

	
	//------------------------------------  display  ---------------------------------------
	//	Displays a single, detailed path from a source vertex to a destination vertex
	//	Preconditions:	the parameters passed in are integer values. all vertex pointers 
	//					in VertexNode point to a vertex object		
	//	Postconditions:	the findShortestPath function is called to update the 
	//					private data member Table T. The specific data from the requested
	//					source vertex to destination vertex is printed including the 
	//					source vertex, destination vertex, distance travelled, exact path,
	//					and the name of the vertex. 
	void display(int source, int destination);


private:
	static const int MAX_VERTICES = 101;

	struct EdgeNode {
		int adjVertex;			// subscript of the adjacent vertex 
		int weight;				// weight of edge
		EdgeNode* nextEdge;
	};

	struct VertexNode {
		EdgeNode* edgeHead;		// head of the list of edges
		Vertex* data;			// store vertex data here
	};

	// array of VertexNodes
	VertexNode vertices[MAX_VERTICES];

	// table of information for Dijkstra's algorithm
	struct Table {
		bool visited;			// whether vertex has been visited
		int dist;				// shortest known distance from source
		int path;				// previous vertex in path of min dist
	};

	int size;					// number of vertices in the graph

	// Table stores visited, distance, path - two dimensional in order to solve for all sources
	Table T[MAX_VERTICES][MAX_VERTICES];

	
	//--------------------------  findShortestPathHelper  ----------------------------------
	//	A helper function to findShortestPath that runs Dijkstra's Algorithm from a 
	//	source vertex
	//	Preconditions:	the value being passed in as a parameter to the function is an 
	//					integer. the private member Table T has been initialized.
	//	Postconditions:	Dijkstra's Algorithm is ran to every available destination vertex
	//					from the source vertex. if a shorter path is found to the 
	//					destination vertex then Table T is updated to reflect the newly
	//					discovered lowest weight and the path it came from.
	//
	void findShortestPathHelper(int source);

	
	//----------------------------  lowestWeightVertex  ------------------------------------
	//	A helper method that finds the next lowest weight vertex that has not been 
	//	visited yet
	//	Preconditions:	the parameter passed in is an integer. the private data member 
	//					of the calling Graph object, Table T, has been being kept updated
	//					by the findShortestPath algorithm.
	//	Postconditions: no changes made to the Graph object. the vertex that has not been
	//					visited and has the lowest weight is returned to the calling
	//					object. If no vertex is found, then 0 is returned.
	int lowestWeightVertex(int source) const;

	
	//------------------------------  clearGraph  ------------------------------------------
	//	Clears the Graph object and deallocates any dynamic memory
	//	Preconditions:	the edgeHead EdgeNodes point to the first element in the list. 
	//					the EdgeNodes have been dynamically allocated
	//	Postconditions:	the Graph object is empty and has no EdgeNodes or dynamically
	//					allocated memory
	void clearGraph();

	
	//--------------------------------  printPath  ----------------------------------------
	//	A helper method that recursively finds the path from a source vertex to a destination
	//	vertex
	//	Preconditions:	the parameters passed in are integer values. Table T is updated.		
	//	Postconditions:	the destination is printed recursively for each source vertex, 
	//					resulting in a path from a source vertex to a destination vertex.
	//					graph object is not changed.
	void printPath(const int& source,const int& destination) const;

	
	//------------------------  printLocationDescriptions  --------------------------------
	//	A helper method that recursively finds the path location descriptions from a 
	//	source vertex to a destination vertex
	//	Preconditions:	the parameters passed in are integer values. all vertex pointers
	//					in vertexNode point to a vertex object. Table T is updated
	//	Postconditions:	the destination path is travelled recursively and once done, prints
	//					vertex data in recursive fashion. this results in a detailed path
	//					description printed in order of travel. graph object is not changed
	void printLocationDescriptions(const int& source, const int& destination) const;


	//--------------------------------  copyGraph  ----------------------------------------
	//	Helper method that copy constructor and assignment operator use to copy a graph
	//	Preconditions:	fromGraph is a previously instantiated Graph object. there is
	//					enough memory for the copy to take place. vertices.edgeHead 
	//					points to the head of the list. size should have been updated
	//					to match fromGraph before the copyGraph function was called.
	//	Postconditions:	a deep copy is made of the fromGraph into this Graph object.
	//					this should now be identical to fromGraph. the table is updated
	//					to match the table in fromGraph
	void copyGraph(const Graph& fromGraph);
};
