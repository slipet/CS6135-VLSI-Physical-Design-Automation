#include "btree.hpp"

float dratio=0;
float Area=0;
int Max_x=0,Max_y=0;
float rotate_rate = 0.3;
float swap_rate = 0.5;


/////////////
float init_avg = 0.00001;
int hill_climb_stage = 4;
float avg_ratio=150;
float lamda=0.6;
////////////
bool rand_bool(){
  return bool(rand()%2);
}

float rand_01(){
  return float(rand()%10000)/10000;
}

vector<string> split(string strToSplit, char delimeter){
    stringstream ss(strToSplit);
    string item;
    vector<string> splittedStrings;
    while (getline(ss, item, delimeter)){
       splittedStrings.push_back(item);
    }
    return splittedStrings;
}

void btree::plot(char *filename,int Max_x){
	
	

	
	FILE *fptr; 
	fptr = fopen("plot.txt", "w");
	map<int,module>::iterator it;
	
	fprintf(fptr,"void setup(){\n");
    fprintf(fptr,"size(1000,1000);\n");
	fprintf(fptr,"background(255);\n");
	fprintf(fptr,"}\n");
	fprintf(fptr,"void draw(){\n");
	fprintf(fptr,"noFill();\n");
	fprintf(fptr,"stroke(0,0,0);\n");
	fprintf(fptr,"fill(255,255,255,128);\n");

	fprintf(fptr,"fill(255,255,255,128);\n");
	fprintf(fptr,"rect(0,0,%d,%d);\n",Max_x,Max_x);
	fprintf(fptr,"fill(0,0,0,128);\n");
	
	
	for(it=fp_modules.begin();it!=fp_modules.end();it++){

		fprintf(fptr,"fill(255,255,255,128);\n");
		fprintf(fptr,"rect(%d,%d,%d,%d);\n",(it->second).x,(it->second).y,(((it->second).rotation)?(it->second).h:(it->second).w),(((it->second).rotation)?(it->second).w:(it->second).h));
		fprintf(fptr,"fill(0,0,0,128);\n");
		fprintf(fptr,"textSize(10);\n");
		fprintf(fptr,"text(\"%d\",%d,%d);\n",(it->second).name,(it->second).x,((it->second).y+(((it->second).rotation)?(it->second).w:(it->second).h)));

	}
	fprintf(fptr,"}\n");
	fclose(fptr);
}

void read_hardmodule(char *filename,map<int,module> &modules,map<int,fixed_pin> &pins){
	Area=0;Max_x=0;Max_y=0;
	string line;
	vector<string> split_line;
	ifstream modulefile (filename);
	int HardModuleNum=0;
	int TerminalNum=0;
		
	getline(modulefile,line);
	split_line=split(line,' ');
	HardModuleNum=stoi(split_line[2],nullptr,10);
	
	getline(modulefile,line);
	split_line=split(line,' ');
	TerminalNum=stoi(split_line[2],nullptr,10);
	
	cout<<"HardModuleNum:"<<HardModuleNum<<" TerminalNum:"<<TerminalNum<<endl;
	
	if (modulefile.is_open()){
			getline(modulefile,line);
			for(int i=0;i<HardModuleNum;i++){
				int module_name=0;
				getline(modulefile,line);
				split_line=split(line,' ');
				split_line[7].erase(0,1);
				split_line[7].erase(split_line[7].length()-1,1);
				split_line[8].erase(split_line[8].length()-1,1);
				module_name=stoi(split_line[0].erase(0,2),nullptr,10);
				
				module *tmp=new module;				
				tmp->name=module_name;
				tmp->w=stoi(split_line[7]);
				tmp->h=stoi(split_line[8]);
				tmp->rotation=0;
				tmp->x=0;
				tmp->y=0;
				Area+=((tmp->w)*(tmp->h));
				tmp->area=Area;
				modules[module_name]=*tmp;
				//cout<<"|"<<module_name<<"|"<<modules[module_name].w<<" "<<modules[module_name].h<<endl;
				delete tmp;
			}			
			getline(modulefile,line);
			for(int i=0;i<TerminalNum;i++){
				int pin_name=0;
				getline(modulefile,line);
				split_line=split(line,' ');
				
				pin_name=stoi(split_line[0].erase(0,1),nullptr,10);
				//cout<<pin_name<<endl;		
				fixed_pin *tmp=new fixed_pin;
				
				tmp->index=pin_name;
				tmp->x=0;
				tmp->y=0;
				pins[pin_name]=*tmp;
				delete tmp;
			}			
		modulefile.close();
	}else cout << "Unable to open file:"<<filename<<endl;
	
	Max_x=Max_y=(sqrt(Area*(1+dratio)));
	
}

