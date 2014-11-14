#include <SImage.h>
#include <SImageIO.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include <Harris.h>
#include <Feature_det.h>
#include <ProjTransform.h>



using namespace std;

// The simple image class is called SDoublePlane, with each pixel represented as
// a double (floating point) type. This means that an SDoublePlane can represent
// values outside the range 0-255, and thus can represent gradient magnitudes,
// harris corner scores, etc. 
//
// The SImageIO class supports reading and writing PNG files. It will read in
// a color PNG file, convert it to grayscale, and then return it to you in 
// an SDoublePlane. The values in this SDoublePlane will be in the range [0,255].
//
// To write out an image, call write_png_file(). It takes three separate planes,
// one for each primary color (red, green, blue). To write a grayscale image,
// just pass the same SDoublePlane for all 3 planes. In order to get sensible
// results, the values in the SDoublePlane should be in the range [0,255].
//

// Below are two helper functions that overlay rectangles / circles 
// on an image plane for visualization purpose. 

// Draws a rectangle on an image plane, using the specified gray level value and line width.
//
void overlay_rectangle(SDoublePlane &input, int _top, int _left, int _bottom, int _right, double graylevel, int width)
{
  for(int w=-width/2; w<=width/2; w++) {
    int top = _top+w, left = _left+w, right=_right+w, bottom=_bottom+w;

    // if any of the coordinates are out-of-bounds, truncate them 
    top = min( max( top, 0 ), input.rows()-1);
    bottom = min( max( bottom, 0 ), input.rows()-1);
    left = min( max( left, 0 ), input.cols()-1);
    right = min( max( right, 0 ), input.cols()-1);
      
    // draw top and bottom lines
    for(int j=left; j<=right; j++)
	  input[top][j] = input[bottom][j] = graylevel;
    // draw left and right lines
    for(int i=top; i<=bottom; i++)
	  input[i][left] = input[i][right] = graylevel;
  }
}




struct Coordinate
{  
  int row, col;
};



// Definition for a descriptor.
// Add additional fields if necessary, and change DESCRIPTOR_LENGTH depending on the design of your descriptor.
// 
const int DESCRIPTOR_LENGTH = 32; 
struct Descriptor 
{
  double vector[DESCRIPTOR_LENGTH];
};


// Find corners in an image.
//
vector<Coordinate> find_corners(const SDoublePlane &input)
{
  // Implement a Harris corner detector

 vector<Coordinate> points;

	int rows1 = input.rows();
	int cols1 = input.cols();
	double max = 0.0;	

	SDoublePlane output(rows1,cols1);

	//find max
	for(int i=0; i<rows1; i++)
	{
		for(int j=0; j<cols1; j++)
		{
			if(input[i][j] > max)
				max = input[i][j];
		}
	}

	//Threshhold and nonmaxima suppression
	for(int i=12; i<rows1-12; i++)
	{
		for(int j=12; j<cols1-12; j++)
		{
		 bool flag = 1;	
				if(input[i][j] > 0.05*max)
				{
					for(int k =0; k < 13; k++)
					{
						for(int l=0;l < 13;l++)
						{
							if(input[i][j] < input[i-k][j-l] && input[i][j] < input[i+k][j+l])
							flag = 0;
						}
					}				
					if(flag==1)
					{
						Coordinate c;
   						c.row = i;
   						c.col = j;
						points.push_back(c);
					}
				}
		}

	}	

  return points;
}


// Compute an invarient descriptor for each feature point.


