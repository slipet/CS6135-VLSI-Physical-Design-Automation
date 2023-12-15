#include"linked_list.h"

vector<string> split(string strToSplit, char delimeter)
{
    stringstream ss(strToSplit);
    string item;
    vector<string> splittedStrings;
    while (getline(ss, item, delimeter))
    {
       splittedStrings.push_back(item);
    }
    return splittedStrings;
}

string& trim(string &s) 
{
    if (s.empty()) 
    {
        return s;
    }
    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}



void read_cells(char *filename,map<int,pair<int,int>> &cells,vector<int> &size_list){
	//string cell_name;
    clock_t start = clock();
	int total=0;
	
	string line;
	vector <string> cell;
	ifstream cellsfile (filename);
	//cout<<cell_name<<endl;
	int index;
	int cell_size;
	int count=0;
	int max_cell_size=0;
	if (cellsfile.is_open()){
		
		while ( getline (cellsfile,line) ){
			cell = split(line,' ');
	//cout<<count++<<" ";
			cell[0].erase(0,1);
			index = stoi(cell[0],nullptr,10);
			cell_size = stoi(cell[1],nullptr,10);
			
			cells[index].first=cell_size;
			cells[index].second=0;
			if(max_cell_size<cell_size)
				max_cell_size=cell_size;
			
			total+=cell_size;
		}
		cellsfile.close();
	}
	else cout << "Unable to open file"; 
	
	map<int,pair<int,int>>::iterator iter;
	size_list.resize(max_cell_size+1);
	
	
	for(iter = cells.begin(); iter != cells.end(); iter++){
		//cout<<(iter->second).first<<endl;
		size_list[(iter->second).first]++;
		
	}
	
	cout<<"cells size:"<<total<<endl;
  clock_t end = clock();
	cout << "cell i/o time : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";	  
}

void read_nets(char *filename,vector<vector<int>> &nets,map<int,pair<int,int>> &cells){
	string line;
	int net_size;	
	ifstream netsfile (filename);
    clock_t start = clock();
	vector <string> net;
	int index ;
	int cell;
	vector <string> tmp;
	vector<int> tmp_net;
	if(netsfile.is_open())
	{
		while (getline(netsfile,line,' '))
		{

			if((line[0])=='n'){
				line.erase(0,1);
				index = stoi(line,nullptr,10);
				tmp_net.clear();

			}

			if((line[0])=='c'){
				line.erase(0,1);
				cell = stoi(line,nullptr,10);

				tmp_net.push_back(cell);
				
				cells[cell].second+=1;

			}
			if((line[0])=='}'){
				nets.push_back(tmp_net);
			}
			
		}
		netsfile.close();

	}
	else cout << "Unable to open file";
	
	cout<<"nets size:"<<nets.size()<<endl;
    clock_t end = clock();
    cout << "net i/o time : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n"; 	
}

void output_cell_net(vector<vector<int>> &nets,map<int,pair<int,int>> &cells){
	map<int,pair<int,int>>::iterator iter;
	for(iter = cells.begin(); iter != cells.end(); iter++){
		cout<<"cell: "<<(*iter).first<<" size: "<<(iter->second).first<<" pin: "<<(iter->second).second<<endl;
	}

	for(int i=0;i<nets.size();i++){
		cout<<"net: "<<i+1<<" ";
		for(int j=0;j<nets[i].size();j++){
			cout<<nets[i][j]<<" ";
		}
		cout<<endl;
	}		
}
int total_cells_size(map<int,pair<int,int>> &cells){
	int total=0;
	map<int,pair<int,int>>::iterator iter;
	
	for(iter = cells.begin(); iter != cells.end(); iter++){

		total+=((iter->second).first);
	}
	return total;
}
int bucket_list_islegal(int sumA,int sumB,int total_size){
	
	float conditon = sumA-sumB;
	float constraint=(total_size/10);
	if(conditon<0)conditon*=-1;
	
	if(conditon<constraint){
		return 1;
	}
	return 0;	
}
//read binary file with buffer