void read_pl(char *filename,map<int,fixed_pin> &pins){
	string line;
	vector<string> split_line;
	ifstream plfile (filename);
	if (plfile.is_open()){
		while(getline(plfile,line)){
			int pin_name=0;
			split_line=split(line,'\t');
			//cout<<"|"<<split_line[0]<<"|"<<split_line[1]<<" "<<split_line[2]<<endl;
			split_line[0].erase(0,1);
			pin_name=stoi(split_line[0],nullptr,10);

			pins[pin_name].x=stoi(split_line[1],nullptr,10);
			pins[pin_name].y=stoi(split_line[2],nullptr,10);
			//cout<<"|"<<pin_name<<"|"<<pins[pin_name].x<<" "<<pins[pin_name].y<<endl;
		}
		plfile.close();
	}else cout << "Unable to open file:"<<filename<<endl; 
}

void read_net(char *filename,map<int,net> &nets,map<int,fixed_pin> &pins,map<int,module> &modules){
	string line;
	vector<string> split_line;
	int NumNets=0;
	int NumPins=0;
	ifstream netfile (filename);
	
	getline(netfile,line);
	split_line=split(line,' ');
	NumNets=stoi(split_line[2],nullptr,10);
	
	getline(netfile,line);
	split_line=split(line,' ');
	NumPins=stoi(split_line[2],nullptr,10);
	
	cout<<"NumNets:"<<NumNets<<" NumPins:"<<NumPins<<endl;		
	
	if (netfile.is_open()){
		for(int i=0;i<NumNets;i++){
			getline(netfile,line);
			split_line=split(line,' ');
			int degree=stoi(split_line[2],nullptr,10);
			nets[i].degree=degree;
			nets[i].pin_num=0;
			nets[i].module_num=0;
			for(int j=0;j<degree;j++){
				
				getline(netfile,line);
				if(line[0]=='p'){
					line.erase(0,1);
					int pin_name=stoi(line,nullptr,10);
					nets[i].pin_num+=1;
					nets[i].pins.push_back(pins[pin_name]);
					//cout<<"pin:"<<pin_name<<" x:"<<nets[i].pins[pin_name].x<<" y:"<<nets[i].pins[pin_name].y<<endl;
				}
				if(line[0]=='s'){
					line.erase(0,2);
					int module_name=stoi(line,nullptr,10);
					nets[i].module_num+=1;
					nets[i].modules.push_back(modules[module_name]);
					//cout<<"moduel:"<<module_name<<" w:"<<nets[i].modules[module_name].w<<" h:"<<nets[i].modules[module_name].h<<endl;
				}
			}
		}
		netfile.close();
	}else cout << "Unable to open file:"<<filename<<endl;			
}



void output(char* filename,btree &fp,int length){
	
	FILE *fptr;
	cout<<"filename:"<<filename<<endl;
	fptr = fopen(filename, "w");
	
	fprintf(fptr,"Wirelength %d\n",length);
	fprintf(fptr,"Blocks\n");
	
	for(int i=0;i<fp.modules_N;i++){
		//cout<<"i:"<<i<<" "<<modules[i].name<<endl;
		fprintf(fptr,"sb%d %d %d %d\n",fp.fp_modules[i].name,fp.fp_modules[i].x,fp.fp_modules[i].y,fp.fp_modules[i].rotation);
	}
	cout<<"modules_N"<<fp.modules_N<<endl;
	fclose(fptr);

	print(fp,filename);	

}

