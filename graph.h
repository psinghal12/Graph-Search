/* Assignment 3 was made by Pankhuri Singhal and Gabriel Oliver */

/**
 * A graph is made up of vertices and edges
 * A vertex can be connected to other vertices via weighted, directed edge
 */

/*
Pankhuri Singhal, Gabriel Oliver
CSS 343
Header file that represents a graph read from a file, with methods to conduct 
DFS, BFS, and Dijkstra's algorithm to find an optimal path between 
two vertices. Also checks if vertices and edges are compatible, finds 
vertices and gets the weight of the edge between the start and end. 
Also has a destructor to free up memory.
*/
#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <string>
#include "vertex.h"
#include "edge.h"

class Graph {
 public:
    /** constructor, empty graph */
    Graph();

    /** destructor, delete all vertices and edges
        only vertices stored in map
        no pointers to edges created by graph */
    ~Graph();

    /** return number of vertices */
    int getNumVertices() const;

    /** return number of vertices */
    int getNumEdges() const;

    /** add a new edge between start and end vertex
        if the vertices do not exist, create them
        calls Vertex::connect
        a vertex cannot connect to itself
        or have multiple edges to another vertex */
    bool add(const std::string& start, const std::string& end, int edgeWeight = 0);

    /** return weight of the edge between start and end
        returns INT_MAX if not connected or vertices don't exist */
    int getEdgeWeight(const std::string& start, const std::string& end) const;

    /** read edges from file
        the first line of the file is an integer, indicating number of edges
        each edge line is in the form of "string string int"
        fromVertex  toVertex    edgeWeight */
    void readFile(const std::string& filename);

    /** depth-first traversal starting from startLabel
        call the function visit on each vertex label */
    void depthFirstTraversal(const std::string& startLabel,
                             void visit(const std::string&));

    /** breadth-first traversal starting from startLabel
        call the function visit on each vertex label */
    void breadthFirstTraversal(const std::string& startLabel,
                               void visit(const std::string&));

    /** find the lowest cost from startLabel to all vertices that can be reached
        using Djikstra's shortest-path algorithm
        record costs in the given map weight
        weight["F"] = 10 indicates the cost to get to "F" is 10
        record the shortest path to each vertex using given map previous
        previous["F"] = "C" indicates get to "F" via "C"

        cpplint gives warning to use pointer instead of a non-const map
        which I am ignoring for readability */
    void djikstraCostToAllVertices(
        const std::string& startLabel,
        std::map<std::string, int>& weight,
        std::map<std::string, std::string>& previous);

 private:
    /** number of vertices in graph */
    int numberOfVertices;

    /** number of edges in graph */
    int numberOfEdges;

    /** mapping from vertex label to vertex pointer for quick access */
    std::map<std::string, Vertex*>* vertices;

    /** helper for depthFirstTraversal */
    void depthFirstTraversalHelper(Vertex* startVertex,
                                   void visit(const std::string&));

    // Parameters: string startLabel, map<string, int>& weight, map<string,
    // 			   string>& previous
    // Post: sets the default values for both maps.
    void initializeDjikstraMaps(const std::string& startLabel,
								std::map<std::string, int>& weight,
								std::map<std::string, std::string>& previous);

    // Parameters: string startLabel, map<string, int>& weight, map<string,
    // 			   string>& previous, int currWeight
    // Post: Finds the shortest to each vertex in the maps
    void djikstraHelper(const std::string& currLabel,
						std::map<std::string, int>& weight,
						std::map<std::string, std::string>& previous,
						int currWeight);

    /** mark all verticies as unvisited */
    void unvisitVertices();

    /** find a vertex, if it does not exist return nullptr */
    Vertex* findVertex(const std::string& vertexLabel) const;

    /** find a vertex, if it does not exist create it and return it */
    Vertex* findOrCreateVertex(const std::string& vertexLabel);

    // Parameters: string start, string end
    // Post: Determines whether the two vertices can be used in an operation,
    // 		 such as connecting them together.
    // Returns: bool true if both strings are not equal to each other, or
    // 			there isn't an edge that starts at start and ends in end.
    //			Returns bool false otherwise.
    bool verticesEdgePairCompatible(const std::string& start, const std::string& end) const;

    // Post: Resets the adjacencyList to all vertices
	void resetAllNeighbors();

	// Post: Frees all the allocated memory in this instance of the graph class
	void clearEverything();

};  // end Graph

#include "graph.cpp"
#endif  // GRAPH_H
