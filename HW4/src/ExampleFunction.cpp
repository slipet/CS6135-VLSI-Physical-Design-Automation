#include "ExampleFunction.h"

// minimize 3*x^2 + 2*x*y + 2*y^2 + 7
#include<iostream>
#include <errno.h>
using namespace std;
ExampleFunction::ExampleFunction(unsigned dimension,binArray &bin,Placement &placement)
{
	this->_dimension=dimension;
	this->_placement=placement;
	this->_bin=bin;
}

void ExampleFunction::evaluateFG(const vector<double> &x, double &f, vector<double> &g)
{
	errno = 0;
	int offset=_placement.numModules();
	double delta=0.4;
	double beta=100;
	double Tb=0.7;
	double Denstity=0;
	
	vector<double> Ci((this->_placement.numModules()));
	//cout<<"x:"<<0<<" v:"<<x[100]<<endl;
	//long double lf=0;
	
clock_t start = clock();	
	
	for(int i=0;i<(this->_placement).numNets();i++)
	{
		Net &_net=(this->_placement).net(i);
		long double expX1=0,expX2=0;
		long double expY1=0,expY2=0;
		for(int j=0;j<_net.numPins();j++)
		{
			Pin &_pin=_net.pin(j);
			unsigned id=_pin.moduleId();
			expX1+=expl(x[id]/delta);
			expX2+=expl(-1.0*x[id]/delta);
			expY1+=expl(x[id+offset]/delta);
			expY2+=expl(-1.0*x[id+offset]/delta);
			
			//lf=exp(x[id]/delta);
			//cout<<"f:"<<expl(x[id]/delta)<<endl;
		}
		f+=(log(expX1)+log(expX2)+log(expY1)+log(expY2));
		//cout<<"f:"<<log(expY1)<<endl;
		//exit(0);
	}
	
//clock_t end = clock();
//cout << "for1 Runtime : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";	
	
	for(unsigned id=0;id<(this->_placement.numModules());id++)
	{
		double area=_placement.module(id).area();
		
		for(int i=0;i<this->_bin.bins.size();i++)
		{
			double dx;
			double Xi,Xb,Wb,Wi;
			long double thetaX=0;
			Xi=x[id]+(_placement.module(id).width()/2);
			Xb=_bin.bins[i].centerX();
			Wb=_bin.bins[i].w();
			Wi=_placement.module(id).width();
			dx=(Xi>Xb)?Xi-Xb:Xb-Xi;

			if(0<=dx && dx<= (Wb/2+Wi/2))
			{
				double a = 4/((Wb+Wi)*(2*Wb+Wi));
				thetaX=1-a*pow(dx,2);
			} 
			if((Wb/2+Wi/2)<dx && dx<=(Wb+Wi/2))
			{
				double b = 4/(Wb*(2*Wb+Wi));
				thetaX=b*pow((dx-Wb-Wi/2),2);
				
			}
			if((Wb+Wi/2)<dx)
			{
				thetaX=0;
			}
			//////////////////////////////////////
			//////////////////////////////////////
			double dy;
			double Yi,Yb,Hb,Hi;
			long double thetaY=0;
			Yi=x[id+offset]+(_placement.module(id).height()/2);
			Yb=_bin.bins[i].centerY();
			Hb=_bin.bins[i].h();
			Hi=_placement.module(id).height();
			dy=(Yi>Yb)?Yi-Yb:Yb-Yi;
			
			if(0<=dy && dy<= (Hb/2+Hi/2))
			{
				double a = 4/((Hb+Hi)*(2*Hb+Hi));
				thetaY=1-a*pow(dy,2);
			} 
			if((Hb/2+Hi/2)<dy && dy<=(Hb+Hi/2))
			{
				double b = 4/(Hb*(2*Hb+Hi));
				thetaY=b*pow((dy-Hb-Hi/2),2);
				
			}
			if((Hb+Hi/2)<dy)
			{
				thetaY=0;
			}			
			//cout<<"thetaX:"<<thetaX<<" thetaY:"<<thetaY<<endl;
			Ci[id]+=(thetaX*thetaY);
			
		}
		Ci[id]=area/Ci[id];
	}
	
//end = clock();
//cout << "for2 Runtime : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";	
	
	for(int i=0;i<this->_bin.bins.size();i++)
	{
		double Db=0;
		for(unsigned id=0;id<(this->_placement.numModules());id++)
		{
			double dx;
			double Xi,Xb,Wb,Wi;
			long double thetaX=0;
			Xi=x[id]+(_placement.module(id).width()/2);
			Xb=_bin.bins[i].centerX();
			Wb=_bin.bins[i].w();
			Wi=_placement.module(id).width();
			dx=(Xi>Xb)?Xi-Xb:Xb-Xi;
			
			if(0<=dx && dx<= (Wb/2+Wi/2))
			{
				double a = 4/((Wb+Wi)*(2*Wb+Wi));
				thetaX=1-a*pow(dx,2);
			} 
			if((Wb/2+Wi/2)<dx && dx<=(Wb+Wi/2))
			{
				double b = 4/(Wb*(2*Wb+Wi));
				thetaX=b*pow((dx-Wb-Wi/2),2);
				
			}
			if((Wb+Wi/2)<dx)
			{
				thetaX=0;
			}
			//////////////////////////////////////
			//////////////////////////////////////
			double dy;
			double Yi,Yb,Hb,Hi;
			long double thetaY=0;
			Yi=x[id+offset]+(_placement.module(id).height()/2);
			Yb=_bin.bins[i].centerY();
			Hb=_bin.bins[i].h();
			Hi=_placement.module(id).height();
			dy=(Yi>Yb)?Yi-Yb:Yb-Yi;
			
			if(0<=dy && dy<= (Hb/2+Hi/2))
			{
				double a = 4/((Hb+Hi)*(2*Hb+Hi));
				thetaY=1-a*pow(dy,2);
			} 
			if((Hb/2+Hi/2)<dy && dy<=(Hb+Hi/2))
			{
				double b = 4/(Hb*(2*Hb+Hi));
				thetaY=b*pow((dy-Hb-Hi/2),2);
				
			}
			if((Hb+Hi/2)<dy)
			{
				thetaY=0;
			}			
			//cout<<"thetaX:"<<thetaX<<" thetaY:"<<thetaY<<endl;
			Db+=Ci[id]*(thetaX*thetaY);
		}
		//cout<<"Denstity:"<<Denstity<<endl;
		Denstity+=pow((Db-Tb),2);
	}
	Denstity*=beta;
	//cout<<"Denstity:"<<Denstity<<endl;
	f*=delta;
	f+=(Denstity);
	//f+=Db;
	//exit(0);
	//cout<<"of:"<<f<<endl;
	//vector<long double> lg(g.size());
	
//end = clock();
//cout << "for3 Runtime : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";	
		
		for(int i=0;i<(this->_placement).numNets();i++)
		{
			Net &_net=(this->_placement).net(i);
			long double expX1=0,expX2=0;
			long double expY1=0,expY2=0;
			//bool mod_meet=false;
			for(int j=0;j<_net.numPins();j++)
			{
				Pin &_pin=_net.pin(j);
				unsigned id=_pin.moduleId();
				expX1+=expl(x[id]/delta);
				expX2+=expl(-1.0*x[id]/delta);
				expY1+=expl(x[id+offset]/delta);
				expY2+=expl(-1.0*x[id+offset]/delta);
				
				
				
			}
			for(int j=0;j<_net.numPins();j++)
			{
				Pin &_pin=_net.pin(j);
				unsigned id=_pin.moduleId();				
				long double gX1=0,gX2=0;
				long double gY1=0,gY2=0;
				
				long double GX=0,GY=0;
				
				gX1=1*(expl(x[id]/delta))/(delta*(expX1));
				gX2=-1.0*(expl(-1.0*x[id]/delta))/(delta*(expX2));
				gY1=1*(expl(x[id+offset]/delta))/(delta*(expY1));
				gY2=-1.0*(expl(-1.0*x[id+offset]/delta))/(delta*(expY2));

				GX=delta*(gX1+gX2);
				GY=delta*(gY1+gY2);
				g[id]+=GX;
				g[id+offset]+=GY;
			}				
		}
		//cout<<"==================================="<<endl;
//end = clock();
//cout << "for4 Runtime : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";	

	for(int i=0;i<this->_bin.bins.size();i++)
	{
		double Db=0;
		for(unsigned id=0;id<(this->_placement.numModules());id++)
		{
			double dx;
			double Xi,Xb,Wb,Wi;
			long double thetaX=0;
			long double gthetaX=0;
			Xi=x[id]+(_placement.module(id).width()/2);
			Xb=_bin.bins[i].centerX();
			Wb=_bin.bins[i].w();
			Wi=_placement.module(id).width();
			dx=(Xi>Xb)?Xi-Xb:Xb-Xi;
			
			if(0<=dx && dx<= (Wb/2+Wi/2))
			{
				double a = 4/((Wb+Wi)*(2*Wb+Wi));
				thetaX=1-a*pow(dx,2);
				gthetaX=-2*a*(Xi-Xb);
			} 
			if((Wb/2+Wi/2)<dx && dx<=(Wb+Wi/2))
			{
				double b = 4/(Wb*(2*Wb+Wi));
				thetaX=b*pow((dx-Wb-Wi/2),2);
				gthetaX=2*b*((Xi-Xb)-Wb-Wi/2);
			}
			if((Wb+Wi/2)<dx)
			{
				thetaX=0;
				gthetaX=0;
			}
			//////////////////////////////////////
			//////////////////////////////////////
			double dy;
			double Yi,Yb,Hb,Hi;
			long double thetaY=0;
			long double gthetaY=0;
			Yi=x[id+offset]+(_placement.module(id).height()/2);
			Yb=_bin.bins[i].centerY();
			Hb=_bin.bins[i].h();
			Hi=_placement.module(id).height();
			dy=(Yi>Yb)?Yi-Yb:Yb-Yi;
			
			if(0<=dy && dy<= (Hb/2+Hi/2))
			{
				double a = 4/((Hb+Hi)*(2*Hb+Hi));
				thetaY=1-a*pow(dy,2);
				gthetaY=-2*a*(Xi-Xb);
			} 
			if((Hb/2+Hi/2)<dy && dy<=(Hb+Hi/2))
			{
				double b = 4/(Hb*(2*Hb+Hi));
				thetaY=b*pow((dy-Hb-Hi/2),2);
				gthetaY=2*b*((Xi-Xb)-Hb-Hi/2);
			}
			if((Hb+Hi/2)<dy)
			{
				thetaY=0;
				gthetaY=0;
			}			
			//cout<<"thetaX:"<<thetaX<<" thetaY:"<<thetaY<<endl;
			//Db+=Ci[id]*(thetaX*thetaY);
			g[id]+=(2*beta*Ci[id]*(gthetaX)*(thetaY));
			g[id+offset]+=(2*beta*Ci[id]*(thetaX)*(gthetaY));
		}
		//cout<<"Denstity:"<<Denstity<<endl;
		//Denstity+=pow((Db-Tb),2);
	}

//end = clock();
//cout << "for5 Runtime : " << ((double) end - start) / CLOCKS_PER_SEC << "s\n";
	
    if (errno == ERANGE) {
        cout<<"1exp(f) overflows:"<<f<<endl;
        //result = param;
    }		
	
		
	/*
	for(int i=0;i<_placement.numNets();i++)
	{
		Net &_net=_placement.net(i);
		for(int j=0;j<_net.numPins();j++)
		{
			Pin &_pin=_net.pin(j);
			unsigned id=_pin.moduleId();
			coe[id]+=1;
			coe[id+offset]+=1;
		}
		
	}	
	*/
	//exit(0);
	//double delta=0.1;
	//f=delta*(log(exp(x[0]/delta))+log(exp(x[1]/delta)));
	//g[0]=delta*(1/delta/(exp(x[0]/delta)*log(10)));
	//g[1]=delta*(1/delta/(exp(x[1]/delta)*log(10)));
	/*
    f = 3 * x[0] * x[0] + 2 * x[0] * x[1] + 2 * x[1] * x[1] + 7; // objective function
    g[0] = 6 * x[0] + 2 * x[1];                                  // gradient function of X
    g[1] = 2 * x[0] + 4 * x[1];                                  // gradient function of Y
	*/
}