vector<Descriptor> invariant_descriptors(const SDoublePlane &input,const vector<Coordinate> &point,const SDoublePlane &theta)
// const vector<Coordinate> &points)
{
vector<Descriptor> descriptors;
 	
 	for(int i = 0; i < point.size(); i++) {
	Descriptor decriptor;
		/*int a=point.size();
		cout<<a<<endl;*/
                int r=point[i].row;
		//cout<<"row  "<<r<<endl;
		int c=point[i].col;
		//cout<<"col  "<<c<<endl;

		int count1=0,count2=0,count3=0,count4=0, count5=0,count6=0,count7=0,count8=0,count9=0,count10=0,count11=0,count12=0,count13=0,count14=0,count15=0,count16=0;
		int count17=0,count18=0,count19=0,count20=0, count21=0,count22=0,count23=0,count24=0,count25=0,count26=0,count27=0,count28=0,count29=0,count30=0,count31=0,count32=0;

	//taking the 8*8 matrix at bottom right

		for(int j=r; j<(r+8);j++)
		{
			for(int k=c; k<(c+8);k++)
			{
				if((-180.0) <= theta[j][k] <= (-135.0))
					++count1;
				else if((-135.0)<theta[j][k]<=(-90.0))	
					++count2;
				else if((-90.0)<theta[j][k]<(-45.0))
					++count3;
				else if((-45.0)<theta[j][k]<=0.0)
					++count4;
				else if(0.0<theta[j][k]<=45.0)
					++count5;
				else if(45.0<theta[j][k]<=90.0)
					++count6;
				else if(90.0<theta[j][k]<=135.0)
					++count7;
				else if(135.0<theta[j][k]<=180.0)
					++count8;
			}
		}
		decriptor.vector[0]=count1;
		decriptor.vector[1]=count2;
		decriptor.vector[2]=count3;
		decriptor.vector[3]=count4;
		decriptor.vector[4]=count5;
		decriptor.vector[5]=count6;
		decriptor.vector[6]=count7;
		decriptor.vector[7]=count8;

      //taking the 8*8 matrix at top left

		for(int j=(r-8); j<r;j++)
		{
			for(int k=(c-8); k<c;k++)
			{
				if((-180.0) <= theta[j][k] && theta[j][k] <= (-135.0))
					++count9;
				else if((-135.0)<theta[j][k]<=(-90.0))	
					++count10;
				else if((-90.0)<theta[j][k]<(-45.0))
					++count11;
				else if((-45.0)<theta[j][k]<=0.0)
					++count12;
				else if(0.0<theta[j][k]<=45.0)
					++count13;
				else if(45.0<theta[j][k]<=90.0)
					++count14;
				else if(90.0<theta[j][k]<=135.0)
					++count15;
				else if(135.0<theta[j][k]<=180.0)
					++count16;
			}
		}

		decriptor.vector[8]=count9;
		decriptor.vector[9]=count10;
		decriptor.vector[10]=count11;
		decriptor.vector[11]=count12;
		decriptor.vector[12]=count13;
		decriptor.vector[13]=count14;
		decriptor.vector[14]=count15;
		decriptor.vector[15]=count16;

    		//taking the 8*8 matrix at left bottom
        	for(int j=r; j<(r+8);j++)
		{
			for(int k=(c-8); k<c;k++)
			{
				if((-180.0) <= theta[j][k] <= (-135.0))
					++count17;
				else if((-135.0)<theta[j][k]<=(-90.0))	
					++count18;		
				else if((-90.0)<theta[j][k]<(-45.0))
					++count19;
				else if((-45.0)<theta[j][k]<=0.0)
					++count20;
				else if(0.0<theta[j][k]<=45.0)
					++count21;
				else if(45.0<theta[j][k]<=90.0)
					++count22;
				else if(90.0<theta[j][k]<=135.0)
					++count23;
				else if(135.0<theta[j][k]<=180.0)
					++count24;
			}
		}

		decriptor.vector[16]=count17;
		decriptor.vector[17]=count18;
		decriptor.vector[18]=count19;
		decriptor.vector[19]=count20;
		decriptor.vector[20]=count21;
		decriptor.vector[21]=count22;
		decriptor.vector[22]=count23;
		decriptor.vector[23]=count24;

        //taking the 8*8 matrix at right top
		for(int j=(r-8); j<r;j++)
		{
			for(int k=c; k<(c+8);k++)
			{
				if((-180.0) <= theta[j][k] <= (-135.0))
					++count25;
				else if((-135.0)<theta[j][k]<=(-90.0))	
					++count26;
				else if((-90.0)<theta[j][k]<(-45.0))
					++count27;
				else if((-45.0)<theta[j][k]<=0.0)
					++count28;
				else if(0.0<theta[j][k]<=45.0)
					++count29;
				else if(45.0<theta[j][k]<=90.0)
					++count30;
				else if(90.0<theta[j][k]<=135.0)
					++count31;
                		else if(135.0<theta[j][k]<=180.0)
					++count32;
			}
		}

		decriptor.vector[24]=count25;
		decriptor.vector[25]=count26;
		decriptor.vector[26]=count27;
		decriptor.vector[27]=count28;
		decriptor.vector[28]=count29;
		decriptor.vector[29]=count30;
		decriptor.vector[30]=count31;
		decriptor.vector[31]=count32;

	/*	for(int r=0;r<32;r++)	{
			cout<<"  des "<<r<<" ";
			cout<<decriptor.vector[r]<<endl;
		}*/
	
	
	
	descriptors.push_back(decriptor);
   }

  return descriptors;
}

