#ifndef EXAMPLEFUNCTION_H
#define EXAMPLEFUNCTION_H

#include "NumericalOptimizerInterface.h"
#include "GlobalPlacer.h"
#include<cmath>

class Bin
{
public:	
	double x(){return X;}
	double y(){return Y;};
	double w(){return W;};
	double h(){return H;};
	double centerX(){return X+(W/2);};
	double centerY(){return Y+(H/2);};
	
	void set(double,double,double,double);
	
private:
	double X;
	double Y;
	double W;
	double H;
};

class binArray
{
public:
	vector<Bin> bins;
	
	double w(){return PlaceW;}
	double h(){return PlaceH;}
	double PlaceArea(){return PlaceW*PlaceH;}
	
	void setbins(double);
	
	void setPlaceW(double );
	void setPlaceH(double );
	void setbinArea(double );
	//void setbinScale(double );
	
private:
	double PlaceH;
	double PlaceW;
		
		//double Area;
	double binArea;
};

class ExampleFunction : public NumericalOptimizerInterface
{
	unsigned _dimension;
	binArray _bin;
	//vector<double> _coe;
	Placement _placement;
	
	
public:
    ExampleFunction(unsigned ,binArray &,Placement &);

    void evaluateFG(const vector<double> &x, double &f, vector<double> &g);
    void evaluateF(const vector<double> &x, double &f);
    unsigned dimension();
};

class ExampleFunction2 : public NumericalOptimizerInterface
{
	unsigned _dimension;
	binArray _bin;
	//vector<double> _coe;
	Placement _placement;
	
	
public:
    ExampleFunction2(unsigned ,binArray &,Placement &);

    void evaluateFG(const vector<double> &x, double &f, vector<double> &g);
    void evaluateF(const vector<double> &x, double &f);
    unsigned dimension();
};


#endif // EXAMPLEFUNCTION_H
