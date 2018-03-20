#include "stdafx.h"
#include "Cluster.h"
#include <iostream>
using namespace std;

Cluster::Cluster()
{
}

Cluster::Cluster(Point* first, Point* second, Point* third)
{
	points.push_back(first);
	points.push_back(second);
	points.push_back(third);
}

void Cluster::rotate(int shift)
{
	vector <int> n;
	for (int i = 0; i < size; i++) {
		n.push_back(points[i]->color);
	}
	for (int i = 0; i < size; i++) {
		points[(i + shift) % size]->color = n[i];
	}
}
void Cluster::print()
{
	for (int i = 0; i < size; i++){
		cout << points[i]->color << ' ';
	}
	cout << endl;
}

bool Cluster::oneColor()
{
	return ((points[0]->color == points[1]->color) && (points[1]->color == points[2]->color));
}