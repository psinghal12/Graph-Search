/* Assignment 3 was made by Pankhuri Singhal and Gabriel Oliver */

/*
Pankhuri Singhal, Gabriel Oliver
Prof. Awad
Implementation that represents a graph read from a file, with methods to conduct 
DFS, BFS, and Dijkstra's algorithm to find an optimal path between 
two vertices. Also checks if vertices and edges are compatible, finds 
vertices and gets the weight of the edge between the start and end. 
Also has a destructor to free up memory.
*/

#include <queue>
#include <climits>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>

using namespace std;
/**
 * A graph is made up of vertices and edges
 * A vertex can be connected to other vertices via weighted, directed edge
 */

/** constructor, empty graph */
Graph::Graph() {
	vertices = new map<std::string, Vertex*>();
	numberOfEdges = 0;
	numberOfVertices = 0;
}

/** destructor, delete all vertices and edges
    only vertices stored in map
    no pointers to edges created by graph */
Graph::~Graph() {
	clearEverything();
}

/** return number of vertices */
int Graph::getNumVertices() const { return numberOfVertices; }

/** return number of edges */
int Graph::getNumEdges() const { return numberOfEdges; }

/** add a new edge between start and end vertex
    if the vertices do not exist, create them
    calls Vertex::connect
    a vertex cannot connect to itself
    or have multiple edges to another vertex */
bool Graph::add(const std::string& start, const std::string& end, int edgeWeight) {
	bool compatible = verticesEdgePairCompatible(start, end);
	if (!compatible) {
		return false;
	}
	findOrCreateVertex(start);
	findOrCreateVertex(end);
	findVertex(start)->connect(end, edgeWeight);
	numberOfVertices = static_cast<int>(vertices->size());
	numberOfEdges++;
	return true;
}

/** return weight of the edge between start and end
    returns INT_MAX if not connected or vertices don't exist */
int Graph::getEdgeWeight(const std::string& start, const std::string& end) const {
	Vertex* startVert = findVertex(start);
	Vertex* endVert = findVertex(end);
	if (startVert == nullptr) {
		cout << start << "does not exist" << endl;
		return INT_MAX;
	}
	if (endVert == nullptr) {
		cout << end << "does not exist" << endl;
		return INT_MAX;
	}
	if (start == end) {
		cout << start << " is the same as " << end << "." << endl;
		return INT_MAX;
	}
	int result = INT_MAX;
	result = startVert->getEdgeWeight(end);
	if (result < 0) {
		cout << "There is no connection starting from " << start
		<< " and ending at " << end << "." << endl;
		return INT_MAX;
	}
	return result;
}

/** read edges from file
    the first line of the file is an integer, indicating number of edges
    each edge line is in the form of "string string int"
    fromVertex  toVertex    edgeWeight */
void Graph::readFile(const std::string& filename) {
	clearEverything();
	vertices = new map<std::string, Vertex*>();
	std::ifstream graphFile;
	graphFile.open(filename);
	if (!(graphFile.is_open())) {
		cout << "File " << filename << " cannot be opened." << endl;
		return;
	}
	string numOfEdgesStr;
	getline(graphFile, numOfEdgesStr);
	int tempNumOfEdges = std::stoi(numOfEdgesStr);

	for (int i = 0; i < tempNumOfEdges; i++) {
		std::string connection;
		getline(graphFile, connection);
		std::istringstream str(connection);
		std::string firstVertStr;
		std::string endVertStr;
		std::string weightStr;
		str >> firstVertStr >> endVertStr >> weightStr;
		add(firstVertStr, endVertStr, std::stoi(weightStr));
	}
	graphFile.close();
}

/** depth-first traversal starting from startLabel
    call the function visit on each vertex label */
