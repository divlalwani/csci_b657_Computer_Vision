#include <SImage.h>
#include <SImageIO.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <math.h>
#include<limits.h>
using namespace std;

SDoublePlane unary_stereo(const SDoublePlane &left_image, const SDoublePlane &right_image)
{
 
 SDoublePlane result(left_image.rows(), left_image.cols());

 //We want to store the disparities in dmat and cost due to disparities in costd
 SDoublePlane costd(left_image.rows(), left_image.cols());
 SDoublePlane dmat(left_image.rows(), left_image.cols());
 int count=0;
 double min,a;
 int md=0;

 for(int i=8; i<(left_image.rows()-8); i++)
   for(int j=8; j<(left_image.cols()-8); j++)
	{ 
		min = UINT_MAX;		
		
		for(int d=0;d<30;d++)
		  {
			  costd[i][j]=0;
			    
	    		    for(int u=-8; u<8; u++)
               			  for(int v=-8; v<8 ; v++)
	        		    {    
			   		 //From the formula given in assignment question we compute the cost			   
	           			  costd[i][j]= costd[i][j]+ ((left_image[i+u][j+v]-right_image[i+u][j+v+d])* (left_image[i+u][j+v]-right_image[i+u][j+v+d])); 
	          			 
				    }
	      			   
				    //According to the costd computed, we select appropriate d value
	       			    if(costd[i][j]<min)
	      			    {

	      			          min=costd[i][j];
	      			          dmat[i][j]=d;
	     			    }
		  }

	}
  return dmat;
}


SDoublePlane mrf_loopy(const SDoublePlane &cost)
{

 //We want to store messages coming from top, bottom, left and right and update them
 SDoublePlane mleft(cost.rows(), cost.cols());
 SDoublePlane mright(cost.rows(), cost.cols());
 SDoublePlane mtop(cost.rows(), cost.cols());
 SDoublePlane mbottom(cost.rows(), cost.cols());

 //We want to store labels updated by each messaging passing to compare and select the best
 SDoublePlane newdisp(cost.rows(), cost.cols());
 SDoublePlane newr(cost.rows(), cost.cols());
 SDoublePlane newl(cost.rows(), cost.cols());
 SDoublePlane newb(cost.rows(), cost.cols());
 SDoublePlane newt(cost.rows(), cost.cols());

 double msg,msgcal,vcost;

//For all directions, we compute the beliefs propagated
//We consider 10 iterations of loopy

for(int times=0;times<10;times++)
{

//messages propagating to the right
 for(int i=2;i<(cost.rows()-2);i++)
  for(int j=2;j<(cost.cols()-2);j++)
	{
		//For all labels we find the minimum cost 
		double min=UINT_MAX;	
		for(int m=0; m<30; m++)
		  {
			//Messages from all neighbours except one it will send
			msg= (mright[i-1][j]+ mright[i][j-1] + mright[i+1][j])/3;
			msgcal= (msg-mright[i-1][j])+ (msg-mright[i][j-1])+ (msg-mright[i+1][j]);
			
			//Smoothness cost
			vcost=pow((m-cost[i-1][j]),2.0)+pow((m-cost[i][j-1]),2.0)+pow((m-cost[i+1][j]),2.0)+pow((m-cost[i][j+1]),2.0);
     			
			//message packet
			mleft[i][j]=cost[i][j]+ vcost+ msgcal;
			
			//Checking for best label based on message
			if(mright[i][j]<min)	
	  		{
			min=mright[i][j];
	 		newr[i][j]=m;	
			}
		}
	 //Passing best value to propagate to neighbors
	 mright[i][j]=min;	
	}


 //messages propagating to the left
 for(int i=2;i<(cost.rows()-2);i++)
  for(int j=(cost.cols()-2);j>2;j--)
	{

		//For all labels we find the minimum cost 
		double min=UINT_MAX;	
		for(int m=0; m<30; m++)
		  {
			//Messages from all neighbours except one it will send
			msg= (mleft[i-1][j]+ mleft[i][j+1] + mleft[i+1][j])/3;
			msgcal= (msg-mleft[i-1][j])+ (msg-mleft[i][j+1])+ (msg-mleft[i+1][j]);
			
			//Smoothness cost
			vcost=pow((m-cost[i-1][j]),2.0)+pow((m-cost[i][j-1]),2.0)+pow((m-cost[i+1][j]),2.0)+pow((m-cost[i][j+1]),2.0);
     			
			//message packet
			mleft[i][j]=cost[i][j]+ vcost+ msgcal;
			
			//Checking for best label based on message
			 if(mleft[i][j]<min)	
	  		{
			min=mleft[i][j];
	 		newl[i][j]=m;	
			 }
		}
	 //Passing best value to propagate to neighbors
	 mleft[i][j]=min;
	
	}

 //messages to the bottom
 for(int j=2;j<(cost.cols()-2);j++)
  for(int i=2;i<(cost.rows()-2);i++)
	{
		//For all labels we find the minimum cost 
		double min=UINT_MAX;	
		for(int m=0; m<30; m++)
		{
			//Messages from all neighbours except one it will send 
			msg= (mbottom[i-1][j]+ mbottom[i][j-1] + mbottom[i][j+1])/3;
			msgcal= (msg-mbottom[i-1][j])+ (msg-mbottom[i][j-1])+ (msg-mbottom[i][j+1]);
			
			//Smoothness cost
			vcost=pow((m-cost[i-1][j]),2.0)+pow((m-cost[i][j-1]),2.0)+pow((m-cost[i+1][j]),2.0)+pow((m-cost[i][j+1]),2.0);

     			//message packet
			mbottom[i][j]=cost[i][j]+ vcost+ msgcal;
		
			//Checking for best label based on message
			if(mbottom[i][j]<min)	
	  		{
			min=mbottom[i][j];
	 		newb[i][j]=m;	
			}
		}
	//Passing best value to propagate to neighbors
	mbottom[i][j]=min;
 
	}


//messages to the top
for(int j=2;j<(cost.cols()-2);j++)
  for(int i=(cost.rows()-2);i>2;i--)
	{
		//messages propagating to the right
		double min=UINT_MAX;	
		for(int m=0; m<30; m++)
		  {
			//Messages from all neighbours except one it will send 
			msg= (mtop[i+1][j]+ mtop[i][j-1] + mtop[i][j+1])/3;
			msgcal= (msg-mtop[i+1][j])+ (msg-mtop[i][j-1])+ (msg-mtop[i][j+1]);
			
			//Smoothness cost
			vcost=pow((m-cost[i-1][j]),2.0)+pow((m-cost[i][j-1]),2.0)+pow((m-cost[i+1][j]),2.0)+pow((m-cost[i][j+1]),2.0);
     			
			//message packet
			mtop[i][j]=cost[i][j]+ vcost+ msgcal;

			//Checking for best label based on message
			if(mtop[i][j]<min)	
	  		{
			min=mtop[i][j];
	 		newt[i][j]=m;	
			}
		}
	 
	  //Passing best value to propagate to neighbors
	  mtop[i][j]=min;

	}

for(int u=0; u<cost.rows(); u++)
 for(int v=0; v<cost.cols() ; v++)
  {
	
	newdisp[u][v]=(newr[u][v]+newl[u][v]+newb[u][v]+newt[u][v])*256.0/(4.0*30.0); 
  }

}


return newdisp;
}