int getwirelength(map<int,net>& nets,map<int,module>&modules,map<int,fixed_pin>&pins){
	map<int,net>::iterator it;

	int wirelength=0;
	//cout<<"heare"<<endl;
	for(it=nets.begin();it!=nets.end();it++){
		int min_x=INT_MAX,min_y=INT_MAX;
		int max_x=INT_MIN,max_y=INT_MIN;		
		int pin_n=(it->second).pin_num;
		int module_n=(it->second).module_num;
		
		for(int j=0;j<pin_n;j++){
			min_x=min(min_x,((it->second).pins[j]).x);
			min_y=min(min_y,((it->second).pins[j]).y);
			max_y=max(max_y,((it->second).pins[j]).y);
			max_x=max(max_x,((it->second).pins[j]).x);
			
		}
		
		for(int j=0;j<module_n;j++){
			int id=((it->second).modules[j]).name;
			int h=((modules[id]).rotation)?((modules[id])).w:((modules[id])).h;
			int w=((modules[id]).rotation)?((modules[id])).h:((modules[id])).w;
			int center_x=((modules[id]).x+w)/2;
			int center_y=((modules[id]).y+h)/2;
			min_x=min(min_x,center_x);
			min_y=min(min_y,center_y);
			max_y=max(max_y,center_y);
			max_x=max(max_x,center_x);
			
		}
		
		wirelength+=((max_x-min_x)+(max_y-min_y));
	}
	cout<<"here wirelength: ";
	cout<<"wirelength:"<<wirelength<<endl;
	
	return wirelength;
}


void btree::clear(){
  // initial contour value
  contour_head = -1;
  FloorPlan::clear();
}

void btree::set_Mod_Pos(){
	stack<int> Stack;
	clear();
	int cur=nodes_head;
	
	
	place_module(cur,-1,0);
	
	//nodes[0].block.rotation=0;
	node &CurNode = nodes[cur];
	if(CurNode.rchild != -1)      Stack.push(CurNode.rchild);
	if(CurNode.lchild  != -1)      Stack.push(CurNode.lchild);	
	
	
	
	while(!Stack.empty()){
		cur = Stack.top();
		Stack.pop();		
		node &CurNode = nodes[cur];
		bool is_left = (nodes[CurNode.parent].lchild == CurNode.id);
		//cout<<"cur:"<<cur<<" "<<nodes[cur].block.name<<endl;
		
		place_module(cur,CurNode.parent,is_left);
		
		if(CurNode.rchild != -1)      Stack.push(CurNode.rchild);
		if(CurNode.lchild  != -1)      Stack.push(CurNode.lchild);
	}
	double max_x=-1,max_y=-1;
	for(int p= contour_head; p != -1; p=contour[p].front){
		max_x = max(max_x,double(fp_modules[p].bounding_x));  
		max_y = max(max_y,double(fp_modules[p].bounding_y));
	}

	Width  = max_x;
	Height = max_y;
	Area   = Height*Width;
	
	FloorPlan::set_Mod_Pos(); 	 	
	
	
}