void Graph::depthFirstTraversal(const std::string& startLabel,
                                void visit(const std::string&)) {
	if (vertices->empty()) {
		cout << "Graph is empty." << endl;
		return;
	}
	unvisitVertices();
	resetAllNeighbors();
	Vertex* startingVertex = findVertex(startLabel);
	depthFirstTraversalHelper(startingVertex, visit);
	unvisitVertices();
}

/** helper for depthFirstTraversal */
void Graph::depthFirstTraversalHelper(Vertex* startVertex,
									  void visit(const std::string&)) {
	string startVertexLabel = startVertex->getLabel();
	visit(startVertexLabel);
	startVertex->visit();
	startVertex->resetNeighbor();
	for (Vertex* nextNeighbor = findVertex(startVertex->getCurrentNeighbor());
	nextNeighbor->getLabel() != startVertexLabel;
	nextNeighbor = findVertex(startVertex->getNextNeighbor())) {
		if (!nextNeighbor->isVisited()) {
			depthFirstTraversalHelper(nextNeighbor, visit);
		}
	}
}

/** breadth-first traversal starting from startLabel
    call the function visit on each vertex label */
void Graph::breadthFirstTraversal(const std::string& startLabel,
                                  void visit(const std::string&)) {
	if (vertices->empty()) {
		cout << "Graph is empty." << endl;
		return;
	}
	unvisitVertices();
	resetAllNeighbors();
	queue<Vertex*> bfsQueue;
	Vertex* startingVertex = findVertex(startLabel);
	startingVertex->resetNeighbor();
	bfsQueue.push(startingVertex);
	while (!bfsQueue.empty()) {
		Vertex* currentVert = bfsQueue.front();
		if (!currentVert->isVisited()) {
			visit(currentVert->getLabel());
			currentVert->visit();
		}
		for (Vertex* nextNeighbor =
				findVertex(currentVert->getCurrentNeighbor());
			 nextNeighbor != currentVert;
			 nextNeighbor = findVertex(currentVert->getNextNeighbor())) {
			if (!nextNeighbor->isVisited()) {
				visit(nextNeighbor->getLabel());
				nextNeighbor->visit();
				bfsQueue.push(nextNeighbor);
			}
		}
		bfsQueue.pop();
	}
	unvisitVertices();
}

/** find the lowest cost from startLabel to all vertices that can be reached
    using Djikstra's shortest-path algorithm
    record costs in the given map weight
    weight["F"] = 10 indicates the cost to get to "F" is 10
    record the shortest path to each vertex using given map previous
    previous["F"] = "C" indicates get to "F" via "C"

    cpplint gives warning to use pointer instead of a non-const map
    which I am ignoring for readability */
void Graph::djikstraCostToAllVertices(
    const std::string& startLabel,
    std::map<std::string, int>& weight,
    std::map<std::string, std::string>& previous) {
	if (vertices->empty()){
		cout << "Graph is Empty" << endl;
		return;
	}
	weight.clear();
	previous.clear();
	unvisitVertices();
	resetAllNeighbors();
	initializeDjikstraMaps(startLabel, weight, previous);
	Vertex* startVertex = findVertex(startLabel);
	startVertex->visit();
	djikstraHelper(startLabel, weight, previous, 0);
}

// Parameters: string startLabel, map<string, int>& weight, map<string,
// 			   string>& previous
// Post: sets the default values for both maps.
void Graph::initializeDjikstraMaps(const std::string& startLabel,
								   std::map<std::string, int>& weight,
								   std::map<std::string, std::string>& previous) {
	for (pair<string, Vertex*> currPair : *vertices) {
		if (currPair.first != startLabel) {
			weight.insert(pair<string, int>(currPair.first, INT_MAX));
			previous.insert(pair<string, string>(currPair.first, ""));
		}
	}
}