int size_of_buctket_list(map<int,int> &state_of_cell,map<int,pair<int,int>> &cells,int A_or_B){
	int W=0;
	map<int,int>::iterator iter;
	
	for(iter = state_of_cell.begin(); iter != state_of_cell.end(); iter++){
		if((iter->second)==A_or_B){
			W+=cells[(iter->first)].first;
		}
	}
	//cout<<"sum"<<W<<endl;
	return W;
}
int constraint(int sumA,int sumB){
	float A=sumA*1.0;
	float B=sumB*1.0;
	float lconstraint =2.2;
	float rconstraint =12.8;
	cout<<"sum:"<<sumA<<" "<<sumB<<endl;
	
	if((lconstraint<=sumA&&sumA<=rconstraint)||(lconstraint<=sumB&&sumB<=rconstraint))
		return 1;
	
	return 0;
}
void init_AB_bucket_list(map<int,pair<int,int>> &cells,map<int,int> &state_of_cell,int input_seed){
	int seed=time(NULL);
	srand(input_seed);
	
	cout<<"seed: "<<seed<<endl;
	int sumA=0,sumB=0;
	int partition;
	int total_size=total_cells_size(cells);
	map<int,pair<int,int>>::iterator iter;
	
	for(iter = cells.begin(); iter != cells.end(); iter++){
		//cout<<"cell: "<<(*iter).first<<" size: "<<(iter->second).first<<" pin: "<<(iter->second).second<<endl;
		if((rand()%2) == 1)
		{
			partition = 1;
		}
		else
		{
			partition = -1;
		}
		if(partition==1){
			if(bucket_list_islegal(sumA+((iter->second).first),sumB,total_size)==1){
				sumA+=((iter->second).first);
				state_of_cell[(*iter).first]=partition;
			}
			else{
				sumB+=((iter->second).first);
				state_of_cell[(*iter).first]=partition*-1;				
			}
		}
		else if(partition==-1){
			if(bucket_list_islegal(sumA,sumB+((iter->second).first),total_size)==1){
				sumB+=((iter->second).first);
				state_of_cell[(*iter).first]=partition;
			}
			else{
				sumA+=((iter->second).first);
				state_of_cell[(*iter).first]=partition*-1;				
			}	
		}
		
	}

}

void init_AB_bucket_list_pin_max(map<int,pair<int,int>> &cells,map<int,int> &state_of_cell,int pin_max){
	map<int,vector<int>> cell_sort_by_pin;
	map<int,pair<int,int>>::iterator iter;
	int sumA=0,sumB=0;
	int partition=1;
	int total_size=total_cells_size(cells);	
	
	for(int i=pin_max;i>0;i--){
		
		for(iter=cells.begin();iter!=cells.end();iter++){
			if((iter->second).second==i){
				cell_sort_by_pin[i].push_back(iter->first);
			}
		}
	}
	map<int,vector<int>>::iterator iter2;
	for(iter2=cell_sort_by_pin.begin();iter2!=cell_sort_by_pin.end();iter2++){
		for(int i=0;i<(iter2->second).size();i++){
			int cell=(iter2->second)[i];
			partition*=-1;
			if(partition==1){
				if(bucket_list_islegal(sumA+(cells[cell].first),sumB,total_size)==1){
					sumA+=(cells[cell].first);
					state_of_cell[cell]=partition;
				}
				else{
					sumB+=(cells[cell].first);
					state_of_cell[cell]=partition*-1;				
				}
			}
			else if(partition==-1){
				if(bucket_list_islegal(sumA,sumB+(cells[cell].first),total_size)==1){
					sumB+=(cells[cell].first);
					state_of_cell[cell]=partition;
				}
				else{
					sumA+=(cells[cell].first);
					state_of_cell[cell]=partition*-1;				
				}	
			}			
		}
	}
}