//2nd technique to find corresponding features
vector<Coordinate> correspond(const vector<Coordinate> icord1,const vector<Descriptor> id1,const vector<Coordinate> icord2,const vector<Descriptor> id2)
{
	 vector<Coordinate> corrs;
	 for(int i=0; i<icord1.size();i++)
	 {  
	    double min=5000.0;	
	    int index=0;
	     
	    double diff_vec[icord2.size()];

	for(int j=0; j<icord2.size();i++)
	{
	// double diff_vec[icord2.size()];
	 double diff=0;
	 
	 for (int k=0;k<32;k++)
	  {
		     diff=diff+45;
		     //ABS(icord1[i].id1[k]-icord2[j].id2.vector[k]);
	  }

	 ++index;
	 diff_vec[index]=diff;
		  

	}
	 int min_ind=0;
	 for(int m=0;m<icord2.size();m++)
	{
		 if(diff_vec[index]<min) 
	 {
	 min=diff_vec[index] ;
	 min_ind=index;
	 }
	}

	//pushing in img1 feature
	   Coordinate cd_img1;
	   cd_img1.row =icord1[i].row;
	   cd_img1.col =icord1[i].col ;
	   corrs.push_back(cd_img1);
	  // cout<<" cd_img1 row and col "<<cd_img1.row<<"  "<<cd_img1.col<<endl;

	 //pushing in img2 feature that corresponds
	   Coordinate cd_img2;
	   cd_img2.row =icord2[min_ind].row ;
	   cd_img2.col =icord2[min_ind].col;
	   corrs.push_back(cd_img2);   
	   //cout<<" cd_img2 row and col "<<cd_img2.row<<"  "<<cd_img2.col<<endl;

	  }

	return corrs;
}


//calculate mean of a pixel using 10X10 window
double cal_mean(int row,int col,const SDoublePlane &intensity){

	int count=0;
	double abc=0.0;
	
	if(((row - 3) > 0) && ((col - 3) > 0) && ((row + 3) < intensity.rows()) && ((col + 3)  < intensity.cols()))
	{
	
	 for (int i=(row-3);i<(row+3); i++){
	  for(int j=(col-3);j<(col+3); j++)
	  {
		abc=abc+intensity[i][j];
		++count;
	  }
	}
	abc=abc/count;

	return abc;
	}
	else
	return 0;
}


//2nd technique to find corresponding features
vector<Coordinate> NCC(const vector<Coordinate> &icord1,const vector<Descriptor> &ides1,const vector<Coordinate> &icord2,const vector<Descriptor> ides2,const SDoublePlane &inten1,const SDoublePlane &inten2)
{
	cout<<"there we are "<<endl;
	//we want to create pairs such that point at corrs 1 and corrs 2 are matches
	vector<Coordinate> corrs;

	double mean1=0,mean2=0;
	double a=0,b=0,c=0;

	 for(int i=0; i<icord1.size();i++)
	{	

	double maximum=0;
	//cout<<"cord 1 is taken"<<endl;
	int max_ind;
	//cout<<"cord 1 is taken"<<endl;
	

	double dis[icord2.size()];
		

	 for(int j=0; j<icord2.size();j++)
	{
		a= inten1[icord1[i].row][icord2[i].col]-cal_mean(icord1[i].row,icord1[i].col,inten1);
		//a= cal_mean(icord1[i].row,icord1[i].col,inten1);
		b= inten2[icord2[j].row][icord2[j].col]-cal_mean(icord2[j].row,icord2[j].col,inten2);
		
	double c1=a*a;
	double c2=b*b;
	double c3=sqrt(c1*c2);
	double x=(a*b)/c3;
	dis[j]=x;
		if(dis[j] > maximum)
		{
			maximum = dis[j];
			max_ind = j;
		}
	}
	

	//pushing in img1 feature
	   Coordinate cd_img1;
	   cd_img1.row =icord1[i].row;
	   cd_img1.col =icord1[i].col ;
	   corrs.push_back(cd_img1);
	 //  cout<<" cd_img1 row and col "<<cd_img1.row<<"  "<<cd_img1.col<<endl;

	//pushing in img2 feature that corresponds
	   Coordinate cd_img2;
	   cd_img2.row =icord2[max_ind].row ;
	   cd_img2.col =icord2[max_ind].col;
	   corrs.push_back(cd_img2);   
	   // cout<<" cd_img2 row and col "<<cd_img2.row<<"  "<<cd_img2.col<<endl;
	}
	

	return corrs;
}


