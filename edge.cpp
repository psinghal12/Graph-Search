/* Assignment 3 was made by Pankhuri Singhal and Gabriel Oliver */
/*
Pankhuri Singhal, Gabriel Oliver
CSS 343
8/16/21
Prof. Awad
Imp file that represents a single edge of the graph, connecting between 
two vertices. Has methods to represent weight and the end vertex label.
*/

#pragma once
#ifndef EDGE_CPP
#define EDGE_CPP

#include <string>

////////////////////////////////////////////////////////////////////////////////
// This is 80 characters - Keep all lines under 80 characters                 //
////////////////////////////////////////////////////////////////////////////////

// the initializers in edge.h mean that we don't need to do anything in the
// default constr
Edge::Edge() {
  this->endVertex = "";
  this->edgeWeight = 0; //just in case
}

/** constructor with label and weight */
Edge::Edge(const std::string& end, int weight) {
  this->endVertex = end;
  this->edgeWeight = weight;
}

/** return the vertex this edge connects to */
std::string Edge::getEndVertex() const { 
  return this->endVertex; 
}

/** return the weight/cost of travlleing via this edge */
int Edge::getWeight() const { 
  return this->edgeWeight; 
}

#endif