void init_cell_in_net(map<int,vector<int>> &cell_in_net,vector<vector<int>> &nets){
	
	for(int i=0;i<nets.size();i++){
		for(int j=0;j<nets[i].size();j++){
			int cell =nets[i].at(j);
			
			cell_in_net[cell].push_back(i);
		}
	}

}

void init_cell_num_of_net_in_AB(map<int,map<int,int>> &cell_num_of_net_in_AB, vector<vector<int>> &nets, map<int,int> &state_of_cell){
	int cell=-1;
	cell_num_of_net_in_AB[1].clear();
	cell_num_of_net_in_AB[-1].clear();
	for(int i=0;i<nets.size();i++){
		for(int j=0;j<nets[i].size();j++){
			
			cell=nets[i][j];
			if(state_of_cell[cell]==1){
				cell_num_of_net_in_AB[1][i]+=1;
			}
			else if(state_of_cell[cell]==-1){
				cell_num_of_net_in_AB[-1][i]+=1;
			}
			else{
				cout<<"cell: "<<cell<<" state: "<<state_of_cell[cell]<<" is not legal num."<<endl;
			}
			
		}
	}
	

}

void init_gain(map<int,int> &gain_of_cell,map<int,map<int,int>> &cell_num_of_net_in_AB,vector<vector<int>> &nets,map<int,vector<int>> &cell_in_net,map<int,int> &state_of_cell){
	gain_of_cell.clear();
	map<int,vector<int>>::iterator iter;
	int cell;
	for(iter=cell_in_net.begin();iter!=cell_in_net.end();iter++){
		cell = (iter->first);
		//cout<<"cell:"<<cell<<" ";
		for(int i=0;i<(iter->second).size();i++){

			int net = (iter->second).at(i);
			int F=0;
			int T=0;
			//cell in A
			if(state_of_cell[cell]==1){

				F=cell_num_of_net_in_AB[1][net];
				T=cell_num_of_net_in_AB[-1][net];
				//cout<<" F:"<<F<<" T:"<<T;
			}
			//cell in B
			else if(state_of_cell[cell]==-1){
				//T=cells_num_of_net_in_A[net];
				//F=cells_num_of_net_in_B[net];		
				T=cell_num_of_net_in_AB[1][net];
				F=cell_num_of_net_in_AB[-1][net];					
				//cout<<" F:"<<F<<" T:"<<T;
			}
		
			if(F==1)
				gain_of_cell[cell]+=1;
			if(T==0)
				gain_of_cell[cell]-=1;
		
			//cout<<"   ";
		}
		
	}

	
}

void init_locked_cell(map<int,int> &locked_cell, map<int,int> &state_of_cell){
	
	map<int,int>::iterator iter;
	
	for(iter=state_of_cell.begin();iter!=state_of_cell.end();iter++){
		//cout<<"cell:"<<(iter->first)<<" "<<"state:"<<(iter->second)<<endl;
		locked_cell[(iter->first)]=0;
	}
	
}

int getPinMax(map<int,pair<int,int>> &cells){
	int pin_max=0;
	map<int,pair<int,int>>::iterator iter;
	
	for(iter=cells.begin();iter!=cells.end();iter++){
		//cout<<"cell:"<<(iter->first)<<" "<<"pin:"<<(iter->second).second<<endl;
		if(((iter->second).second)>pin_max)
			pin_max=(iter->second).second;
	}
	
	return pin_max;
}