void btree::place_module(int cur,int pre,int is_left){
	module &CurModule = fp_modules[cur];
	//cout<<"========================================"<<endl;
	
	module &PModule = fp_modules[pre];
	int w =  CurModule.w;
	int h =  CurModule.h;
	if(CurModule.rotation){
		int tmp=w;
		w=h;
		h=tmp;
	}
	//cout<<"before place cur:"<<cur<<" x:"<<CurModule.x<<"y:"<<CurModule.y<<" h:"<<h<<"w:"<<w<<endl;
	if(pre==-1){
		
		contour_head = cur;
		contour[cur].back = -1;
		contour[cur].front = -1;
		
		CurModule.x = 0;
		CurModule.y=0;
		
		CurModule.bounding_x = w;
		CurModule.bounding_y = h;
		//cout<<"after place cur:"<<cur<<" x:"<<CurModule.x<<"y:"<<CurModule.y<<" h:"<<h<<"w:"<<w<<" rx:"<<CurModule.bounding_x<<"ry:"<<CurModule.bounding_y<<endl;
		return;		
	}
	
	int contour_p;


	if(is_left){
		int pre_w=(PModule.rotation ? PModule.h:PModule.w);
		
		CurModule.x=PModule.x+pre_w;
		
		CurModule.bounding_x=CurModule.x+w;
		
		contour_p=contour[pre].front;
		contour[pre].front = cur;
		contour[cur].back = pre;
		
		if(contour_p==-1){
		  CurModule.y = 0;
		  CurModule.bounding_y = h;
		  contour[cur].front = -1;
		  //cout<<"after place cur:"<<cur<<" x:"<<CurModule.x<<"y:"<<CurModule.y<<" h:"<<h<<"w:"<<w<<" rx:"<<CurModule.bounding_x<<"ry:"<<CurModule.bounding_y<<endl;
		  return;
		}		
	}else{	// upper
		CurModule.x = PModule.x;
		CurModule.bounding_x = CurModule.x + w;
		contour_p = pre;
		 
		int now=contour[pre].back;

		if(now==-1){
		  contour_head = cur;
		  contour[cur].back = -1;
		  //cout<<"contour_head:"<<contour_head<<"contour_p:"<<contour[pre].front<<endl;
		}
		else{
		  contour[now].front = cur;
		  contour[cur].back = now;
		}
	}
	int Y=INT_MIN;
	int contour_X,contour_Y;
	
	//for(int a=contour_p; a!=-1 ; a=contour[a].front)
		//cout<<"p:"<<a<<" ";
	//cout<<endl;
	for(; contour_p!=-1 ; contour_p=contour[contour_p].front){
		//cout<<"p:"<<contour_p<<"| ";
		contour_X = fp_modules[contour_p].bounding_x;
		contour_Y = fp_modules[contour_p].bounding_y;
		//cout<<"p:"<<contour_p<<"| contour_Y:"<<contour_Y<<endl;
		Y = max(Y, contour_Y);
		  
		if(contour_X >= CurModule.bounding_x){
		  CurModule.y = Y;
		  CurModule.bounding_y = CurModule.y + h;
		  if(contour_X >CurModule.bounding_x){
			contour[cur].front = contour_p;
			contour[contour_p].back = cur;
		  }else{ 
			int n= contour[contour_p].front;
			
			contour[cur].front = n;
			if(n!=-1)
			  contour[n].back = cur;
		  }
		  break;     
		}
	  }
	
	if(contour_p==-1){
		CurModule.y  = (Y==INT_MIN? 0 : Y);
		CurModule.bounding_y = CurModule.y + h;
		contour[cur].front = -1;
	}
	//cout<<"after place cur:"<<cur<<" x:"<<CurModule.x<<"y:"<<CurModule.y<<" h:"<<h<<"w:"<<w<<" rx:"<<CurModule.bounding_x<<"ry:"<<CurModule.bounding_y<<endl;
}

void btree::rand_gen_tree(map<int,module> &modules){
	fp_modules=modules;
	
	modules_N = fp_modules.size();
	TotalArea = 0;
	for(int i=0; i < modules_N; i++)
		TotalArea += fp_modules[i].area;
	
	

	nodes.resize(modules_N);
	contour.resize(modules_N);
	for(int i=0;i<modules_N;i++){
		nodes[i].id = i;
		nodes[i].block=fp_modules[i];
		nodes[i].block.rotation=rand()%2;
		
		nodes[i].parent=(i+1)/2-1;
		
		
		if((i*2+1)<modules_N)
			nodes[i].lchild=(i*2+1);
		else 
			nodes[i].lchild=-1;
		
		if((i*2+2)<modules_N)
			nodes[i].rchild=(i*2+2);
		else 
			nodes[i].rchild=-1;

	}
	
	nodes[0].parent=-1;
	nodes_head=0;
	contour_head=-1;
	clear();

	
	normalize_cost(500);
}
void update_module(vector<node> &nodes,map<int,module> &modules){
	for(int i=0;i<nodes.size();i++){
		modules[nodes[i].block.name]=nodes[i].block;
	}
}

void btree::print_tree(){
  cout << "root: " << 0 << endl;
  for(int i=0; i < nodes.size(); i++){
    cout << nodes[i].block.name << ": ";
    cout << nodes[i].lchild << " ";
    cout << nodes[i].parent << " ";
    cout << nodes[i].rchild << endl;
  }
}


void btree::wire_nodes(int parent,int child,DIR edge){

  (edge==LEFT? nodes[parent].lchild: nodes[parent].rchild) = child;
  if(child!=-1) nodes[child].parent = nodes[parent].id;
}