void ExampleFunction::evaluateF(const vector<double> &x, double &f)
{
	int offset=_placement.numModules();
	double delta=0.4;
	double beta=100;
	double Tb=0.7;
	double Denstity=0;
	
	vector<double> Ci((this->_placement.numModules()));	
	
	for(int i=0;i<(this->_placement).numNets();i++)
	{
		Net &_net=(this->_placement).net(i);
		long double expX1=0,expX2=0;
		long double expY1=0,expY2=0;
		for(int j=0;j<_net.numPins();j++)
		{
			Pin &_pin=_net.pin(j);
			unsigned id=_pin.moduleId();
			expX1+=expl(x[id]/delta);
			expX2+=expl(-1.0*x[id]/delta);
			expY1+=expl(x[id+offset]/delta);
			expY2+=expl(-1.0*x[id+offset]/delta);
			
			//lf=exp(x[id]/delta);
			//cout<<"f:"<<expl(x[id]/delta)<<endl;
		}
		f+=(log(expX1)+log(expX2)+log(expY1)+log(expY2));
		//cout<<"f:"<<log(expY1)<<endl;
		//exit(0);
	}
	for(unsigned id=0;id<(this->_placement.numModules());id++)
	{
		double area=_placement.module(id).area();
		
		for(int i=0;i<this->_bin.bins.size();i++)
		{
			double dx;
			double Xi,Xb,Wb,Wi;
			long double thetaX=0;
			Xi=x[id]+(_placement.module(id).width()/2);
			Xb=_bin.bins[i].centerX();
			Wb=_bin.bins[i].w();
			Wi=_placement.module(id).width();
			dx=(Xi>Xb)?Xi-Xb:Xb-Xi;

			if(0<=dx && dx<= (Wb/2+Wi/2))
			{
				double a = 4/((Wb+Wi)*(2*Wb+Wi));
				thetaX=1-a*pow(dx,2);
			} 
			if((Wb/2+Wi/2)<dx && dx<=(Wb+Wi/2))
			{
				double b = 4/(Wb*(2*Wb+Wi));
				thetaX=b*pow((dx-Wb-Wi/2),2);
				
			}
			if((Wb+Wi/2)<dx)
			{
				thetaX=0;
			}
			//////////////////////////////////////
			//////////////////////////////////////
			double dy;
			double Yi,Yb,Hb,Hi;
			long double thetaY=0;
			Yi=x[id+offset]+(_placement.module(id).height()/2);
			Yb=_bin.bins[i].centerY();
			Hb=_bin.bins[i].h();
			Hi=_placement.module(id).height();
			dy=(Yi>Yb)?Yi-Yb:Yb-Yi;
			
			if(0<=dy && dy<= (Hb/2+Hi/2))
			{
				double a = 4/((Hb+Hi)*(2*Hb+Hi));
				thetaY=1-a*pow(dy,2);
			} 
			if((Hb/2+Hi/2)<dy && dy<=(Hb+Hi/2))
			{
				double b = 4/(Hb*(2*Hb+Hi));
				thetaY=b*pow((dy-Hb-Hi/2),2);
				
			}
			if((Hb+Hi/2)<dy)
			{
				thetaY=0;
			}			
			//cout<<"thetaX:"<<thetaX<<" thetaY:"<<thetaY<<endl;
			Ci[id]+=(thetaX*thetaY);
			
		}
		Ci[id]=area/Ci[id];
	}
	
	
	for(int i=0;i<this->_bin.bins.size();i++)
	{
		double Db=0;
		for(unsigned id=0;id<(this->_placement.numModules());id++)
		{
			double dx;
			double Xi,Xb,Wb,Wi;
			long double thetaX=0;
			Xi=x[id]+(_placement.module(id).width()/2);
			Xb=_bin.bins[i].centerX();
			Wb=_bin.bins[i].w();
			Wi=_placement.module(id).width();
			dx=(Xi>Xb)?Xi-Xb:Xb-Xi;
			
			if(0<=dx && dx<= (Wb/2+Wi/2))
			{
				double a = 4/((Wb+Wi)*(2*Wb+Wi));
				thetaX=1-a*pow(dx,2);
			} 
			if((Wb/2+Wi/2)<dx && dx<=(Wb+Wi/2))
			{
				double b = 4/(Wb*(2*Wb+Wi));
				thetaX=b*pow((dx-Wb-Wi/2),2);
				
			}
			if((Wb+Wi/2)<dx)
			{
				thetaX=0;
			}
			//////////////////////////////////////
			//////////////////////////////////////
			double dy;
			double Yi,Yb,Hb,Hi;
			long double thetaY=0;
			Yi=x[id+offset]+(_placement.module(id).height()/2);
			Yb=_bin.bins[i].centerY();
			Hb=_bin.bins[i].h();
			Hi=_placement.module(id).height();
			dy=(Yi>Yb)?Yi-Yb:Yb-Yi;
			
			if(0<=dy && dy<= (Hb/2+Hi/2))
			{
				double a = 4/((Hb+Hi)*(2*Hb+Hi));
				thetaY=1-a*pow(dy,2);
			} 
			if((Hb/2+Hi/2)<dy && dy<=(Hb+Hi/2))
			{
				double b = 4/(Hb*(2*Hb+Hi));
				thetaY=b*pow((dy-Hb-Hi/2),2);
				
			}
			if((Hb+Hi/2)<dy)
			{
				thetaY=0;
			}			
			//cout<<"thetaX:"<<thetaX<<" thetaY:"<<thetaY<<endl;
			Db+=Ci[id]*(thetaX*thetaY);
		}
		//cout<<"Denstity:"<<Denstity<<endl;
		Denstity+=pow((Db-Tb),2);
	}
	Denstity*=beta;	
	f*=delta;
	f+=(Denstity);	
	//double delta=0.1;
	//f=delta*(log(exp(x[0]/delta))+log(exp(x[1]/delta)));
    //f = 3 * x[0] * x[0] + 2 * x[0] * x[1] + 2 * x[1] * x[1] + 7; // objective function
}