//Trivial Ransac assuming that the image is just translated. Parameter corrs is calculated from NCC
/*SDoublePlane Ransac(vector<Coordinate> corrs) {
		SDoublePlane matrix(4,4);
	int s=corrs.size();
		int k=0,count=0;
	double a=0.0,b=0.0;

	for(int y=0; y<5000; y++)
	{    
	 if ( (0<=(corrs[k].row-corrs[k+2].row)<20 || 0<=(corrs[k].col-corrs[k+2].col)<20) && (0<=(corrs[k+1].row-corrs[k+3].row)<20 || 0<=(corrs[k].col-corrs[k+3].col)<20))
	   {
	 if( (0<=(corrs[k+2].row-corrs[k+4].row)<20 || 0<=(corrs[k+2].col-corrs[k+].col)<20) && (0<=(corrs[k+3].row-corrs[k+5].row)<20 || 0<=(corrs[k+3].col-corrs[k+5].col<20))
	 {
	  if ( (0<=(corrs[k+4].row-corrs[k+6].row)<20 || 0<=(corrs[k+4].col-corrs[k+6].col)<20) && (0<=(corrs[k+5].row-corrs[k+7].row)<20 || 0<=(corrs[k+5].col-corrs[k+7].col)<20))
	{
	 if ( (0<=(corrs[k+6].row-corrs[k+8].row)<20 || 0<=(corrs[k+6].col-corrs[k+8].col)<20)	&& (0<=(corrs[k+7].row-corrs[k+9].row)<20 || 0<=(corrs[k+7].col-corrs[k+9].col)<20))
	    {
	    cout<<"some colinear points here";
	    a= a+ ((corr[k].row-corr[k+1].row)+(corr[k+2].row-corr[k+3].row)+(corr[k+4].row-corr[k+5].row));
	    b= b+ ((corr[k].col-corr[k+1].col)+(corr[k+3].col-corr[k+5].col)+(corr[k+5].col-corr[k+7].col));
	    ++count;  
	     }
	}
		 }	
		            }
	   
		  k=k+2;
		  
	     }

	 a=a/count;
	 b=b/count;
	 

	 matrix[0][0]=1;
	 matrix[0][1]=0;
	 matrix[0][2]=a;

	 matrix[1][0]=0;
	 matrix[1][1]=1;
	 matrix[1][2]=b;

	 matrix[2][0]=0;
	 matrix[2][1]=0;
	 matrix[2][2]=1;

	return matrix;
}
*/

//function to calculate intensity using image gradients
SDoublePlane intensity(const SDoublePlane ix2, const SDoublePlane iy2)
{
	 //SDoublePlane result(ix2.rows()+1,ix2.rows()+1);
	 SDoublePlane sq_root(ix2.rows(),ix2.cols());
	 int a=ix2.rows();
	 int b=ix2.cols();
	 double sum=0.0;


	  for(int i=0;i<a;i++){
	   for(int j=0;j<b;j++){
	    sum=ix2[i][j]+iy2[i][j];
	    sq_root[i][j]=sqrt(sum);
	    }
	   }

	return sq_root;
}