int btree::child(int node,DIR d){

  return (d==LEFT? nodes[node].lchild:nodes[node].rchild);  
}

void btree::insert_node(node &parent, node &node){
  node.parent = parent.id;
  bool edge = rand_bool();

  if(edge){

    node.lchild  = parent.lchild;
    node.rchild = -1;
    if(parent.lchild!=-1)
      nodes[parent.lchild].parent = node.id;

    parent.lchild = node.id;

  }else{

    node.lchild  = -1;
    node.rchild = parent.rchild;
    if(parent.rchild!=-1)
      nodes[parent.rchild].parent = node.id;
    
    parent.rchild = node.id;
  }

}

void btree::swap_node(node &n1, node &n2){

  if(n1.lchild!=-1){  

    nodes[n1.lchild].parent  = n2.id;
  }
  if(n1.rchild!=-1){
 
    nodes[n1.rchild].parent = n2.id;  
  }
  if(n2.lchild!=-1){

    nodes[n2.lchild].parent  = n1.id;
  }
  if(n2.rchild!=-1){

    nodes[n2.rchild].parent = n1.id;  
  }

  if(n1.parent != -1){

    if(nodes[n1.parent].lchild==n1.id)
       nodes[n1.parent].lchild  = n2.id;
    else
       nodes[n1.parent].rchild = n2.id; 
  }else{
  
    nodes_head = n2.id;
  }

  if(n2.parent != -1){

    if(nodes[n2.parent].lchild==n2.id)
       nodes[n2.parent].lchild  = n1.id;
    else
       nodes[n2.parent].rchild = n1.id; 
  }else{

    nodes_head = n1.id;
  }

  swap(n1.lchild,n2.lchild);
  swap(n1.rchild,n2.rchild);
  swap(n1.parent,n2.parent);
}

void btree::delete_node(node &input_node){
  int child    = -1;	
  int subchild = -1;   
  int subparent= -1; 

  if(!input_node.isleaf()){
    bool left= rand_bool();			
    if(input_node.lchild ==-1) left=false;
    if(input_node.rchild==-1) left=true;



    if(left){
      child = input_node.lchild;			
      if(input_node.rchild!=-1){
        subchild  = nodes[child].rchild;
        subparent = input_node.rchild;
        nodes[input_node.rchild].parent = child; 
        nodes[child].rchild = input_node.rchild;	
      }
    }
    else{
      child = input_node.rchild;
      if(input_node.lchild!=-1){
        subchild  = nodes[child].lchild;
        subparent = input_node.lchild;
	nodes[input_node.lchild].parent = child;
        nodes[child].lchild = input_node.lchild;
      }
    }

    nodes[child].parent = input_node.parent;
  }

  if(input_node.parent == -1){			
    nodes_head = child;
  }else{					
    if(input_node.id == nodes[input_node.parent].lchild)
      nodes[input_node.parent].lchild  = child;
    else
      nodes[input_node.parent].rchild = child;
  }


  if(subchild != -1){
    node &sc= nodes[subchild];
    

    while(1){
      node &p= nodes[subparent];

      if(p.lchild==-1 || p.rchild==-1){


	sc.parent = p.id;
        if(p.lchild==-1) p.lchild = sc.id;
        else p.rchild = sc.id;
        break;
      }else{
	subparent = (rand_bool() ? p.lchild : p.rchild);
      }
    }
  }
}

bool btree::delete_node2(node &node,DIR pull){
  DIR npull = !pull;
 
  int p = node.parent;
  int n= node.id;
  int c= child(n,pull);
  int cn=child(n,npull);


  DIR p2c = (nodes[p].lchild==n ? LEFT:RIGHT);

  if(c==-1){
    wire_nodes(p,cn,p2c);
    return (cn!=-1);  
  }else{
    wire_nodes(p,c,p2c);
  }

  while(c!=-1){
    int k=child(c,npull);
    wire_nodes(c,cn ,npull);
    cn= k;
    n= c;
    c= child(c,pull);
  }

  if(cn != -1){
    wire_nodes(n,cn,pull);
    return true;
  }else 
    return false;
}

