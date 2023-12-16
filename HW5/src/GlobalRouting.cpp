#include "GlobalRouting.hpp"

std::string& trim(std::string &s) 
{
    if (s.empty()) 
    {
        return s;
    }
    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
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

void GlobalRouting::readfile(char *filename){
	string line;
	vector<string> split_line;
	ifstream modulefile (filename);
	
	getline(modulefile,line);//grid 3 3
	split_line=split(line,' ');
	GridWidth=stoi(split_line[1],nullptr,10);
	GridHeight=stoi(split_line[2],nullptr,10);
	
	getline(modulefile,line);//vertical capacity 2
	split_line=split(line,' ');
	Vcapacity=stoi(split_line[2],nullptr,10);
	
	getline(modulefile,line);//horizontal capacity 2
	split_line=split(line,' ');
	Hcapacity=stoi(split_line[2],nullptr,10);
	
	getline(modulefile,line);//num net 3
	split_line=split(line,' ');
	NetsNum=stoi(split_line[2],nullptr,10);
	nets.resize(NetsNum);
	
	cout<<"===================basic setting==================="<<endl;
	cout<<"Grid Width:"<<getGridWidth()<<" Height:"<<getGridHeight()<<endl;
	cout<<"Vertical capacity:"<<getVcapacity()<<" Horizontal capacity:"<<getHcapacity()<<endl;
	cout<<"Nets number:"<<getNetsNum()<<endl;
	cout<<"===================basic setting==================="<<endl;
	//construct grids map
	grids.resize(GridWidth);
	
	int max_hpwl=-1;
	int count=0;
	for (int i=0;i<GridWidth;i++){
		grids[i].resize(GridHeight);
	}
	for (int y=0;y<GridHeight;y++){
		for(int x=0;x<GridWidth;x++){
			grids[x][y].x=x;
			grids[x][y].y=y;
			
			grids[x][y].down.rotation=(y==0)?0:1;
			grids[x][y].up.rotation=(y==(GridHeight-1))?0:1;
			grids[x][y].left.rotation=(x==0)?0:-1;
			grids[x][y].right .rotation=(x==(GridWidth-1))?0:-1;
			
			if(grids[x][y].up.rotation==1){
				grids[x][y].up.max_capacity=Vcapacity;
				grids[x][y].up.capacity=0;
			}
			if(grids[x][y].down.rotation==1){
				grids[x][y].down.max_capacity=Vcapacity;
				grids[x][y].down.capacity=0;
			}
			if(grids[x][y].right.rotation==-1){
				grids[x][y].right.max_capacity=Hcapacity;
				grids[x][y].right.capacity=0;
			}
			if(grids[x][y].left.rotation==-1){
				grids[x][y].left.max_capacity=Hcapacity;
				grids[x][y].left.capacity=0;
			}
			
		}
	}
	
	
/*	
	for (int y=GridHeight-1;y>=0;y--){
		//cout<<i<<" ";
		for(int x=0;x<GridWidth;x++){
			if(grids[x][y].left.rotation==-1){
				cout<<" ";
			}					
			if(grids[x][y].up.rotation==1){
				cout<<"|";
			}
			if(grids[x][y].right.rotation==-1){
				cout<<" ";
			}					
		}
		cout<<endl;
		for(int x=0;x<GridWidth;x++){
			if(grids[x][y].left.rotation==-1){
				cout<<"-";
			}
			cout<<"O";
			if(grids[x][y].right.rotation==-1){
				cout<<"-";
			}					
		}
		cout<<endl;
		for(int x=0;x<GridWidth;x++){
			if(grids[x][y].left.rotation==-1){
				cout<<" ";
			}					
			if(grids[x][y].down.rotation==1){
				cout<<"|";
			}
			if(grids[x][y].right.rotation==-1){
				cout<<" ";
			}
		}
		cout<<endl;
	}	
*/	
	if (modulefile.is_open()){
		for(int i=0;i<NetsNum;i++){
			Net net;
			
			getline(modulefile,line);//net-name net-id # of pins
			split_line=split(line,' ');
			net.id=stoi(split_line[1],nullptr,10);
			net.pinsNum=stoi(split_line[2],nullptr,10);
			//cout<<net.id<<" "<<net.pinsNum<<endl;
			
			getline(modulefile,line);//x y
			line=trim(line);
			split_line=split(line,' ');
			//cout<<split_line[0]<<" "<<split_line[1]<<endl;
			net.pins[0].x=stoi(split_line[0],nullptr,10);
			net.pins[0].y=stoi(split_line[1],nullptr,10);
			
			getline(modulefile,line);
			line=trim(line);
			split_line=split(line,' ');
			net.pins[1].x=stoi(split_line[0],nullptr,10);
			net.pins[1].y=stoi(split_line[1],nullptr,10);
			
			int xwidth=(net.pins[0].x>net.pins[1].x)?net.pins[0].x-net.pins[1].x:net.pins[1].x-net.pins[0].x;
			int yheght=(net.pins[0].y>net.pins[1].y)?net.pins[0].y-net.pins[1].y:net.pins[1].y-net.pins[0].y;
			count+=(xwidth*yheght>64);
			//if(hpwl>max_hpwl)max_hpwl=hpwl;
			
			nets[net.id]=net;
			nets[net.id].routed=false;
		}
		cout<<"count:"<<count<<endl;
		modulefile.close();
	}else cout << "Unable to open file:"<<filename<<endl;
	
}

void GlobalRouting::recoveryPath(Net &net,vector<PathNode> &sol,Pin &start,Pin &end){
	PathNode netP;
	netP.x=start.x;
	netP.y=start.y;
	net.paths.push_back(netP);
	for(int i=1;i<sol.size();i++){
		PathNode nextPoint=net.paths.back();
		int nextx=sol[i].x-sol[i-1].x;
		int nexty=sol[i].y-sol[i-1].y;
		nextPoint.x+=nextx;
		nextPoint.y+=nexty;
		net.paths.push_back(nextPoint);
		//cout<<"("<<sol[i].x<<","<<sol[i].y<<")->";
		
	}
	
	for(int i=1;i<net.paths.size();i++){
		PathNode currentP=net.paths[i-1];
		PathNode nextPoint=net.paths[i];
		
		int nextx=sol[i].x-sol[i-1].x;
		int nexty=sol[i].y-sol[i-1].y;
		
		if(nextx>0){//go right
			//update current
			grids[currentP.x][currentP.y].right.capacity+=1;
			
			//update next
			
			grids[nextPoint.x][nextPoint.y].left.capacity+=1;
			
		}else if(nexty>0){//go up

			grids[currentP.x][currentP.y].up.capacity+=1;
			
			//update next
			
			grids[nextPoint.x][nextPoint.y].down.capacity+=1;
		
		}else if(nexty<0){//go down

			grids[currentP.x][currentP.y].down.capacity+=1;
			
			//update next
			
			grids[nextPoint.x][nextPoint.y].up.capacity+=1;
		
		
		}else{
			cout<<"next x <0"<<endl;
			break;
		}
		
	}
	
	
}

void GlobalRouting::RoutNet(Net &net){
	//cout<<"Rout net:"<<net.id<<" || Pin 0 x:"<<net.pins[0].x<<" y:"<<net.pins[0].y<<" | Pin 1 x:"<<net.pins[1].x<<" y:"<<net.pins[1].y<<endl;
	
	//rout from left to right; if x equal from bottom to top
	
	//debug control
	//net.pins[0].x=20;net.pins[0].y=20;
	//net.pins[1].x=35;net.pins[1].y=5;
	//cout<<"id:"<<net.id<<" "<<endl;
	
	if(net.id==2){
		cout<<net.pins[0].x<<" "<<net.pins[0].y<<endl;
		cout<<net.pins[1].x<<" "<<net.pins[1].y<<endl;
	}
	
	int HPWL_W=(net.pins[0].x>net.pins[1].x)?net.pins[0].x-net.pins[1].x:net.pins[1].x-net.pins[0].x;
	int HPWL_H=(net.pins[0].y>net.pins[1].y)?net.pins[0].y-net.pins[1].y:net.pins[1].y-net.pins[0].y;
	bool ifHadlock=(HPWL_W*HPWL_H<=110);
	//cout<<"ifHadlock:"<<ifHadlock<<endl;
	
	Pin start;
	Pin end;
	
	if(net.pins[0].x==net.pins[1].x){
		if(net.pins[0].y<=net.pins[1].y){
			start=net.pins[0];
			end=net.pins[1];
		}else{
			start=net.pins[1];
			end=net.pins[0];
		}
	}else{
		if(net.pins[0].x<net.pins[1].x){
			start=net.pins[0];
			end=net.pins[1];			
		}else{
			start=net.pins[1];
			end=net.pins[0];			
		}
	}
	
	//cout<<"Start x:"<<start.x<<" y:"<<start.y<<" | End x:"<<end.x<<" y:"<<end.y<<endl;
	if(start.x==end.x){
		PathNode current;
		current.x=start.x;
		current.y=start.y;
		for(;current.y<=end.y;current.y++){
			net.paths.push_back(current);
		}
		
		net.routed=true;
	}

	if(start.y==end.y){
		PathNode current;
		current.x=start.x;
		current.y=start.y;
		for(;current.x<=end.x;current.x++){
			net.paths.push_back(current);
		}
		
		net.routed=true;
	}
	
	if(start.x!=end.x && start.y!=end.y){
		//countruct map
		vector<vector<GraphNode>> graph;
		
		int Gwidth=end.x-start.x;
		int Gheight=end.y-start.y;

		//|Gwidth| and |Gheight|
		Gwidth=(Gwidth<0)?(Gwidth*-1)+1:Gwidth+1;
		Gheight=(Gheight<0)?(Gheight*-1)+1:Gheight+1;
		//cout<<"Gwidth:"<<Gwidth<<" Gheight:"<<Gheight<<endl;
		
		//cout<<"Gwidth:"<<Gwidth<<" Gheight:"<<Gheight<<endl;
		
		//inital graph
		graph.resize(Gwidth);
		for(int i=0;i<graph.size();i++){
			graph[i].resize(Gheight);
		}
		if(ifHadlock){
			if(start.y<end.y){ //left-down to right-up
				
				int currentY=start.y;
				for(int y=0;y<Gheight;y++){
					int currentX=start.x;
					for(int x=0;x<Gwidth;x++){
						graph[x][y].x=currentX;
						graph[x][y].y=currentY;
						
						
						graph[x][y].down.weight=(y==0)?0:1+grids[currentX][currentY].down.capacity;
						graph[x][y].up.weight=(y==(Gheight-1))?0:1+grids[currentX][currentY].up.capacity;
						graph[x][y].left.weight=(x==0)?0:1+grids[currentX][currentY].left.capacity;
						graph[x][y].right.weight=(x==(Gwidth-1))?0:1+grids[currentX][currentY].right.capacity;
						
						currentX++;
					}
					currentY++;
				}
				
				PathNode graphEnd;
				graphEnd.x=end.x-start.x;
				graphEnd.y=end.y-start.y;
				
				//cout<<"Graph end x:"<<graphEnd.x<<" y:"<<graphEnd.y<<endl;
				
				GraphPaths Gpaths;
				//GraphPaths path;
				//Gpaths.push_back(path);
				
				PathNode current;
				int initWeight=0;
				current.x=0;
				current.y=0;
				Gpaths.path.push_back(current);
				Gpaths.Weight=initWeight;
						
				
				priority_queue<GraphPaths, vector<GraphPaths>, cmp> pq;
				pq.push(Gpaths);
								

	/*			for (int y=Gheight-1;y>=0;y--){
					//cout<<i<<" ";
					for(int x=0;x<Gwidth;x++){
						if(graph[x][y].left.weight){
							cout<<" ";
						}					
						if(graph[x][y].up.weight){
							cout<<"|";
						}
						if(graph[x][y].right.weight){
							cout<<" ";
						}					
					}
					cout<<endl;
					for(int x=0;x<Gwidth;x++){
						if(graph[x][y].left.weight){
							cout<<"-";
						}
						cout<<"O";
						if(graph[x][y].right.weight){
							cout<<"-";
						}					
					}
					cout<<endl;
					for(int x=0;x<Gwidth;x++){
						if(graph[x][y].left.weight){
							cout<<" ";
						}					
						if(graph[x][y].down.weight){
							cout<<"|";
						}
						if(graph[x][y].right.weight){
							cout<<" ";
						}
					}
					cout<<endl;
				}			
	*/		
				
				
				//cout<<"top w:"<<pq.top().Weight<<" x:"<<pq.top().path.back().x<<" y:"<<pq.top().path.back().y<<endl;
				
				while(pq.top().path.back().x !=graphEnd.x || pq.top().path.back().y !=graphEnd.y ){
					GraphPaths topPath=pq.top();
					PathNode currentNode;
					currentNode.x=topPath.path.back().x;
					currentNode.y=topPath.path.back().y;
					pq.pop();
					const int onestep_weight=1;
					const int turn_weight=1;
					
	//				for(int i=0;i<topPath.path.size();i++){
	//					cout<<"("<<topPath.path[i].x<<","<<topPath.path[i].y<<")->"<<endl;
	//				}

					//walk up one step
					if(currentNode.y+1<=graphEnd.y){
						GraphPaths walkUp=topPath;
						PathNode next;
						
						next.x=currentNode.x;
						next.y=currentNode.y+1;
						
						
						
						walkUp.path.push_back(next);
						walkUp.Weight+=graph[next.x][next.y].down.weight;//+edge + turn right weight
						if(walkUp.path.size()>2){
							int last=walkUp.path.size()-1;
							int mid=walkUp.path.size()-2;
							int first=walkUp.path.size()-3;
							
							PathNode lastNode=walkUp.path[last];
							PathNode midNode=walkUp.path[mid];
							PathNode firstNode=walkUp.path[first];
							
							if(lastNode.x!=firstNode.x){
								walkUp.Weight+=turn_weight;
							}
							
						}
						
						
						pq.push(walkUp);					
					}

					//walk right one step
					if(currentNode.x+1<=graphEnd.x){
						GraphPaths walkRight=topPath;
						PathNode next;
						//cout<<"("<<next.x<<")1";
						next.x=currentNode.x+1;
						next.y=currentNode.y;
						
						walkRight.path.push_back(next);
						walkRight.Weight+=graph[next.x][next.y].left.weight;//+edge + turn right weight
						
						if(walkRight.path.size()>2){
							int last=walkRight.path.size()-1;
							int mid=walkRight.path.size()-2;
							int first=walkRight.path.size()-3;
							
							PathNode lastNode=walkRight.path[last];
							PathNode midNode=walkRight.path[mid];
							PathNode firstNode=walkRight.path[first];
							
							if(lastNode.y!=firstNode.y){
								walkRight.Weight+=turn_weight;
							}
							
						}					
						
						//cout<<" here0:"<<(walkRight.path[0].x)<<endl;
						pq.push(walkRight);
						//cout<<" here:"<<(pq.top().path[0].x)<<endl;
					}
					
					
					//break;
				}
				
				GraphPaths topPath=pq.top();
				net.routed=true;

				recoveryPath(net,topPath.path,start,end);
				
			}else{//left-up to right down
				
				int currentY=end.y;
				for(int y=0;y<Gheight;y++){
					int currentX=start.x;
					for(int x=0;x<Gwidth;x++){
						graph[x][y].x=currentX;
						graph[x][y].y=currentY;
						
						graph[x][y].down.weight=(y==0)?0:1+grids[currentX][currentY].down.capacity;
						graph[x][y].up.weight=(y==(Gheight-1))?0:1+grids[currentX][currentY].up.capacity;
						graph[x][y].left.weight=(x==0)?0:1+grids[currentX][currentY].left.capacity;
						graph[x][y].right.weight=(x==(Gwidth-1))?0:1+grids[currentX][currentY].right.capacity;					
						
						currentX++;
					}
					currentY++;
				}
		
				PathNode graphEnd;
				graphEnd.x=end.x-start.x;
				graphEnd.y=0;
				
				//cout<<"Graph end x:"<<graphEnd.x<<" y:"<<graphEnd.y<<endl;
				
				GraphPaths Gpaths;
				//GraphPaths path;
				//Gpaths.push_back(path);
				
				PathNode current;
				int initWeight=0;
				current.x=0;
				current.y=start.y-end.y;
				Gpaths.path.push_back(current);
				Gpaths.Weight=initWeight;
				
				//cout<<" start x:"<<start.x<<" y:"<<start.y<<endl;
				//cout<<" end x:"<<end.x<<" y:"<<end.y<<endl;
				//cout<<"Graph start x:"<<current.x<<" y:"<<current.y<<endl;
				//cout<<"Graph end x:"<<graphEnd.x<<" y:"<<graphEnd.y<<endl;
				
				priority_queue<GraphPaths, vector<GraphPaths>, cmp> pq;
				pq.push(Gpaths);			

				while(pq.top().path.back().x !=graphEnd.x || pq.top().path.back().y !=graphEnd.y ){
					GraphPaths topPath=pq.top();
					PathNode currentNode;
					currentNode.x=topPath.path.back().x;
					currentNode.y=topPath.path.back().y;
					pq.pop();
					const int onestep_weight=1;
					const int turn_weight=1;
					
	//				for(int i=0;i<topPath.path.size();i++){
	//					cout<<"("<<topPath.path[i].x<<","<<topPath.path[i].y<<")->"<<endl;
	//				}

					//walk down one step
					if(currentNode.y-1>=graphEnd.y){
						GraphPaths walkDown=topPath;
						PathNode next;
						
						next.x=currentNode.x;
						next.y=currentNode.y-1;
						
						
						
						walkDown.path.push_back(next);
						walkDown.Weight+=graph[next.x][next.y].up.weight;//+edge + turn right weight
						if(walkDown.path.size()>2){
							int last=walkDown.path.size()-1;
							int mid=walkDown.path.size()-2;
							int first=walkDown.path.size()-3;
							
							PathNode lastNode=walkDown.path[last];
							PathNode midNode=walkDown.path[mid];
							PathNode firstNode=walkDown.path[first];
							
							if(lastNode.x!=firstNode.x){
								walkDown.Weight+=turn_weight;
							}
							
						}
						
						
						pq.push(walkDown);					
					}

					//walk right one step
					if(currentNode.x+1<=graphEnd.x){
						GraphPaths walkRight=topPath;
						PathNode next;
						//cout<<"("<<next.x<<")1";
						next.x=currentNode.x+1;
						next.y=currentNode.y;
						
						walkRight.path.push_back(next);
						walkRight.Weight+=graph[next.x][next.y].left.weight;//+edge + turn right weight
						
						if(walkRight.path.size()>2){
							int last=walkRight.path.size()-1;
							int mid=walkRight.path.size()-2;
							int first=walkRight.path.size()-3;
							
							PathNode lastNode=walkRight.path[last];
							PathNode midNode=walkRight.path[mid];
							PathNode firstNode=walkRight.path[first];
							
							if(lastNode.y!=firstNode.y){
								walkRight.Weight+=turn_weight;
							}
							
						}					
						
						//cout<<" here0:"<<(walkRight.path[0].x)<<endl;
						pq.push(walkRight);
						//cout<<" here:"<<(pq.top().path[0].x)<<endl;
					}
					
					
					//break;
				}
				
				GraphPaths topPath=pq.top();
				net.routed=true;
				
				recoveryPath(net,topPath.path,start,end);
			}

		}else{
			if(start.y<end.y){//left-down to right-up
				
				int currentY=start.y;
				for(int y=0;y<Gheight;y++){
					int currentX=start.x;
					for(int x=0;x<Gwidth;x++){
						graph[x][y].x=currentX;
						graph[x][y].y=currentY;
						
						
						graph[x][y].down.weight=(y==0)?0:1+grids[currentX][currentY].down.capacity;
						graph[x][y].up.weight=(y==(Gheight-1))?0:1+grids[currentX][currentY].up.capacity;
						graph[x][y].left.weight=(x==0)?0:1+grids[currentX][currentY].left.capacity;
						graph[x][y].right.weight=(x==(Gwidth-1))?0:1+grids[currentX][currentY].right.capacity;
						
						currentX++;
					}
					currentY++;
				}				
				PathNode graphEnd;
				graphEnd.x=end.x-start.x;
				graphEnd.y=end.y-start.y;
				
				//cout<<"Graph end x:"<<graphEnd.x<<" y:"<<graphEnd.y<<endl;
				
				GraphPaths Gpaths;
				//GraphPaths path;
				//Gpaths.push_back(path);
				
				PathNode current;
				int initWeight=0;
				current.x=0;
				current.y=0;
				Gpaths.path.push_back(current);
				Gpaths.Weight=initWeight;
						
				
				
				while(Gpaths.path.back().x!=graphEnd.x || Gpaths.path.back().y!=graphEnd.y){
					PathNode backNode=Gpaths.path.back();
					int rightEdgeWeight=graph[backNode.x][backNode.y].right.weight;
					int upEdgeWeight=graph[backNode.x][backNode.y].up.weight;
					//PathNode currentNode;
					//currentNode.x=topPath.path.back().x;
					//currentNode.y=topPath.path.back().y;
					//pq.pop();
					
					if(backNode.x+1<=graphEnd.x && backNode.y+1<=graphEnd.y){
						// if next step do not reach boundary 
						// ck right and up weight to go 
						if(rightEdgeWeight<upEdgeWeight){//turn right
							PathNode nextStep=backNode;
							nextStep.x+=1;
							
							Gpaths.path.push_back(nextStep);
							Gpaths.Weight+=rightEdgeWeight;
							
						}else if(rightEdgeWeight>upEdgeWeight){//turn up
							PathNode nextStep=backNode;
							nextStep.y+=1;						

							Gpaths.path.push_back(nextStep);
							Gpaths.Weight+=upEdgeWeight;					
						}else {
							//right and up weight eq
							if((graphEnd.x-backNode.x)>=(graphEnd.y-backNode.y)){
								//go right
								PathNode nextStep=backNode;
								nextStep.x+=1;
								
								Gpaths.path.push_back(nextStep);
								Gpaths.Weight+=rightEdgeWeight;								
							}else{
								PathNode nextStep=backNode;
								nextStep.y+=1;						

								Gpaths.path.push_back(nextStep);
								Gpaths.Weight+=upEdgeWeight;								
							}
							
						}
						
						
					}else if(backNode.x+1>graphEnd.x && backNode.y+1<=graphEnd.y){
						//right reach boundary then go up
						PathNode nextStep=backNode;
						nextStep.y+=1;						

						Gpaths.path.push_back(nextStep);
						Gpaths.Weight+=upEdgeWeight;						
					}else if(backNode.x+1<=graphEnd.x && backNode.y+1>graphEnd.y){
						//right reach up then go right
						PathNode nextStep=backNode;
						nextStep.x+=1;
						
						Gpaths.path.push_back(nextStep);
						Gpaths.Weight+=rightEdgeWeight;							
					}
					
					

					
					//cout<<Gpaths.path.back().x<<" "<<Gpaths.path.back().y;
					
					//break;
				}
				net.routed=true;
				
				recoveryPath(net,Gpaths.path,start,end);					
				
			}else{//left-up to right-down
				
				int currentY=end.y;
				for(int y=0;y<Gheight;y++){
					int currentX=start.x;
					for(int x=0;x<Gwidth;x++){
						graph[x][y].x=currentX;
						graph[x][y].y=currentY;
						
						graph[x][y].down.weight=(y==0)?0:1+grids[currentX][currentY].down.capacity;
						graph[x][y].up.weight=(y==(Gheight-1))?0:1+grids[currentX][currentY].up.capacity;
						graph[x][y].left.weight=(x==0)?0:1+grids[currentX][currentY].left.capacity;
						graph[x][y].right.weight=(x==(Gwidth-1))?0:1+grids[currentX][currentY].right.capacity;					
						
						currentX++;
					}
					currentY++;
				}

				PathNode graphEnd;
				graphEnd.x=end.x-start.x;
				graphEnd.y=0;
				
				//cout<<"Graph end x:"<<graphEnd.x<<" y:"<<graphEnd.y<<endl;
				
				GraphPaths Gpaths;
				//GraphPaths path;
				//Gpaths.push_back(path);
				
				PathNode current;
				int initWeight=0;
				current.x=0;
				current.y=start.y-end.y;
				Gpaths.path.push_back(current);
				Gpaths.Weight=initWeight;
				
				while(Gpaths.path.back().x!=graphEnd.x || Gpaths.path.back().y!=graphEnd.y){
					PathNode backNode=Gpaths.path.back();
					
					
					int rightEdgeWeight=graph[backNode.x][backNode.y].right.weight;
					int downEdgeWeight=graph[backNode.x][backNode.y].down.weight;
					

					if(backNode.x+1<=graphEnd.x && backNode.y-1>=graphEnd.y){
						
						if(rightEdgeWeight<downEdgeWeight){//turn right
							PathNode nextStep=backNode;
							nextStep.x+=1;
							
							Gpaths.path.push_back(nextStep);
							Gpaths.Weight+=rightEdgeWeight;
							
						}else if(rightEdgeWeight>downEdgeWeight){//turn down
							PathNode nextStep=backNode;
							nextStep.y-=1;						

							Gpaths.path.push_back(nextStep);
							Gpaths.Weight+=downEdgeWeight;					
						}else {
							//right and down weight eq
							if((graphEnd.x-backNode.x)>=(backNode.y-graphEnd.y)){
								//go right
								PathNode nextStep=backNode;
								nextStep.x+=1;
								
								Gpaths.path.push_back(nextStep);
								Gpaths.Weight+=rightEdgeWeight;								
							}else{
								PathNode nextStep=backNode;
								nextStep.y-=1;						

								Gpaths.path.push_back(nextStep);
								Gpaths.Weight+=downEdgeWeight;								
							}
							
						}						
					}else if(backNode.x+1>graphEnd.x && backNode.y-1>=graphEnd.y){
						PathNode nextStep=backNode;
						nextStep.y-=1;						

						Gpaths.path.push_back(nextStep);
						Gpaths.Weight+=downEdgeWeight;						
						
						
					}else if(backNode.x+1<=graphEnd.x && backNode.y-1<graphEnd.y){
						PathNode nextStep=backNode;
						nextStep.x+=1;
						
						Gpaths.path.push_back(nextStep);
						Gpaths.Weight+=rightEdgeWeight;							
					}
					
				}
				net.routed=true;
				recoveryPath(net,Gpaths.path,start,end);				
			}
		}
		
		
	}
	/*
	for(int i=0;i<net.paths.size();i++){
		cout<<"x:"<<net.paths[i].x<<" y:"<<net.paths[i].y<<endl;
	}*/
	
}
void GlobalRouting::output(char* filename){
	
	FILE *fptr;
	//cout<<"filename:"<<filename<<endl;
	fptr = fopen(filename, "w");
	
	for(int i=0;i<nets.size();i++){
		if(nets[i].paths[0].x!=nets[i].pins[0].x || nets[i].paths[0].y!=nets[i].pins[0].y)
			reverse(nets[i].paths.begin(), nets[i].paths.end());
		
		fprintf(fptr,"net%d %d\n",i,i);
		//if(i==2){
		PathNode current=nets[i].paths[0];
		//print current
		//cout<<"("<<current.x<<", "<<current.y<<", 1)";
		fprintf(fptr,"(%d, %d, 1)",current.x,current.y);		
		for(int j=2;j<nets[i].paths.size();j++){
			if(nets[i].paths[j].x!=nets[i].paths[j-2].x && nets[i].paths[j].y!=nets[i].paths[j-2].y){
				current=nets[i].paths[j-1];
				//print
				//cout<<"-("<<current.x<<", "<<current.y<<", 1)"<<endl;
				fprintf(fptr,"(-%d, %d, 1)\n",current.x,current.y);
				
				//cout<<"("<<current.x<<", "<<current.y<<", 1)";
				fprintf(fptr,"(%d, %d, 1)",current.x,current.y);
			}
			
		}
		current=nets[i].paths.back();
		//cout<<"-("<<current.x<<", "<<current.y<<", 1)"<<endl<<"!"<<endl;
		
		fprintf(fptr,"-(%d, %d, 1)\n!\n",current.x,current.y);

	}


	fclose(fptr);	
}
void GlobalRouting::Routing(){
	
	//routing order
	vector<int> routing_order(nets.size());
	
	clock_t start = clock();
	
	
	priority_queue<Net, vector<Net>, cmp2> pq;
	for(int i=0;i<nets.size();i++){
		pq.push(nets[i]);
	}
	int routing_order_i=0;
	while(!pq.empty()){
		routing_order[routing_order_i]=pq.top().id;
		
		routing_order_i++;
		pq.pop();
	}
	int t=routing_order.back();
	cout<<"top"<<endl;
	cout<<nets[t].pins[0].x<<" "<<nets[t].pins[0].y<<endl;
	cout<<nets[t].pins[1].x<<" "<<nets[t].pins[1].y<<endl;
	cout<<"top"<<endl;
	
	reverse(routing_order.begin(),routing_order.end());

	clock_t end = clock();
	cout << "net ordering Runtime : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";		
	
	start = clock();
	//construct a sub graph to rout the net
	//for(int i=0;i<15000;i<i++){
	//	cout<<i<<endl;
	int count=0;
	//nets[2].pins[0].x=0;nets[2].pins[0].y=0;
	//nets[2].pins[1].x=10;nets[2].pins[1].y=20;
	
	for(int i=0;i<routing_order.size();i++){
		RoutNet(nets[routing_order[i]]);
		if(nets[routing_order[i]].routed)count++;
	}
	cout<<"count:"<<count<<endl;
	 end = clock();
	cout << "sa Runtime : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";	
}