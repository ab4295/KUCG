#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "particle.h"

#define SIZEN 10

using namespace std;
using namespace glm;

class Hashtable
{
public:
	vector<int> hashGrid[SIZEN][SIZEN][SIZEN];

	void HashTable(vector<particle> p);
	vector<int> GetNeighbor(double x, double y, double z, float radius);
};