void reallocate_AB_list(map<int,Doubly_Linked_List> &A_bucket_list,map<int,Doubly_Linked_List> &B_bucket_list,map<int,node*> &addr_cell_in_bucket_list,map<int,int> &state_of_cell,map<int,int> &gain_of_cell,int &pin_max){
	
	map<int,int>::iterator iter;
	A_bucket_list.clear();
	B_bucket_list.clear();
	

	A_bucket_list[0];
	B_bucket_list[0];
	for(int i=1;i<=pin_max;i++){
		A_bucket_list[i];
		A_bucket_list[i*-1];
		B_bucket_list[i];
		B_bucket_list[i*-1];		
	}
	
	for(iter=state_of_cell.begin();iter!=state_of_cell.end();iter++){
		if(state_of_cell[(iter->first)]==1){
			
			A_bucket_list[gain_of_cell[(iter->first)]].add_end((iter->first));
			
			addr_cell_in_bucket_list[(iter->first)]=A_bucket_list[gain_of_cell[(iter->first)]].getend();

		}
		else if(state_of_cell[(iter->first)]==-1){
			B_bucket_list[gain_of_cell[(iter->first)]].add_end((iter->first));
			//addr_of_cell_in_bucketlist[(iter->first)]=&(*(B_bucket_list[gain_of_cell[(iter->first)]].rbegin()));
			addr_cell_in_bucket_list[(iter->first)]=B_bucket_list[gain_of_cell[(iter->first)]].getend();
		}
	}

}

int  get_cut_size(vector<vector<int>> &nets,map<int,int> &state_of_cell){
	
	int cut_size=0;
	for(int i=0;i<nets.size();i++){
		int state=state_of_cell[nets[i].at(0)];
		
		for(int j=1;j<nets[i].size();j++){
			
			if(state_of_cell[nets[i].at(j)]==state*-1){
				cut_size++;
				break;
			}
		}
	}

	return cut_size;
}


void update_bucket_list(map<int,Doubly_Linked_List> &bucket_list,map<int,int> &gain_of_cell,map<int,node*> &addr_cell_in_bucket_list,int pre_gain,int cell){
	

	int cell_gain=gain_of_cell[cell];

	

	bucket_list[pre_gain].delete_node(addr_cell_in_bucket_list[cell]);

	bucket_list[cell_gain].add_end(cell);
	addr_cell_in_bucket_list[cell]=bucket_list[cell_gain].getend();
}

