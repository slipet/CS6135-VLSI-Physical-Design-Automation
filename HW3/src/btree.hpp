#ifndef BTREE_H
#define BTREE_H
#include<map>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>
#include<iostream>
#include<algorithm>
#include<ctime>
#include<time.h>
#include<list>
#include <climits>
#include<stack>
using namespace std;

extern float Area;
extern float dratio;
extern int Max_x,Max_y;
const bool LEFT=0, RIGHT=1;

typedef bool DIR;

struct module
{
	int name;
	int w;
	int h;
	int x;
	int y;
	int bounding_x,bounding_y;
	int area;
	bool rotation;//0 or 1 to representation the state of rotation;
	//0:w h 1:h w
};

struct fixed_pin
{
	int index;
	int x;
	int y;
};

struct net
{
	int degree;
	int pin_num;
	int module_num;
	vector<module> modules;
	vector<fixed_pin> pins;
};
struct node
{
	int id,parent,lchild,rchild;
	bool leaf;
	module block;
	
	bool isleaf(){return (lchild==-1&&rchild==-1);}
};
struct Contour{
	int front,back;
};
struct Pin{
  int mod;
  int net;
  int x,y;
  int ax,ay;  
  Pin(int x_=-1,int y_=-1){ x=x_,y=y_; }
};
struct backup{
      int nodes_head;
      vector<node> nodes;
      double cost;
      backup() { cost = 1; }
      void clear() { cost = 1, nodes.clear(); }	
};
class FloorPlan
{
public:	
	
	virtual void perturb()=0;
	virtual void set_Mod_Pos()=0;
    virtual void keep_sol()	=0;
    virtual void keep_best()	=0;
    virtual void recover()	=0;
    virtual void recover_best() =0;	
	virtual double getCost();
	virtual void keep_constraint()=0;
	virtual void recover_constraint()=0;
	
	FloorPlan(float calpha);
	map<int,module> fp_modules;
	map<int,fixed_pin> fp_pins;
	map<int,net> fp_nets;


    int    size()         { return modules_N; }
    double getTotalArea() { return TotalArea; }
    double getArea()      { return Area;      }
    int    getWireLength(){ return WireLength;}
    double getWidth()     { return Width;     }
    double getHeight()    { return Height;    }
	float  getDeadSpace();
	double  getConstraintH();
	double  getConstraintW();
	double  getDSR();
	
	void clear();
	void normalize_cost(int);
	
	int modules_N;
    double Area;
    double Width,Height;
    int WireLength;
    double TotalArea;
	double norm_area, norm_wire;
	float cost_alpha;
	void setting(map<int,net> &,map<int,fixed_pin> &,map<int,module> &);
	
	
	
	double calcWireLength();
};
class btree:public FloorPlan{
public:
	
	btree(float calpha=1) :FloorPlan(calpha) {}
	

	
	//
	virtual void perturb();
    virtual void keep_sol();
    virtual void keep_best();
    virtual void keep_constraint();
    virtual void recover();
    virtual void recover_best();
	virtual void recover_constraint();
	
	int contour_head,nodes_head;
	vector<node> nodes;
	vector<Contour> contour;
	
	
	friend void update_module(vector<node> &,map<int,module> &);
	virtual void set_Mod_Pos();
	void rand_gen_tree(map<int,module> &);
	void clear();

	void print_tree();
	void plot(char *,int);
	void place_module(int ,int,int);
	
	void wire_nodes(int parent,int child,DIR edge);
	int child(int node,DIR d);
    // SA permutating operation
    void swap_node(node &n1, node &n2);
    void insert_node(node &parent,node &node);
    void delete_node(node &node);
    
    bool delete_node2(node &node,DIR pull);
    void insert_node2(node &parent,node &node,DIR edge,DIR push,bool fold=false);	
	
    void get_backup(backup &sol);
    void recover(backup &sol);
    //void recover_partial();

    backup best_sol, last_sol,constraint_sol;

    vector<node> changed_nodes;
    int changed_root; 	
	
};


void print(btree &fp,char* filename);
int SA(FloorPlan &fp,int h,int w, int k, int local=0, float term_T=0.1);
void update_module(vector<node> &,map<int,module> &);
void output(char* ,btree &,int );
int getwirelength(map<int,net>&,map<int,module>&,map<int,fixed_pin>&);
vector<string> split(string , char );
void read_hardmodule(char *,map<int,module> &,map<int,fixed_pin> &);
void read_pl(char *,map<int,fixed_pin> &);
void read_net(char *,map<int,net> &,map<int,fixed_pin> &,map<int,module> &);

#endif