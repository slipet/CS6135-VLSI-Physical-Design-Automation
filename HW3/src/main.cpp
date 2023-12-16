#include "btree.hpp"




int main(int argc,char **argv){
	srand(1607153820);
	//1607153820
	clock_t start = clock();	
	map<int,module> modules;
	map<int,fixed_pin> pins;
	map<int,net> nets;
	//cout<<"input:"<<argv[1]<<" "<<argv[2]<<" "<<argv[3]<<" "<<argv[4]<<endl;
	
	//dratio;
	dratio=stof(argv[5]);
	//cout<<dratio<<endl;
	read_hardmodule(argv[1],modules,pins);
	
	
	
	read_pl(argv[3],pins);
	read_net(argv[2],nets,pins,modules);
	clock_t end = clock();
	cout << "IO Runtime : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";
	
	start = clock();
   int times=2000, local=7;
   float term_temp=0.1;	
	
	btree floorplan(0.6);
	floorplan.setting(nets,pins,modules);
	//cout<<floorplan.size()<<endl;
	if(modules.size()<150){
		cout<<"here1"<<endl;
		floorplan.rand_gen_tree(modules);
		end = clock();
		cout << "init Runtime : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";
		start = clock();
		int ctr=SA(floorplan,Max_y,Max_x, times, local, term_temp);
		end = clock();
		cout << "sa Runtime : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";
	}else if(modules.size()<250){
		//cout<<"here2"<<endl;
		//cout<<"rand:"<<250<<endl;
		//for(int i=0;i<72;i++){
			//cout<<"rand:"<<i<<endl;
		int seed=time(NULL);		
		//srand(seed);
		times=1250;
		floorplan.rand_gen_tree(modules);
		end = clock();
		cout << "init Runtime : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";
		start = clock();		
		int ctr=SA(floorplan,Max_y,Max_x, times, local, term_temp);
		end = clock();
		cout << "sa Runtime : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";
		//if(ctr==1)cout<<"contraint meet seed:"<<seed<<endl;
		//}
		
	}else{
		//cout<<"here3"<<endl;
		//for(int i=0;i<72;i++){
		int seed=time(NULL);		
		//srand(seed);
		times=1550;
		floorplan.rand_gen_tree(modules);
		end = clock();
		cout << "init Runtime : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";
		start = clock();		
		int ctr=SA(floorplan,Max_y,Max_x, times, local, term_temp);
		end = clock();
		cout << "sa Runtime : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";		
		//if(ctr==1)cout<<"contraint meet seed:"<<seed<<endl;
		//}		
		
	}
	floorplan.plot(argv[4],Max_x);
	
	output(argv[4],floorplan,floorplan.calcWireLength());
	//cout<<"fp.module_N:"<<floorplan.size()<<endl;
	//clock_t end = clock();
	cout<<"Area:"<<Area<<" Max_X:"<<Max_x<<" Max_Y:"<<Max_y<<endl;
	//cout<<"seed:"<<seed<<endl;
	end = clock();
	cout << "Runtime : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";		
	return 0;
}