void update_gain(vector<vector<int>> &nets,map<int,Doubly_Linked_List> &A_bucket_list,map<int,Doubly_Linked_List> &B_bucket_list,\
					map<int,int> &gain_of_cell,map<int,node*> &addr_cell_in_bucket_list,map<int,map<int,int>> &cell_num_of_net_in_AB,\
					map<int,int> &locked_cell,map<int,vector<int>> &cell_in_net,map<int,int> &state_of_cell,int cell){

	locked_cell[cell]=1;	
	int controll=0;
	int cell_orignial_state=state_of_cell[cell];

	for(int index=0;index<cell_in_net[cell].size();index++){
		//before move
		//cell in A
		int F;
		int T;
		//cout<<endl;
		
		
		int net=cell_in_net[cell][index];
		//cout<<" net:"<<net<<" ";
		if(cell_orignial_state==1){
			F=cell_num_of_net_in_AB[1][net];
			T=cell_num_of_net_in_AB[-1][net];
		}//cell in B
		else if(cell_orignial_state==-1){
			F=cell_num_of_net_in_AB[-1][net];
			T=cell_num_of_net_in_AB[1][net];			
		}
		
		//cout<<"F:"<<F<<" T:"<<T;
		if(T==0){
			//gain_of_cell[cell]+=2;
			for(int i=0;i<nets[net].size();i++){
				if(locked_cell[nets[net].at(i)]!=1){ 
					int pre_gain=gain_of_cell[nets[net].at(i)];
					gain_of_cell[nets[net].at(i)]++;
					if(state_of_cell[nets[net].at(i)]==1){
						update_bucket_list(A_bucket_list,gain_of_cell,addr_cell_in_bucket_list,pre_gain,nets[net].at(i));
					}
					else if(state_of_cell[nets[net].at(i)]==-1){
						update_bucket_list(B_bucket_list,gain_of_cell,addr_cell_in_bucket_list,pre_gain,nets[net].at(i));
					}
				}
			}
		}
		else if(T==1){
			//cout<<"||||";
			for(int i=0;i<nets[net].size();i++){
				if(locked_cell[nets[net].at(i)]!=1){
					
					//decrease gain of cell in to block 
					if(state_of_cell[nets[net].at(i)]==(cell_orignial_state*-1)){
						
						int pre_gain=gain_of_cell[nets[net].at(i)];
						gain_of_cell[nets[net].at(i)]--;
					
						if(state_of_cell[nets[net].at(i)]==1){
							update_bucket_list(A_bucket_list,gain_of_cell,addr_cell_in_bucket_list,pre_gain,nets[net].at(i));
						}
						else if(state_of_cell[nets[net].at(i)]==-1){
							update_bucket_list(B_bucket_list,gain_of_cell,addr_cell_in_bucket_list,pre_gain,nets[net].at(i));
						}
					}
				}
			}
			//cout<<endl;
		}
		//move
		F--;T++;
		//move cell
		//change a b list
		if(cell_orignial_state==1){
			cell_num_of_net_in_AB[1][net]--;
			cell_num_of_net_in_AB[-1][net]++;
		}
		else if(cell_orignial_state==-1){
			cell_num_of_net_in_AB[1][net]++;
			cell_num_of_net_in_AB[-1][net]--;			
		}

		state_of_cell[cell]*=-1;

		//cout<<" | F':"<<F<<" T':"<<T;
		//check critical nets after the move
		if(F==0){
			//gain_of_cell[cell]-=2;
			for(int i=0;i<nets[net].size();i++){
				if(locked_cell[nets[net].at(i)]!=1){
					//cout<<"[[[[";
					int pre_gain=gain_of_cell[nets[net].at(i)];
					gain_of_cell[nets[net].at(i)]--;
					if(state_of_cell[nets[net].at(i)]==1){
						update_bucket_list(A_bucket_list,gain_of_cell,addr_cell_in_bucket_list,pre_gain,nets[net].at(i));
					}
					else if(state_of_cell[nets[net].at(i)]==-1){
						update_bucket_list(B_bucket_list,gain_of_cell,addr_cell_in_bucket_list,pre_gain,nets[net].at(i));
					}					
				}
			}
		}
		else if(F==1){
			for(int i=0;i<nets[net].size();i++){
				if(locked_cell[nets[net].at(i)]!=1){
					if(state_of_cell[nets[net].at(i)]==(state_of_cell[cell])*-1){
						int pre_gain=gain_of_cell[nets[net].at(i)];
						gain_of_cell[nets[net].at(i)]++;
						if(state_of_cell[nets[net].at(i)]==1){
							update_bucket_list(A_bucket_list,gain_of_cell,addr_cell_in_bucket_list,pre_gain,nets[net].at(i));
						}
						else if(state_of_cell[nets[net].at(i)]==-1){
							update_bucket_list(B_bucket_list,gain_of_cell,addr_cell_in_bucket_list,pre_gain,nets[net].at(i));
						}
					
					}
				}
			}			
		}		
	}

}