unsigned ExampleFunction::dimension()
{
    return this->_dimension; // num_blocks*2
    // each two dimension represent the X and Y dimensions of each block
}



void Bin::set(double x,double y,double w,double h)
{
	this->X=x;
	this->Y=y;
	this->W=w;
	this->H=h;
}



void binArray::setPlaceW(double w)
{
	this->PlaceW=w;
	cout<<"PlaceW:"<<PlaceW<<endl;
}

void binArray::setPlaceH(double h)
{
	this->PlaceH=h;
	//cout<<"PlaceW:"<<PlaceW<<endl;
}

void binArray::setbinArea(double Area)
{
	this->binArea=Area;
}

void binArray::setbins(double scale)
{
	cout<<"binArea:"<<binArea<<" sqrt:"<<sqrt(binArea)<<endl;
	int  sqrtbin=sqrt(binArea);
	double binW=PlaceW;
	double binH=PlaceH;
	while(binH>sqrtbin)
		binH=binH/2;
	while(binW>sqrtbin)
		binW=binW/2;
	int W_num=PlaceW/binW;
	int H_num=PlaceH/binH;
	
	//cout<<"h:"<<binH<<" num:"<<H_num<<endl;
	//cout<<"w:"<<binW<<" num:"<<W_num<<endl;
	
	if(bins.size()>0)
	{
		bins.clear();
	}
	if(bins.size()==0)
	{
		bins.resize(W_num*H_num);
	}
	cout<<"bin size:"<<bins.size()<<endl;
	double setX=0,setY=0;
	for(int i=0;i<bins.size();i++)
	{
		bins[i].set(setX/scale,setY/scale,binW/scale,binH/scale);
		
		setX+=binW;
		
		if(setX>=PlaceW)
		{
			setX=0;
			setY+=binH;
		}
	}
	/*
	for(int i=0;i<bins.size();i++)
	{
		if(bins[i].x()+bins[i].w()>PlaceW)
			cout<<"place bin w wrong:"<<bins[i].x()+bins[i].w()<<endl;
		if(bins[i].y()+bins[i].h()>PlaceH)
			cout<<"place bin h wrong"<<endl;
		
		//cout<<"bin "<<i<<": x:"<<bins[i].x()<<" y:"<<bins[i].y()<<endl;
		
	}*/
	
	

}


