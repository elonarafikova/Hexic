#include "stdafx.h"
#include<algorithm>
#include <vector>
#include "Point.h"

using namespace std;

class Cluster
{
	int size = 3;

public:
	vector <Point*> points;
	Cluster(Point*, Point*, Point*);
    void rotate(int);
    void print();
	bool oneColor();
	Cluster operator = (Cluster c)
	{
		this->points = c.points;
		return *this;
	}
	Cluster();
};

