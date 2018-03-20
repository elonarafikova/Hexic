#include "stdafx.h"
#include "Point.h"
#include "Cluster.h"

class Table
{
	vector <Point> points;
	int score = 0;
	public:
		vector <Cluster> allClusters;
		Table();
		void formClusters();
		bool doBestRotation();
		void deleteAndFill(vector<Cluster>);
		void draw();
		Point* findPoint(int, int);
		int calculateScore(vector<Cluster>);
		int checkOneColor();
		void noOneColor();
};