// Match the query image with a retrieval set.
// 
//SDoublePlane compute_homography(const SDoublePlane &image1, const SDoublePlane &image2, const SDoublePlane &intensity,const SDoublePlane &theta)
SDoublePlane compute_homography(SDoublePlane &image1, SDoublePlane &image2)
{
	//Perform step 2 and 3 on image 1
 	 SDoublePlane R1 = findR(image1);
	 vector<Coordinate> image1_coordinates = find_corners(R1);

	SDoublePlane xgrad = CalXgrad(image1);
	SDoublePlane ygrad = CalYgrad(image1);

	SDoublePlane theta = find_theta(xgrad,ygrad);
	 vector<Descriptor> image1_descriptors = invariant_descriptors(image1,image1_coordinates,theta);

	SDoublePlane inten1 = intensity(multiply(xgrad,xgrad),multiply(ygrad,ygrad));
	
	 SDoublePlane green_plane1 = image1;
	 for(int i = 0; i<image1_coordinates.size(); i++) 
	   {
	   int top = image1_coordinates[i].row, left = image1_coordinates[i].col;
	   overlay_rectangle(green_plane1, top, left, top+6, left+6, 255, 3);
	   }
	   SImageIO::write_png_file("debug1.png", image1, green_plane1, image1);
	
	//Perform step 2 and 3 on image 2
	 SDoublePlane R2 = findR(image2);
	 vector<Coordinate> image2_coordinates = find_corners(R2);


	SDoublePlane x2grad = CalXgrad(image2);
	SDoublePlane y2grad = CalYgrad(image2);

	SDoublePlane theta2 = find_theta(x2grad,y2grad);
	
	vector<Descriptor> image2_descriptors = invariant_descriptors(image2,image2_coordinates,theta2);

	SDoublePlane inten2 = intensity(multiply(x2grad,x2grad),multiply(y2grad,y2grad));
	
	 SDoublePlane green_plane2 = image2;
	 for(int i = 0; i<image2_coordinates.size(); i++)
	   {
	   int top = image2_coordinates[i].row, left = image2_coordinates[i].col;
	   overlay_rectangle(green_plane2, top, left, top+6, left+6, 255, 3);
	   }
	   SImageIO::write_png_file("debug2.png", image2, green_plane2, image2);

	vector<Coordinate> ncc_op = NCC(image1_coordinates, image1_descriptors,image2_coordinates, image2_descriptors, inten1, inten2);
	//vector<Coordinate> compare=correspond(image1_coordinates, image1_descriptors,image2_coordinates, image2_descriptors);
	
	//returning random matrix 
	 return SDoublePlane(3,3);
}



int main(int argc, char *argv[])
{
  if(!(argc >= 3)) {
    cerr << "usage: " << argv[0] << " image_file1 image_file2 ..." << endl;
    return 1;
}

  int image_count = argc - 2;
	cout << "image count: ";
	cout << image_count;
  string image1_filename(argv[1]);
  SDoublePlane image1= SImageIO::read_png_file(image1_filename.c_str());

//step 2 Harris Corner Detection
SDoublePlane R = findR(image1);

vector<Coordinate> image1_coordinates = find_corners(R); 

 SDoublePlane green_plane = image1;
  for(int i = 0; i<image1_coordinates.size(); i++) {
    int top = image1_coordinates[i].row, left = image1_coordinates[i].col;
    overlay_rectangle(green_plane, top, left, top+6, left+6, 255, 3);
  }

SImageIO::write_png_file("hcorner.png", image1 , green_plane , image1);

//step 3 extract feature

SDoublePlane xgrad = CalXgrad(image1);
SDoublePlane ygrad = CalYgrad(image1);



// define your double array here
SDoublePlane theta = find_theta(xgrad,ygrad);

vector<Descriptor> descriptors = invariant_descriptors(image1, image1_coordinates,theta);




//step 4 projective transformation

SDoublePlane tout = transform(image1);

SImageIO::write_png_file("tout.png", tout, tout , tout);

//step 5 Compare features


SDoublePlane inten = intensity(multiply(xgrad,xgrad),multiply(ygrad,ygrad));


 for(int i=1; i<=image_count; i++)
    {

      // read in other images; figure out homography between the first image and each additional one
      string image2_filename(argv[i+1]);
	
      SDoublePlane image2= SImageIO::read_png_file(image2_filename.c_str());

     SDoublePlane H = compute_homography(image1, image2);


      // here you'll want to fill in calls to apply the inverse homography and write out the file
    }

  
  return 0;
}
