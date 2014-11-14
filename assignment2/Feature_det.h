
#define PI 22.0/7.0
using namespace std;


//function to find theta
SDoublePlane find_theta(const SDoublePlane &xgrad,const SDoublePlane &ygrad) 
{
	SDoublePlane theta(xgrad.rows(),ygrad.cols());
	
	for(int i=0; i<xgrad.rows(); i++)
	{
		for(int j=0; j<ygrad.cols(); j++) 
		{
			double a=ygrad[i][j];
			double b=xgrad[i][j];
			theta[i][j]=(atan2 (a,b) * 180.0 / 3.143);
			if((-180)>theta[i][j])
				theta[i][j]=(-180);
			else if( theta[i][j]>(+180))
				theta[i][j]=180;
			
		}

	}
 return theta;
}