int main(int argc, char *argv[])
{
  if(argc != 4 && argc != 3)
    {
      cerr << "usage: " << argv[0] << " image_file1 image_file2 [gt_file]" << endl;
      return 1;
    }

  string input_filename1 = argv[1], input_filename2 = argv[2];
  string gt_filename;
  if(argc == 4)
    gt_filename = argv[3];

  // read in images and gt
  SDoublePlane image1 = SImageIO::read_png_file(input_filename1.c_str());
  SDoublePlane image2 = SImageIO::read_png_file(input_filename2.c_str());
  SDoublePlane gt;
  if(gt_filename != "")
  {
    gt = SImageIO::read_png_file(gt_filename.c_str());
    // gt maps are scaled by a factor of 3, undo this...
    for(int i=0; i<gt.rows(); i++)
      for(int j=0; j<gt.cols(); j++)

       gt[i][j] = gt[i][j] / 3.0;
  }
  
  // do stereo using mrf
  SDoublePlane disp3 = unary_stereo(image1, image2);
  SDoublePlane output(disp3.rows(), disp3.cols());
  //Displaying the dispairities and appropriate greyscale
  for(int i=0;i<disp3.rows();i++)
   for(int j=0;j<disp3.cols();j++)
	{
		output[i][j]=disp3[i][j]*256.0/30.0;
	}
	
  
  SImageIO::write_png_file("naive_stereo.png", output, output, output);
  
  //Mrf inference using loopy belief
  SDoublePlane loopy = mrf_loopy(disp3 );
  SImageIO::write_png_file("loopy_mrf_stereo.png", loopy, loopy, loopy);

  // Measure error with respect to ground truth, if we have it...
  if(gt_filename != "")
    {
      double err=0;
      for(int i=0; i<gt.rows(); i++)
	for(int j=0; j<gt.cols(); j++)
	  err += sqrt((disp3[i][j] - gt[i][j])*(disp3[i][j] - gt[i][j]));

      cout << "MRF stereo technique mean error = " << err/gt.rows()/gt.cols() << endl;

    }

  return 0;
}