void btree::insert_node2(node &parent,node &node,\
                        DIR edge=LEFT,DIR push=LEFT,bool fold){
  DIR npush = !push;
  int p= parent.id;
  int n= node.id;
  int c= child(p,edge);

  wire_nodes(p,n,edge);
  wire_nodes(n,c,push);
    
  while(c!=-1){
    wire_nodes(n,child(c,npush) ,npush);
    n= c;
    c= child(c,push);
  }
  wire_nodes(n,-1,npush);

  if(fold){
    wire_nodes(nodes[n].parent,-1,push);
    wire_nodes(nodes[n].parent,n,npush); 
  }
}

void btree::perturb(){
  int p,n;
  n = rand()%(nodes.size());




  if(rotate_rate > rand_01()){

    nodes[n].block.rotation = !nodes[n].block.rotation;
   
  }
  else{ 	

    if(swap_rate >rand_01()){
      do{
        p = rand()%(nodes.size());
      }while(n==p||nodes[n].parent==p||nodes[p].parent==n);



      swap_node(nodes[p],nodes[n]);

    }else{
      do{
        p = rand()%(nodes.size());
      }while(n==p);



      delete_node(nodes[n]);
      insert_node(nodes[p],nodes[n]);
    }
  }
}



void btree::get_backup(backup &sol){
  sol.nodes_head = nodes_head;
  sol.nodes = nodes;
  sol.cost = getCost();
}
//////////////////
/////////////////
void btree::keep_sol(){
  get_backup(last_sol);
}

void btree::keep_best(){
  get_backup(best_sol);
}
void btree::keep_constraint(){
  get_backup(constraint_sol);
}
void btree::recover(){
  recover(last_sol);
  // recover_partial();
}

void btree::recover_best(){
  recover(best_sol);
}
void btree::recover_constraint(){
  recover(constraint_sol);
}
void btree::recover(backup &sol){
  nodes_head = sol.nodes_head;
  nodes = sol.nodes;
}


//////////////////
//////////////////

FloorPlan::FloorPlan(float calpha=1){
  norm_area= 1;
  norm_wire= 1;
  cost_alpha=calpha;
  
}

void FloorPlan::setting(map<int,net> &input_nets,map<int,fixed_pin> &input_pins,map<int,module> &input_modules){
	fp_nets=input_nets;
	fp_pins=input_pins;
	fp_modules=input_modules;
	
	//cout<<"modules[0].name:"<<fp_modules[0].name<<endl;
}



void FloorPlan::clear(){
  Area = 0; 
  WireLength = 0;
}

void FloorPlan::normalize_cost(int t){
  norm_area=norm_wire=0;

  for(int i=0; i < t; i++){
    perturb();
    set_Mod_Pos();
    norm_area += Area;
    norm_wire += WireLength;
  }
  
  norm_area /= t;
  norm_wire /= t;
  //printf("normalize area=%.0f, wire=%.0f\n", norm_area, norm_wire);
}

