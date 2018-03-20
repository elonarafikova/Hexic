#include "stdafx.h"
#include "Table.h"
#include <iostream>
#include <random>
#include <unordered_set>


const int colors = 9; //0 1 2 3 4 5 6 7 8 9
const int Row = 17;
const int Column = 5;



class Rotation
{
public:
	Cluster cluster;
	int turn;
	Rotation()
	{
	}
	Rotation(Cluster c, int turn)
	{
		this->cluster = c;
		this->turn = turn;
	}

};

Table::Table()
{
	random_device random;
	for (int i = 0; i < Row; i++) {
		for (int j = 0; j < Column; j++) {
			points.push_back(Point(i, j, random() % 10));
		}
	}
	this->formClusters();
	cout << "Game Table";
	this->draw();
	this->noOneColor();
	this->draw();
	while (this->doBestRotation()){
	}
}

void Table::formClusters()
{
	for (int i = 0; i < Row - 2; i++) {
		if (i % 2 == 0) {
			for (int j = 0; j < Column - 1; j++) {
				allClusters.push_back(Cluster(&points[i*Column + j], &points[(i + 1)*Column + j], &points[(i + 2)*Column + j]));
				allClusters.push_back(Cluster(&points[i*Column + j], &points[(i + 1)*Column + j + 1], &points[(i + 2)*Column + j]));
			}
			allClusters.push_back(Cluster(&points[i*Column + Column - 1], &points[(i + 1)*Column + Column - 1], &points[(i + 2)*Column + Column - 1]));
		}
		else {
			allClusters.push_back(Cluster(&points[i*Column + 0], &points[(i + 1)*Column + 0], &points[(i + 2)*Column + 0]));
			for (int j = 1; j < Column; j++) {
				allClusters.push_back(Cluster(&points[i*Column + j], &points[(i + 1)*Column + j - 1], &points[(i + 2)*Column + j]));
				allClusters.push_back(Cluster(&points[i*Column + j], &points[(i + 1)*Column + j], &points[(i + 2)*Column + j]));
			}
		}
	}
}

int Table::checkOneColor()
{
	int result = 0;
	vector <Cluster> one;
	for (auto k = allClusters.begin(); k < allClusters.end(); k++)
		if (k->oneColor()) 
			one.push_back(*k);
	if (one.size() == 0) 
		result = 0;
	else {
		if (one.size() == 1)
			result= 3;
		else 
			result = calculateScore(one);
	}
	return result;
}

void Table::noOneColor()
{
	bool different = true;
	for (auto k = allClusters.begin(); k < allClusters.end(); k++) {
			if (k->oneColor()) {
				k->points[0]->color = (k->points[0]->color + 1) % 10;
				different = false;
			}
	}
	if (different) {
		cout << endl;
		cout << "All clusters are different";
	}
	else noOneColor();
}

bool Table::doBestRotation()
{
	int maxScore = 0;
	int nowScore = 0;
	Rotation bestrotation;

	for (auto p = allClusters.begin(); p < allClusters.end(); p++) {
		p->rotate(1);
		nowScore = checkOneColor();
		if (nowScore > maxScore) {
			maxScore = nowScore;
			bestrotation.cluster = *p;
			bestrotation.turn = 1;
		}
		p->rotate(2);
		p->rotate(2);
		nowScore = checkOneColor();
		if (nowScore > maxScore) {
			maxScore = nowScore;
			bestrotation.cluster = *p;
			bestrotation.turn = 2;
		}
		p->rotate(1);
	}
	if (maxScore > 0) {
		vector<Cluster> bestClusters;
		bestrotation.cluster.rotate(bestrotation.turn);

		for (auto k = allClusters.begin(); k < allClusters.end(); k++) {
			if (k->oneColor()) bestClusters.push_back(*k);
		}
		score += maxScore;
		cout << "Score now: " << score;
		this->deleteAndFill(bestClusters);
		cout <<endl<< "Table: ";
		this->draw();
		cout << endl;
		
		return true;
	}
	else {
		cout << endl;
		cout << "No more rotation" << endl;
		cout<< "Final Score = " << score << endl;
		return false;
	}
}

void Table::deleteAndFill(vector<Cluster> deleted)
{
	random_device random;
	vector <Point* > d;
	Point* f;
	unordered_set<Point*>::iterator search;
	for (auto v : deleted) {
		d.push_back(v.points[0]); d.push_back(v.points[1]); d.push_back(v.points[2]);
	}
	unordered_set<Point*> deletepoints(d.begin(), d.end());
	bool fill = false;
	while (deletepoints.size() > 0) {
		auto p = deletepoints.begin();
		int i = 2;
		fill = false;
		while (!fill) {
			if ((*p)->row - i < 0) {
				(*p)->color = random()%10;
				fill = true;
				deletepoints.erase(p);
			}
			else {
				f = findPoint((*p)->row - i, (*p)->column);
				search = deletepoints.find(f);
				if (search != deletepoints.end()) {
					i += 2;
				}
				else {
					(*p)->color = f->color; fill = true;
					deletepoints.erase(p);
					deletepoints.insert(f);
				}
			}
		}
	}
	if (checkOneColor() > 0) {
		vector<Cluster> deleteClusters;
		for (auto k = allClusters.begin(); k < allClusters.end(); k++) {
			if (k->oneColor()) deleteClusters.push_back(*k);
		}

		score += checkOneColor();
		this->draw();
		this->deleteAndFill(deleteClusters);
		cout << endl;
		cout << "Deleted again" << endl; 
		cout << "Score now : " << score;

	}


}


void Table::draw()
{
	cout << endl;
	int size = points.size();
	int i = 0;
	for (int j = 0; j < Column; j++) //start
		cout << "   _";
	cout << endl;
	while (i <= size){
		if (i == 0) { //first
			cout << ' ';
			for (int j = 0; j < Column; j++){
				cout << "_/" << points[i].color << "\\";
				i++;
			}
			cout << endl;
		}
		if (i == size) { //last
			cout << " ";
			for (int j = 0; j < Column; j++)
				cout << " \\_/";
			cout << endl;
			break;
		}
		for (int j = 0; j < Column; j++) { //top
			cout << "/" << points[i].color << "\\_";
			i++;
		}
		cout << '/';
		cout << endl;
		for (int j = 0; j < Column; j++) {  //bottom
			cout << "\\_/" << points[i].color;
			i++;
		}
		cout << '\\';
		cout << endl;
	}



}

Point* Table::findPoint(int r, int c)
{
	for (auto p = points.begin();p < points.end(); p++) {
		if ((p->row == r) && (p->column == c)) return &*p;
	}
}

int Table::calculateScore(vector<Cluster> v)
{
	vector <Point*> s;
	int count = v.size();
	for (auto p = v.begin(); p < v.end(); p++) {
		s.push_back(p->points[0]);
		s.push_back(p->points[1]);
		s.push_back(p->points[2]);
	}
	unordered_set<Point*> us(s.begin(), s.end());
	int po = us.size();
	int result = 0;
	while (count>0){
		if (po == 4 && count == 2) { result += 3 * 3; break; }
		if (po == 5 && count == 2) { result += 3 * 3; break; }
		if (po == 5 && count == 3) { result += 3 * 3 * 3; break; }
		if (po == 6 && count == 4) { result += 3 * 3 * 3 * 3; break; }
		po -= 3;
		count--;
		result += 3;
	}
	return result;
}