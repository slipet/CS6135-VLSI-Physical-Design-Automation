#ifndef GR_H
#define GR_H
#include<ctime>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>
#include<iostream>
#include<queue>
#include <algorithm>
using namespace std;

struct Pin{
	int x;
	int y;
};
struct PathNode{
	int x,y;
};

struct Net{
	int id;
	int pinsNum;
	bool routed;
	Pin pins[2];
	vector<PathNode> paths;
};


struct Edge{
	int rotation;//vertical : 1 horizontal : -1 none : 0
	int max_capacity;
	int capacity;
	vector<Net> passNets;	
};

struct Vertex{
	int x,y;
	vector<Net> passNets;
	
	Edge up;
	Edge down;
	Edge right;
	Edge left;
	
};

///////////////////////////////

struct GraphEdge{
	int weight;
};

struct GraphNode{
	int x,y;
	
	GraphEdge up;
	GraphEdge down;
	GraphEdge right;
	GraphEdge left;	
};

struct GraphPaths{
	int Weight;
	vector<PathNode> path;
};

struct cmp{
	bool operator()(GraphPaths a, GraphPaths b){
		return a.Weight>b.Weight;
	}
};

struct cmp2{
	bool operator()(Net a, Net b){
		int aWidth=(a.pins[0].x>a.pins[1].x)?a.pins[0].x-a.pins[1].x:a.pins[1].x-a.pins[0].x;
		int aHeight=(a.pins[0].y>a.pins[1].y)?a.pins[0].y-a.pins[1].y:a.pins[1].y-a.pins[0].y;
		int bWidth=(b.pins[0].x>b.pins[1].x)?b.pins[0].x-b.pins[1].x:b.pins[1].x-b.pins[0].x;
		int bHeight=(b.pins[0].y>b.pins[1].y)?b.pins[0].y-b.pins[1].y:b.pins[1].y-b.pins[0].y;

		if(aWidth*aHeight==0&&bWidth*bHeight==0){
			int alength=(aWidth==0)?aHeight:aWidth;
			int blength=(bWidth==0)?bHeight:bWidth;
			
			return alength> blength;
		}
		
		if(aWidth*aHeight>=bWidth*bHeight)return true;
		else return false;
		

		
		//Pin pins[2];
		//return a.Weight>b.Weight;
	}
};

//////////////////////////////

/*
class Grids{
	
	vector<vector<Vertex>> map;
	
	vector<vector<Edge>> HEdges;
	vector<vector<Edge>> VEdges;
	
	int Vcapacity;
	int Hcapacity;
	int NetsNum;
	
};*/
class GlobalRouting{

	int NetsNum;
	int Vcapacity;
	int Hcapacity;
	
	int GridWidth;
	int GridHeight;
	
	vector<Net> nets;
	vector<vector<Vertex>> grids;
	

public:
	
	void readfile(char *);
	
	int getNetsNum(){return NetsNum;}
	int getVcapacity(){return Vcapacity;}
	int getHcapacity(){return Hcapacity;}
	int getGridWidth(){return GridWidth;}
	int getGridHeight(){return GridHeight;}
	
	void recoveryPath(Net &,vector<PathNode> &,Pin &,Pin &);
	void output(char*);
	void Routing();
	void RoutNet(Net &);
	
};

#endif