////////////////////////////
void print(btree &fp,char* filename){
	if(fp.modules_N>250){
		char txt2[]="../src/plot3.txt";
		
		string line;
		FILE *fptr;
		ifstream file (txt2);
		fptr = fopen(filename, "w");	
		//fptr = fopen("../output/n200.floorplan", "w");
		cout<<"in print"<<endl;
		
		
		if(file.is_open()){
			while(getline(file,line)){
				fprintf(fptr,line.c_str());
				fprintf(fptr,"\n");
			}
			file.close();
			fclose(fptr);
		}
				
	}else{
		char txt2[]="../src/plot2.txt";
		
		string line;
		FILE *fptr;
		ifstream file (txt2);
		fptr = fopen(filename, "w");	
		//fptr = fopen("../output/n200.floorplan", "w");
		cout<<"in print"<<endl;
		
		
		if(file.is_open()){
			while(getline(file,line)){
				fprintf(fptr,line.c_str());
				fprintf(fptr,"\n");
			}
			file.close();
			fclose(fptr);
		}	
	}

	
}
//////////////////
void FloorPlan::set_Mod_Pos(){
  if(cost_alpha!=1);
     calcWireLength();
}
double FloorPlan::calcWireLength(){

	map<int,net>::iterator it;

	WireLength=0;
	//cout<<"net:"<<0<<"x:"<<fp_nets[0].pins[0].x<<"y:"<<fp_nets[0].pins[0].y<<endl;
	//cout<<"id:"<<fp_nets[0].modules[0].name<<" x:"<<(fp_modules[fp_nets[0].modules[0].name]).x<<"y:"<<(fp_modules[fp_nets[0].modules[0].name]).y<<endl;
	//cout<<"r:"<<(fp_modules[fp_nets[0].modules[0].name]).rotation<<"w:"<<(fp_modules[fp_nets[0].modules[0].name]).w<<"h:"<<(fp_modules[fp_nets[0].modules[0].name]).h<<endl;
	for(it=fp_nets.begin();it!=fp_nets.end();it++){
		//cout<<"net:"<<it->first<<endl;
		int min_x=INT_MAX,min_y=INT_MAX;
		int max_x=INT_MIN,max_y=INT_MIN;		
		int pin_n=(it->second).pin_num;
		int module_n=(it->second).module_num;
		//cout<<"net:"<<it->first<<" pin_n:"<<pin_n<<" module_n:"<<module_n;
		
		for(int j=0;j<module_n;j++){
			
			int id=((it->second).modules[j]).name;
			//cout<<"net:"<<it->first<<"w:"<<((it->second).modules[j]).w<<"h:"<<((it->second).modules[j]).h;
			//cout<<"|| id:"<<id<<"w:"<<(fp_modules[id]).w<<"h:"<<(fp_modules[id]).h<<endl;
			int h=(fp_modules[id].rotation)?(fp_modules[id]).w:(fp_modules[id]).h;
			int w=(fp_modules[id].rotation)?(fp_modules[id]).h:(fp_modules[id]).w;
			int center_x=((fp_modules[id]).x)+w/2;
			int center_y=((fp_modules[id]).y)+h/2;
			min_x=min(min_x,center_x);
			min_y=min(min_y,center_y);
			max_y=max(max_y,center_y);
			max_x=max(max_x,center_x);
			//
		}		
		for(int j=0;j<pin_n;j++){
			
			int id=(it->second).pins[j].index;
			//cout<<" id:"<<id<<" x:"<<fp_pins[id].x<<" y:"<<fp_pins[id].y;
			min_x=min(min_x,(fp_pins[id].x));
			min_y=min(min_y,(fp_pins[id].y));
			max_y=max(max_y,(fp_pins[id].y));
			max_x=max(max_x,(fp_pins[id].x));
			
		}
		//cout<<" min_x:"<<min_x<<"max_x:"<<max_x<<"min_y:"<<min_y<<"max_y:"<<max_y;
		//cout<<endl;

		//(it->first==0)cout<<"min_x:"<<min_x<<"max_x:"<<max_x<<"min_y:"<<min_y<<"max_y:"<<max_y<<endl;
		//if(it->first==0)cout<<"length:"<<((max_y-max_y))<<endl;
		WireLength+=((max_x-min_x)+(max_y-min_y));
	}
	//cout<<"here wirelength"<<endl;
	//cout<<"wirelength:"<<WireLength<<endl;  
 
	//cout<<"net size:"<<fp_nets.size()<<endl;

  return WireLength;
}
float FloorPlan::getDeadSpace(){
  return 100*(Area-TotalArea)/float(Area);
}
/////////////////////////////////////////////
double FloorPlan::getConstraintH(){
  return sqrt((1+getDeadSpace())*Area*0.1);
}
double FloorPlan::getConstraintW(){
  return sqrt((1+getDeadSpace())*Area/0.1);
}
double FloorPlan::getDSR(){
  return (getConstraintH()/getConstraintW());
}
/////////////////////////////////////////////
/////////////////////////////////////////////

double mean(vector<double> &chain){
  double sum=0;
  for(int i=0; i < chain.size();i++)
     sum+= chain[i];
  return sum/chain.size();
}