int move_celli(vector<vector<int>> &nets,map<int,Doubly_Linked_List> &A_bucket_list,map<int,Doubly_Linked_List> &B_bucket_list,\
				map<int,pair<int,int>> &cells,map<int,map<int,int>> &cell_num_of_net_in_AB,map<int,int> &gain_of_cell,map<int,node*> &addr_cell_in_bucket_list,\
				map<int,int> &locked_cell ,map<int,int> &state_of_cell,map<int,vector<int>> &cell_in_net,vector<int> &size_list,int pin_max){
	
	int partial_sum=0;
	int total_cells_size= size_of_buctket_list(state_of_cell,cells,1) + size_of_buctket_list(state_of_cell,cells,-1);
	int max_cell_size=0;
	int cut_size=getPinMax(cells);

	map<int,int> save_state_of_cell;
	map<int,int> before_move_state_of_cell=state_of_cell;

	vector<int> move;

	int count=0;
	int step=0;
	int max_partial_sum_step=-1;
	
	int sizeA=size_of_buctket_list(state_of_cell,cells,1);
	int sizeB=size_of_buctket_list(state_of_cell,cells,-1);
	int cell_num=cells.size();
	int max_partial_sum=0;
	int controll=0;
	
		//clock_t start = clock();
		save_state_of_cell=state_of_cell;
		int max;
		step=0;
		cell_num=cells.size();
		max_partial_sum=0;
		clock_t sum;
	clock_t start,start1;
	clock_t end,end1;
	//cout << "core time : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";
	start1=clock();
		partial_sum=0;
	while(cell_num>0){

		
			
			
			
		
				
				int cell=0;
				
				//cout<<"here2"<<endl;
				
				controll=0;
				int Amax=0;int Bmax=0;
				for(Amax=pin_max;Amax>=pin_max*-1;Amax--){
					//cout<<"here1:"<<A_bucket_list[Amax].size()<<endl;
					if(A_bucket_list[Amax].size()!=0){
						break;
					}
				}
				for(Bmax=pin_max;Bmax>=pin_max*-1;Bmax--){
					//cout<<"here2:"<<Bmax<<" "<<B_bucket_list[Bmax].size()<<endl;
					if(B_bucket_list[Bmax].size()!=0){
						break;
					}
				}
				//cout<<"Amax:"<<Amax<<" Bmax:"<<Bmax<<endl;
				//cout<<"here max:"<<Amax<<" "<<pin_max<<" "<<(A_bucket_list[Amax].getfront()==NULL)<<endl;
				if(Amax>=Bmax){
					
					
					cell = ((A_bucket_list[Amax].getfront())->data);
					if(locked_cell[cell]!=1){
						if(bucket_list_islegal(sizeA-(cells[cell].first),sizeB+(cells[cell].first),total_cells_size)==1){
							controll=1;
						}else{
							cell = ((B_bucket_list[Bmax].getfront())->data);
							if(locked_cell[cell]!=1){
								if(bucket_list_islegal(sizeA+(cells[cell].first),sizeB-(cells[cell].first),total_cells_size)==1){
									controll=-1;
								}else{
									controll=0;
								}
							}
						}
					}
				}else{
					//cout<<"here3:"<<endl;
					cell = ((B_bucket_list[Bmax].getfront())->data);
					
					//cout<<"here:"<<Bmax<<"cell:"<<cell<<locked_cell[cell]<<" "<<constraint(sizeA+(cells[cell].first),sizeB-(cells[cell].first))<<endl;
					if(locked_cell[cell]!=1){
						if(bucket_list_islegal(sizeA+(cells[cell].first),sizeB-(cells[cell].first),total_cells_size)==1){
							
							controll=-1;
						}else{
							cell = ((A_bucket_list[Amax].getfront())->data);
							if(locked_cell[cell]!=1){
								if(bucket_list_islegal(sizeA-(cells[cell].first),sizeB+(cells[cell].first),total_cells_size)==1){
									controll=1;
								}else{
									controll=0;
								}
							}
						}							
					}
				}
				//cout<<"here4:"<<endl;
				//cout<<"cell_num:"<<cell_num<<" max:"<<max<<" A:"<<A_bucket_list[max].size()<<" B:"<<A_bucket_list[max].size()<<endl;
				
				if(controll==1){
					cell = ((A_bucket_list[Amax].getfront())->data);
					

					partial_sum+=gain_of_cell[cell];

					update_gain(nets,A_bucket_list,B_bucket_list,gain_of_cell,addr_cell_in_bucket_list,cell_num_of_net_in_AB,\
								locked_cell,cell_in_net,state_of_cell,cell);

					//cout << "core time : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";	
					
					A_bucket_list[Amax].delete_node(A_bucket_list[Amax].getfront());
					addr_cell_in_bucket_list[cell]=NULL;

					if(max_partial_sum<partial_sum){

						max_partial_sum_step=step;
								//record step
						max_partial_sum=partial_sum;
					}
					step++;
					move.push_back(cell);
					cell_num--;
					sizeA-=(cells[cell].first);
					sizeB+=(cells[cell].first);
					
				}
				if(controll==-1){
					
					cell = ((B_bucket_list[Bmax].getfront())->data);
					
							//cout<<"here5:"<<endl;
					partial_sum+=gain_of_cell[cell];
					

					update_gain(nets,A_bucket_list,B_bucket_list,gain_of_cell,addr_cell_in_bucket_list,cell_num_of_net_in_AB,\
								locked_cell,cell_in_net,state_of_cell,cell);
					

					//cout << "core time : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";	
					
					B_bucket_list[Bmax].delete_node(B_bucket_list[Bmax].getfront());
					addr_cell_in_bucket_list[cell]=NULL;	
					//if(step<5)
					//cout<<"step:"<<step<<"partial_sum:"<<partial_sum<<"|"<<gain_of_cell[cell]<<endl;
					if(max_partial_sum<=partial_sum){
						//cout<<"in if |||step:"<<step<<"partial_sum:"<<partial_sum<<"|"<<gain_of_cell[cell]<<endl;
						max_partial_sum_step=step;
								//record step
						max_partial_sum=partial_sum;
					}
					step++;
					move.push_back(cell);
					cell_num--;
					sizeA+=(cells[cell].first);
					sizeB-=(cells[cell].first);
				
				}
		if(partial_sum<0)
			break;
		
		//cout<<"controll:"<<controll<<endl;

	}
	end1=clock();
	//cout << "iter time : " << ((double) end1 - start1) / CLOCKS_PER_SEC << "s\n";
	

		state_of_cell=save_state_of_cell;
		
		
		
		///move max_partial_sum step
		//cout<<" move:";
		if(max_partial_sum_step>=0){
		for(int i=0;i<=max_partial_sum_step;i++){
			int move_cell=move[i];
			//cout<<"|"<<move_cell<<"|";
			state_of_cell[move_cell]*=-1;
		}
		}
		//cout<<endl;
		
		save_state_of_cell=state_of_cell;
		//unlock
		
		//start = clock();
	start=clock();	
		init_cell_num_of_net_in_AB(cell_num_of_net_in_AB,nets,state_of_cell);
		init_locked_cell(locked_cell,state_of_cell);
		init_gain(gain_of_cell,cell_num_of_net_in_AB,nets,cell_in_net,state_of_cell);
		reallocate_AB_list(A_bucket_list,B_bucket_list,addr_cell_in_bucket_list,state_of_cell,gain_of_cell,pin_max);
	end=clock();
	
		//end = clock();
	//cout << "init untime : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";	


	
	return max_partial_sum;
}