// Parameters: string startLabel, map<string, int>& weight, map<string,
// 			   string>& previous, int currWeight
// Post: Finds the shortest to each vertex in the maps
void Graph::djikstraHelper(const std::string& currLabel,
						   std::map<std::string, int>& weight,
						   std::map<std::string, std::string>& previous,
						   int currWeight) {
	Vertex* currVert = findVertex(currLabel);

	for (Vertex* nextNeighbor = findVertex(currVert->getNextNeighbor());
	nextNeighbor->getLabel() != currLabel;
	nextNeighbor = findVertex(currVert->getNextNeighbor())) {
		if (!nextNeighbor->isVisited()) {
			nextNeighbor->visit();
			weight.find(nextNeighbor->getLabel())->second = currWeight +
					currVert->getEdgeWeight(nextNeighbor->getLabel());
			previous.find(nextNeighbor->getLabel())->second = currLabel;
		}
		else {
			nextNeighbor->resetNeighbor();
			if (currWeight + currVert->getEdgeWeight(nextNeighbor->getLabel())
			< weight.find(nextNeighbor->getLabel())->second) {
				weight.find(nextNeighbor->getLabel())->second = currWeight +
						currVert->getEdgeWeight(nextNeighbor->getLabel());
				previous.find(nextNeighbor->getLabel())->second = currLabel;
			}
			else {
				if (currVert->getCurrentNeighbor() == currLabel) {
					return;
				} else {
					continue;
				}
			}
		}
		djikstraHelper(nextNeighbor->getLabel(), weight, previous,
					   currWeight + currVert->getEdgeWeight(
					   		nextNeighbor->getLabel()));
	}
}

/** mark all verticies as unvisited */
void Graph::unvisitVertices() {
	for (pair<string, Vertex*> pair : *vertices) {
		pair.second->unvisit();
	}
}

/** find a vertex, if it does not exist return nullptr */
Vertex* Graph::findVertex(const std::string& vertexLabel) const {
	if (vertices->empty()) {
		return nullptr;
	}
	_Rb_tree_iterator<pair<const string, Vertex *>> result =
			vertices->find(vertexLabel);
	if (result == vertices->end()) {
		return nullptr;
	}
	return result->second;
}

/** find a vertex, if it does not exist create it and return it */
Vertex* Graph::findOrCreateVertex(const std::string& vertexLabel) {
	Vertex* result = findVertex(vertexLabel);
	if (result == nullptr) {
		result = new Vertex(vertexLabel);
		vertices->insert(pair<string, Vertex*>(vertexLabel, result));
	}
	return result;
}

// Parameters: string start, string end
// Post: Determines whether the two vertices can be used in an operation,
// 		 such as connecting them together.
// Returns: bool true if both strings are not equal to each other, or
// 			there isn't an edge that starts at start and ends in end.
//			Returns bool false otherwise.
bool Graph::verticesEdgePairCompatible(const std::string& start,
									   const std::string& end) const {
	if (start == end) {
		cout << "The start and end vertices cannot be the same." << endl;
		return false;
	}
	if (vertices->empty()) {
		return true;
	}
	Vertex* startVert = findVertex(start);
	Vertex* endVert = findVertex(end);
	if (startVert == nullptr || endVert == nullptr) {
		return true;
	}
	for (Vertex* nextNeighbor = startVert;
		 nextNeighbor->getLabel() != start;
		 nextNeighbor = findVertex(nextNeighbor->getNextNeighbor())) {
		if (nextNeighbor == endVert) {
			cout << "An edge from " << start << " to " << end
					<< " already exists." << endl;
			return false;
		}
	}
	return true;
}

// Post: Resets the adjacencyList to all vertices
void Graph::resetAllNeighbors() {
	for (pair<string, Vertex*> currPair : *vertices) {
		currPair.second->resetNeighbor();
	}
}

// Post: Frees all the allocated memory in this instance of the graph class
void Graph::clearEverything() {
	for (pair<string, Vertex*> currPair : *vertices) {
		delete currPair.second;
		currPair.first = "";
	}
	vertices->clear();
	delete vertices;
	vertices = nullptr;
	numberOfVertices = 0;
	numberOfEdges = 0;
}
