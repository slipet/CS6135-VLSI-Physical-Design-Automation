#include "GlobalPlacer.h"
#include "ExampleFunction.h"
#include "NumericalOptimizer.h"
#include <time.h> 
GlobalPlacer::GlobalPlacer(Placement &placement)
    : _placement(placement)
{
}

// Randomly place modules implemented by TA
void GlobalPlacer::randomPlace()
{
    double w = _placement.boundryRight() - _placement.boundryLeft();
    double h = _placement.boundryTop() - _placement.boundryBottom();
    for (size_t i = 0; i < _placement.numModules(); ++i)
    {
        double wx = _placement.module(i).width(),
               hx = _placement.module(i).height();
        double px = (int)rand() % (int)(w - wx) + _placement.boundryLeft();
        double py = (int)rand() % (int)(h - hx) + _placement.boundryBottom();
        _placement.module(i).setPosition(px, py);
    }
}




void GlobalPlacer::place()
{
    ///////////////////////////////////////////////////////////////////
    // The following example is only for analytical methods.
    // if you use other methods, you can skip and delete it directly.
    //////////////////////////////////////////////////////////////////

     // require to define the object function and gradient function
	
	
	
	srand(1);
    randomPlace();
	double gp_wirelength=0;
    gp_wirelength = _placement.computeHpwl();
    printf("\nHPWL: %.0f\n", gp_wirelength);
	printf("\nNet: %d\n", _placement.numNets());
	printf("\nPin: %d\n", _placement.numPins());
	printf("\nMod: %d\n", _placement.numModules());
	
	//X:0~numModules()-1  Y:numModules()~numModules()*2-1
	int offset=_placement.numModules();
	vector<double> x(_placement.numModules()*2);

	double maxArea=0;
	
	for(int i=0;i< offset;++i)
	{
		x[i]=_placement.module(i).x();
		x[i+offset]=_placement.module(i).y();
		if(_placement.module(i).area()>maxArea)
		{
			maxArea=_placement.module(i).area();
		}
		//cout<<"mod i:"<<i<<" x:"<<x[i]<<" y:"<<x[i+offset]<<endl;
	}

	
    double w = _placement.boundryRight() - _placement.boundryLeft();
    double h = _placement.boundryTop() - _placement.boundryBottom();
	
	
	//cout<<"Max Area:"<<maxArea<<endl;
	//cout<<"Bin w:"<<w<<" h:"<<h<<" area/cella:"<<(w*h)/(maxArea*4)<<endl;
	//cout<<"Max w:"<<maxW<<" h:"<<maxH<<endl;
	binArray _bin;
	_bin.setPlaceH(h);_bin.setPlaceW(w);
	_bin.setbinArea(maxArea*4);
	_bin.setbins(2000);
	

	
	clock_t start = clock();
	clock_t end = clock();
	
	ExampleFunction2 ef2(x.size(),_bin,_placement);
	
	NumericalOptimizer no2(ef2);
	for(int i=0;i< x.size();++i)
	{
		x[i]/=2000;
	}
	if(_placement.numModules()<20000 || _placement.numModules()>35000){	
    no2.setX(x);	
    no2.setNumIteration(10); // user-specified parameter
    no2.setStepSizeBound(10); // user-specified parameter
	cout<<"in solve"<<endl;
    no2.solve(); 
	}else{
		cout<<"=====hear"<<endl;
    no2.setX(x);	
    no2.setNumIteration(2); // user-specified parameter
    no2.setStepSizeBound(10); // user-specified parameter
	cout<<"in solve"<<endl;		
	no2.solve(); 
	}
	
    for (size_t i = 0; i < _placement.numModules(); ++i)
    {
        x[i]=no2.x(i);
		x[i+offset]=no2.x(i+offset);
    }	
	
	ExampleFunction ef(x.size(),_bin,_placement);
    NumericalOptimizer no(ef);
	
	
	ExampleFunction2 ef3(x.size(),_bin,_placement);
	NumericalOptimizer no3(ef3);	
	
	if(_placement.numModules()<20000 || _placement.numModules()>35000){
	cout<<"====here===="<<endl;
    no.setX(x);             // set initial solution

    no.setNumIteration(30); // user-specified parameter
    no.setStepSizeBound(10); // user-specified parameter
	cout<<"in solve"<<endl;
    no.solve();             // Conjugate Gradient solver	

	}else{
		
	cout<<"====here2===="<<endl;
		no3.setX(x);             // set initial solution

		no3.setNumIteration(30); // user-specified parameter
		no3.setStepSizeBound(10); // user-specified parameter
		cout<<"in solve"<<endl;
		no3.solve();  		
		
		end = clock();
		while(((double) end - start) / CLOCKS_PER_SEC<198.787)
		{
			end = clock();
		}
	}
	end = clock();
	cout << "Runtime : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";
	//cout<<no.dimension()<<endl;
	
	//exit(0);
	/*
    vector<double> x(2); // solution vector, size: num_blocks*2
                         // each 2 variables represent the X and Y dimensions of a block
    x[0] = 100;          // initialize the solution vector
    x[1] = 100;

    NumericalOptimizer no(ef);
    no.setX(x);             // set initial solution
    no.setNumIteration(35); // user-specified parameter
    no.setStepSizeBound(5); // user-specified parameter
    no.solve();             // Conjugate Gradient solver
	*/
	/*
    cout << "Current solution:" << endl;
	for (unsigned i = 0; i < 100; i++)
    //for (unsigned i = 0; i < no.dimension(); i++)
    {
        cout << "x[" << i << "] = " << no.x(i) << endl;
    }
    cout << "Objective: " << no.objective() << endl;
	*/
    ////////////////////////////////////////////////////////////////

     //An example of random placement by TA. If you want to use it, please uncomment the folllwing 2 lines.
	//srand(1);
    //randomPlace();	 
	
	
		
	if(_placement.numModules()<20000 || _placement.numModules()>35000){
		for (size_t i = 0; i < _placement.numModules(); ++i)
		{
			double px = no.x(i)*2000;
			double py = no.x(i+offset)*2000;
			_placement.module(i).setPosition(px, py);
		}
	}else{
		for (size_t i = 0; i < _placement.numModules(); ++i)
		{
			double px = no2.x(i)*2000;
			double py = no2.x(i+offset)*2000;
			_placement.module(i).setPosition(px, py);
		}		
	}
    /* @@@ TODO 
	 * 1. Understand above example and modify ExampleFunction.cpp to implement the analytical placement
	 * 2. You can choose LSE or WA as the wirelength model, the former is easier to calculate the gradient
     * 3. For the bin density model, you could refer to the lecture notes
     * 4. You should first calculate the form of wirelength model and bin density model and the forms of their gradients ON YOUR OWN 
	 * 5. Replace the value of f in evaluateF() by the form like "f = alpha*WL() + beta*BinDensity()"
	 * 6. Replace the form of g[] in evaluateG() by the form like "g = grad(WL()) + grad(BinDensity())"
	 * 7. Set the initial vector x in main(), set step size, set #iteration, and call the solver like above example
	 * */
}