void output(vector<vector<int>> &nets,map<int,int> &state_of_cell,char *filename){
	FILE *fptr; 
	fptr = fopen(filename, "w");
	fprintf(fptr,"cut_size %d\n",get_cut_size(nets,state_of_cell));
	
	int A=0,B=0;
	map<int,int>::iterator iter;
	
	for(iter=state_of_cell.begin();iter!=state_of_cell.end();iter++){
		if(state_of_cell[iter->first]==1)
			A++;
		if(state_of_cell[iter->first]==-1)
			B++;
	}
	fprintf(fptr,"A %d\n",A);	
	//compute_cut_size(nets,state_of_cell);

//reverse_hash_func(hash_list,h)
	
	for(iter=state_of_cell.begin();iter!=state_of_cell.end();iter++){
		//cout<<i<<" "<<hash_list[i]<<endl;

		if(state_of_cell[iter->first]==1){
				fprintf(fptr,"c%d\n",iter->first);

			state_of_cell[iter->first]=0;
		}
	}
	fprintf(fptr,"B %d\n",B);	

	for(iter=state_of_cell.begin();iter!=state_of_cell.end();iter++){
		//cout<<i<<" "<<hash_list[i]<<endl;

		if(state_of_cell[iter->first]==-1){
				fprintf(fptr,"c%d\n",iter->first);

			state_of_cell[iter->first]=0;
		}
	}	
	fclose(fptr);
}