double std_var(vector<double> &chain){
  double m = mean(chain);
  double sum=0;
  double var;
  int N= chain.size();

  for(int i=0; i < N;i++)
     sum += (chain[i]-m)*(chain[i]-m);

  var = sqrt(sum/(N-1));
  //printf("  m=%.4f ,v=%.4f\n",m,var);

  return var;
}
double FloorPlan::getCost(){
  //int r=(Height>Width)?Height/Width:Width/Height;
  int beta=0.1;
  if(cost_alpha==1)
     return cost_alpha*(Area/norm_area);
  else if(cost_alpha < 1e-4)
     return (WireLength/norm_wire);
  else
     return cost_alpha*(Area/norm_area)+(beta)*(WireLength/norm_wire)+(1-cost_alpha-beta)*((1-(Height/Width))*(1-(Height/Width)));
}



int SA(FloorPlan &fp,int h,int w, int k, int local, float term_T)
{
  int MT,uphill,reject;
  double pre_cost,best,cost;
  float d_cost,reject_rate;
  
  int N =  k ;//int N = k * fp.size();
  float P=0.9;
  float T,actual_T=1;
  double avg=init_avg;
  float conv_rate = 1;
   

  double estimate_avg = 0.08 / avg_ratio;
  //cout << "Estimate Average Delta Cost = " << estimate_avg << endl;

  if(local==0)
    avg = estimate_avg;

  T = avg / log(P);  
    
  fp.set_Mod_Pos();
  fp.keep_sol();  
  fp.keep_best();
  pre_cost =  best = fp.getCost();
  //return 0;
  int good_num=0,bad_num=0;
  double total_cost=0;
  int count=0;
  int ctr=0;
  
  int num=fp.size();
  
  if(num<150)
	  num=100;
  cout<<"num:"<<num<<endl;
  do{
   count++;
   total_cost = 0;
   MT=uphill=reject=0;
   //printf("Iteration %d, T= %.2f\n", count,  actual_T);
   
   vector<double> chain; 
 
   for(; uphill < num && MT < N; MT++){
       fp.perturb();
       fp.set_Mod_Pos();
       cost = fp.getCost(); 
       d_cost = cost - pre_cost;
       float p = exp(d_cost/T);
       

       chain.push_back(cost);

       if(d_cost <=0 || rand_01() < p ){
         fp.keep_sol();
         pre_cost = cost;

         if(d_cost > 0){       
           uphill++, bad_num++;
           //of << d_cost << ": " << p << endl;
         }else  if(d_cost < 0)  good_num++;

         // keep best solution
         if(cost < best){
           fp.keep_best();
           best = cost;
           //printf("   ==>  Cost= %f, Area= %.6f, ", best, fp.getArea()*1e-6);
           //printf("Wire= %.3f\n", fp.getWireLength()*1e-3);  
           
            
         }
		 if(fp.Height<=h &&fp.Width<=w){
			 ctr=1;
			 //cout<<"constraint meet:"<<fp.Width<<" "<<fp.Height<<endl;
			 //fp.keep_constraint();
		 }
       }
       else{
         reject++;
         fp.recover();
       }
   }
//   cout << T << endl;
   double sv = std_var(chain);
   float r_t = exp(lamda*T/sv);
   T = r_t*T;
	
	//cout<<"before local:"<<T<<endl;
	
   // After apply local-search, start to use normal SA
   if(count ==1){
     T = estimate_avg/ log(P);
     T *= pow(0.9,local);		// smothing the annealing schedule
     actual_T = exp(estimate_avg/T);
	 //T1=actual_T;
   }

   /*
   if(count > local){
		actual_T=T1*mean(chain)/(count);
   }
   */
   if(count > local){
     actual_T = exp(estimate_avg/T);
     conv_rate = 0.95;
   }
	
   reject_rate = float(reject)/MT;
   //printf("  T= %.2f, r= %.2f, reject= %.2f\n\n", actual_T, r_t, reject_rate);
   //ctr--;
  }while( reject_rate < conv_rate&&actual_T > term_T);
//while(reject_rate < conv_rate && actual_T > term_T);
//
  //if(reject_rate >= conv_rate)
    //cout << "\n  Convergent!\n";
  //else if(actual_T <= term_T)
    //cout << "\n Cooling Enough!\n";

  //printf("\n good = %d, bad=%d\n\n", good_num, bad_num);

  fp.recover_best();
  fp.set_Mod_Pos();
  
  if(ctr==1){
	  //fp.recover_constraint();
	  return 1;
	  //cout<<"constraint meet"<<endl;
  }
  return 0; 
}