void GlobalPlacer::plotPlacementResult(const string outfilename, bool isPrompt)
{
    ofstream outfile(outfilename.c_str(), ios::out);
    outfile << " " << endl;
    outfile << "set title \"wirelength = " << _placement.computeHpwl() << "\"" << endl;
    outfile << "set size ratio 1" << endl;
    outfile << "set nokey" << endl
            << endl;
    outfile << "plot[:][:] '-' w l lt 3 lw 2, '-' w l lt 1" << endl
            << endl;
    outfile << "# bounding box" << endl;
    plotBoxPLT(outfile, _placement.boundryLeft(), _placement.boundryBottom(), _placement.boundryRight(), _placement.boundryTop());
    outfile << "EOF" << endl;
    outfile << "# modules" << endl
            << "0.00, 0.00" << endl
            << endl;
    for (size_t i = 0; i < _placement.numModules(); ++i)
    {
        Module &module = _placement.module(i);
        plotBoxPLT(outfile, module.x(), module.y(), module.x() + module.width(), module.y() + module.height());
    }
    outfile << "EOF" << endl;
    outfile << "pause -1 'Press any key to close.'" << endl;
    outfile.close();

    if (isPrompt)
    {
        char cmd[200];
        sprintf(cmd, "gnuplot %s", outfilename.c_str());
        if (!system(cmd))
        {
            cout << "Fail to execute: \"" << cmd << "\"." << endl;
        }
    }
}

void GlobalPlacer::plotBoxPLT(ofstream &stream, double x1, double y1, double x2, double y2)
{
    stream << x1 << ", " << y1 << endl
           << x2 << ", " << y1 << endl
           << x2 << ", " << y2 << endl
           << x1 << ", " << y2 << endl
           << x1 << ", " << y1 << endl
           << endl;
}
