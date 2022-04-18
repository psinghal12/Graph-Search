/* Assignment 3 was made by Pankhuri Singhal and Gabriel Oliver */

/*
Pankhuri Singhal, Gabriel Oliver
CSS 343
Header that represents a single edge of the graph, connecting between 
two vertices. Has methods to represent weight and the end vertex label.
*/

/**
 * Used by vertex to keep track of all the vertices connects to
 * Each edge has a weight, possibly 0
 * Edge is a simple container class, no interesting functions
 */
#pragma once
#ifndef EDGE_H
#define EDGE_H

#include <string>

class Edge {
 public:
    /** empty edge constructor */
    Edge();

    /** constructor with label and weight */
    Edge(const std::string& end, int weight);

    /** return the vertex this edge connects to */
    std::string getEndVertex() const;

    /** return the weight/cost of travlleing via this edge */
    int getWeight() const;

 private:
    /** end vertex, cannot be changed */
    std::string endVertex {""};

    /** edge weight, cannot be changed */
    int edgeWeight {0};
};  //  end Edge

#include "edge.cpp"

#endif  // EDGE_H