ExampleFunction2::ExampleFunction2(unsigned dimension,binArray &bin,Placement &placement)
{
	this->_dimension=dimension;
	this->_placement=placement;
	this->_bin=bin;
}

void ExampleFunction2::evaluateFG(const vector<double> &x, double &f, vector<double> &g)
{
	errno = 0;
	int offset=_placement.numModules();
	double delta=0.4;
	double beta=100;
	double Tb=0.7;
	double Denstity=0;
	
	//vector<double> Ci((this->_placement.numModules()));
	//cout<<"x:"<<0<<" v:"<<x[100]<<endl;
	//long double lf=0;
	

	
	for(int i=0;i<(this->_placement).numNets();i++)
	{
		Net &_net=(this->_placement).net(i);
		long double expX1=0,expX2=0;
		long double expY1=0,expY2=0;
		for(int j=0;j<_net.numPins();j++)
		{
			Pin &_pin=_net.pin(j);
			unsigned id=_pin.moduleId();
			expX1+=expl(x[id]/delta);
			expX2+=expl(-1.0*x[id]/delta);
			expY1+=expl(x[id+offset]/delta);
			expY2+=expl(-1.0*x[id+offset]/delta);
			
			//lf=exp(x[id]/delta);
			//cout<<"f:"<<expl(x[id]/delta)<<endl;
		}
		f+=(log(expX1)+log(expX2)+log(expY1)+log(expY2));
		//cout<<"f:"<<log(expY1)<<endl;
		//exit(0);
	}
	

	
	f*=delta;

		
		for(int i=0;i<(this->_placement).numNets();i++)
		{
			Net &_net=(this->_placement).net(i);
			long double expX1=0,expX2=0;
			long double expY1=0,expY2=0;
			//bool mod_meet=false;
			for(int j=0;j<_net.numPins();j++)
			{
				Pin &_pin=_net.pin(j);
				unsigned id=_pin.moduleId();
				expX1+=expl(x[id]/delta);
				expX2+=expl(-1.0*x[id]/delta);
				expY1+=expl(x[id+offset]/delta);
				expY2+=expl(-1.0*x[id+offset]/delta);
				
				
				
			}
			for(int j=0;j<_net.numPins();j++)
			{
				Pin &_pin=_net.pin(j);
				unsigned id=_pin.moduleId();				
				long double gX1=0,gX2=0;
				long double gY1=0,gY2=0;
				
				long double GX=0,GY=0;
				
				gX1=1*(expl(x[id]/delta))/(delta*(expX1));
				gX2=-1.0*(expl(-1.0*x[id]/delta))/(delta*(expX2));
				gY1=1*(expl(x[id+offset]/delta))/(delta*(expY1));
				gY2=-1.0*(expl(-1.0*x[id+offset]/delta))/(delta*(expY2));

				GX=delta*(gX1+gX2);
				GY=delta*(gY1+gY2);
				g[id]+=GX;
				g[id+offset]+=GY;
			}				
		}
		//cout<<"==================================="<<endl;

	
    if (errno == ERANGE) {
        cout<<"1exp(f) overflows:"<<f<<endl;
        //result = param;
    }		
	
		
	/*
	for(int i=0;i<_placement.numNets();i++)
	{
		Net &_net=_placement.net(i);
		for(int j=0;j<_net.numPins();j++)
		{
			Pin &_pin=_net.pin(j);
			unsigned id=_pin.moduleId();
			coe[id]+=1;
			coe[id+offset]+=1;
		}
		
	}	
	*/
	//exit(0);
	//double delta=0.1;
	//f=delta*(log(exp(x[0]/delta))+log(exp(x[1]/delta)));
	//g[0]=delta*(1/delta/(exp(x[0]/delta)*log(10)));
	//g[1]=delta*(1/delta/(exp(x[1]/delta)*log(10)));
	/*
    f = 3 * x[0] * x[0] + 2 * x[0] * x[1] + 2 * x[1] * x[1] + 7; // objective function
    g[0] = 6 * x[0] + 2 * x[1];                                  // gradient function of X
    g[1] = 2 * x[0] + 4 * x[1];                                  // gradient function of Y
	*/
}