int main(int argc,char **argv){
	
	clock_t start = clock();	
	//cells: <size,pins>
	map<int,pair<int,int>> cells;
	vector<vector<int>> nets;
	//
	vector<int> size_list;
	//1 in A ,-1 in B
	map<int,int> state_of_cell;
	
	//cell in net
	map<int,vector<int>> cell_in_net;
	
	//A to B; every net have F and T
	//vector<vector<int>> AB_From_block,AB_To_block;
	map<int,map<int,int>> cell_num_of_net_in_AB;
	//B to A
	//vector<int,vector<int>> BA_From_block,BA_To_block;
	
	//gain
	map<int,int> gain_of_cell;
	
	//
	map<int,int> locked_cell;
	
	vector<int> hash_list;
	
	map<int,Doubly_Linked_List> A_bucket_list,B_bucket_list;
	map<int,node*> addr_cell_in_bucket_list;
	//store move of cell;
	vector<int> move;
	
	vector<int > record_cell_move_times;
	
	int pin_max=0;
	
	float R=0.5;
	cout<<argv[2]<<endl;
	read_cells(argv[2],cells,size_list);
	//cout<<"stop"<<endl;
	//cell_cin_read_nosync(argv[1],cells,size_list,hash_list);
	//cout<<"cells size:"<<cells.size()<<"\n";

	read_nets(argv[1],nets,cells);
	init_AB_bucket_list(cells,state_of_cell,1605974737);//1605974506//1605974737
	//init_AB_bucket_list_pin_max(cells,state_of_cell, pin_max);
	
	cout<<"init bucket list A:"<<size_of_buctket_list(state_of_cell,cells,1)<<" B:"<<size_of_buctket_list(state_of_cell,cells,-1)<<endl;
	
	init_cell_in_net(cell_in_net,nets);
	init_cell_num_of_net_in_AB(cell_num_of_net_in_AB,nets,state_of_cell);
	init_gain(gain_of_cell,cell_num_of_net_in_AB,nets,cell_in_net,state_of_cell);
	
	init_locked_cell(locked_cell,state_of_cell);
	pin_max=getPinMax(cells);
	reallocate_AB_list(A_bucket_list,B_bucket_list,addr_cell_in_bucket_list,state_of_cell,gain_of_cell,pin_max);
	cout<<"init cut size:"<<get_cut_size(nets,state_of_cell)<<endl;
	
	int Gk;
	
	int i=0;
	if(get_cut_size(nets,state_of_cell)>100000){
		i=5;
	}else if(get_cut_size(nets,state_of_cell)>5000){
		i=10;
	}else if(get_cut_size(nets,state_of_cell)>1000){
		i=35;
		
	}else if(get_cut_size(nets,state_of_cell)>500){
		i=500;
	}else{
		i=2000;
	}
	Gk=1;
	for(;i>0;i--){
	Gk=move_celli(nets,A_bucket_list,B_bucket_list,\
				cells,cell_num_of_net_in_AB,gain_of_cell,addr_cell_in_bucket_list,\
				locked_cell ,state_of_cell,cell_in_net,size_list, pin_max);
		if(Gk<=0)
			break;
	}
	cout<<"init cut size:"<<get_cut_size(nets,state_of_cell)<<endl;


	output(nets,state_of_cell,argv[3]);
	

	
	clock_t end = clock();
	cout << "Runtime : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";	
	return 0;
}