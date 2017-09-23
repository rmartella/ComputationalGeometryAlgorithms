/*
 * Node.cpp
 *
 *  Created on: 27/02/2015
 *      Author: rey
 */

#include "Node.h"
#include "stddef.h"


Node::~Node() {
	// TODO Auto-generated destructor stub
}

Node::Node (int d){
	data=d;
	left=NULL;
	right=NULL;
	bf=0;
}

Node::Node (){
	data=0;
	left=NULL;
	right=NULL;
	bf=0;
}

