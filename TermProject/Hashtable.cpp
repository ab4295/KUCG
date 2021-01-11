#include "Hashtable.h"

void Hashtable::HashTable(vector<particle> p)
{
	for (int x = 0; x < SIZEN; x++) {
		for (int y = 0; y < SIZEN; y++) {
			for (int z = 0; z < SIZEN; z++) {
				hashGrid[x][y][z].clear();
			}
		}
	}

	for (int i = 0; i < p.size(); i++) {
		int x = (p[i].position.x + 50) / SIZEN;
		int y = (p[i].position.y) / SIZEN;
		int z = (p[i].position.z + 50) / SIZEN;

		if (x < 0) x = 0;
		if (x > SIZEN - 1) x = SIZEN - 1;
		if (y < 0) y = 0;
		if (y > SIZEN - 1) y = SIZEN - 1;
		if (z < 0) z = 0;
		if (z > SIZEN - 1) z = SIZEN - 1;
		hashGrid[x][y][z].push_back(i);
		// printf("%d %d %d\n", x, y, z);
	}
}

vector<int> Hashtable::GetNeighbor(double x, double y, double z, float radius)
{
	x = (x + 50) / SIZEN;
	y = (y) / SIZEN;
	z = (z + 50) / SIZEN;
	vector<int> res;

	for (int x0 = x - radius; x0 <= x + radius; x0++) {
		for (int y0 = y - radius; y0 <= y + radius; y0++) {
			for (int z0 = z - radius; z0 <= z + radius; z0++) {
				if (x0 < 0 || x0 > SIZEN - 1 ||
					y0 < 0 || y0 > SIZEN - 1 ||
					z0 < 0 || z0 > SIZEN - 1) continue;
				
				for (int i = 0; i < hashGrid[x0][y0][z0].size(); i++) {
					int j = hashGrid[x0][y0][z0][i];
					res.push_back(j);
				}
			}
		}
	}

	return res;
}