void ExampleFunction2::evaluateF(const vector<double> &x, double &f)
{
	int offset=_placement.numModules();
	double delta=0.4;
	double beta=100;
	double Tb=0.7;
	double Denstity=0;
	
	vector<double> Ci((this->_placement.numModules()));	
	
	for(int i=0;i<(this->_placement).numNets();i++)
	{
		Net &_net=(this->_placement).net(i);
		long double expX1=0,expX2=0;
		long double expY1=0,expY2=0;
		for(int j=0;j<_net.numPins();j++)
		{
			Pin &_pin=_net.pin(j);
			unsigned id=_pin.moduleId();
			expX1+=expl(x[id]/delta);
			expX2+=expl(-1.0*x[id]/delta);
			expY1+=expl(x[id+offset]/delta);
			expY2+=expl(-1.0*x[id+offset]/delta);
			
			//lf=exp(x[id]/delta);
			//cout<<"f:"<<expl(x[id]/delta)<<endl;
		}
		f+=(log(expX1)+log(expX2)+log(expY1)+log(expY2));
		//cout<<"f:"<<log(expY1)<<endl;
		//exit(0);
	}

	
	
	f*=delta;
	
	//double delta=0.1;
	//f=delta*(log(exp(x[0]/delta))+log(exp(x[1]/delta)));
    //f = 3 * x[0] * x[0] + 2 * x[0] * x[1] + 2 * x[1] * x[1] + 7; // objective function
}

unsigned ExampleFunction2::dimension()
{
    return this->_dimension; // num_blocks*2
    // each two dimension represent the X and Y dimensions of each block
}

