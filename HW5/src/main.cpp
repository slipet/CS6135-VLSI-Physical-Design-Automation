#include "GlobalRouting.hpp"


int main(int argc,char **argv){
	
	GlobalRouting gb;
	//cout<<argv[1]<<endl;
	clock_t start = clock();
	gb.readfile(argv[1]);
	clock_t end = clock();
	cout << "I Runtime : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";
	
	gb.Routing();
	
	start = clock();
	gb.output(argv[2]);
	end = clock();
	cout << "O Runtime : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";
	return 0;
}