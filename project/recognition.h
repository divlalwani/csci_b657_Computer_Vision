#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp"  
#include <iostream>  
#include <stdio.h> 

  
using namespace cv;  
using namespace std;  
  
const int train_samples = 10;  
const int classes = 10;  
const int sx = 20;  
const int sy = 30;  
const int ImageSize = sx * sy;  
char pathToImages[] = "images"; 

//struct dataarray {int dat[10][10]; };
 

//preprocess the image
void preprocess(Mat *input,Mat *output,int sx, int sy)  
{ 
	//creating different matrices for each type 
	 Mat gray,blurred,threshold,contour,roi;  
	  
	 vector<vector<Point> > contours;  
	  
	//rgb to color
	 //cout<<input.shape;
	 cvtColor(*input, gray, COLOR_BGR2GRAY);  
	 
	//gray to blurred 
	 GaussianBlur(gray, blurred, Size(5, 5), 2, 2);  

	//thresholding blurred image
	 adaptiveThreshold(blurred, threshold, 255, 1, 1, 11, 2);  
	 
	//copy to contour 
	 threshold.copyTo(contour);  
	  
	 findContours(contour, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);  
	  
	 int idx = 0;  
	 size_t area = 0;  
	 for (size_t i = 0; i < contours.size(); i++)  
	 {  
	  if (area < contours[i].size() )  
	  {  
	   idx = i;  
	   area = contours[i].size();  
	  }  
	 }  
	 //mapping the largest rectangle 
	 Rect rect = boundingRect(contours[idx]);  
	  //cropping image
	 roi = threshold(rect);  
	 //resizing image over required size of image..wrt center 
	 resize(roi,*output, Size(sx, sy)); 
	  
}  
  
//Training the images dataset
void learning(CvMat* alldata, CvMat* labels)  
{  
	Mat img;  

	char file[255];  
	int count=0;
	 //10 classes we will need for all digits
	for (int i = 0; i < 10; i++) 
	{ 
		 for (int j = 1; j < 7; j++)  
		  { 
			  // file path for imread is created
			  sprintf(file, "%s/%d%d.png", pathToImages, i, j);  
			  img = imread(file, 1); 
			 
			  if (!img.data)  
			  {  
			    cout << "File " << file << " not found"<<endl;  
			    exit(1);  
			  }  

			  Mat outfile;  
			  preprocess(&img, &outfile, sx, sy); 
			  
			  for (int n = 0; n < ImageSize; n++)  
			  {  
			   alldata->data.fl[count * ImageSize + n] = outfile.data[n];  
			  } 
			 
			  labels->data.fl[count] = i;  
			  ++count;
		 } 
	}
  
}  

//Extract each cell and recognize the digit
void recognizing(Mat image, KNearest knn, int r, int c)  
{  
	  //struct dataarray a;
	  CvMat* cvobj = cvCreateMat(1, ImageSize, CV_32FC1);  
	  int b[10][10];
	  int k=0;
	  Mat gray,blur,thresh;  
	  //Mat image = imread("images/numbers.png", 1); 
	  
	  //rgb to color
	  cvtColor(image, gray, COLOR_BGR2GRAY);  
	 
	  //gray to blurred 
	  GaussianBlur(gray, blur, Size(5, 5), 2, 2);  

	  //thresholding blurred image
	  adaptiveThreshold(blur, thresh, 255, 1, 1, 11, 2);  
	 
	  int rr=thresh.size().height/r;
	  int cc=thresh.size().width/c;
	 
	  for(int i=0;i<r;i++)
	  {
	    for(int j=0;j<c;j++)
	    {
	    
		    Rect rectcrop(cc*j, rr*i , cc, rr);
		    Mat roi = image(rectcrop);  
		    Mat matchimage;  
		    preprocess(&roi, &matchimage, sx, sy); 

		    for (int n = 0; n < ImageSize; n++)  
		    {  
		     cvobj->data.fl[n] = matchimage.data[n];  
		    }
			 
			 int result = knn.find_nearest(cvobj, 1); 
			 //a.dat[i][j]=result;
			  b[j][i] = result;	
		
				 
	    }	 
	}
	cout<<"Transpose of the displayed matrix is: ";	
	cout<<endl;
	for(int i=0;i<c;i++)
	{
		for(int j=0;j<r;j++)
		{
			cout << " " << b[i][j];
		}
		cout<<endl;
	}

	   imshow("display window", image); 
	   //added so that output could be seen
	   waitKey(0);    
	  //delete cvobj;    
	  //return a;
}  

void opencvdet(Mat image, int r, int c)  
{  
	  
	 //int b;
	 //Mat image = imread(argv[1], 1); 
	 CvMat* alldata = cvCreateMat(classes * train_samples,ImageSize, CV_32FC1);  
	 CvMat* labels = cvCreateMat(classes * train_samples, 1, CV_32FC1); 

	 learning(alldata,labels);  
	  
	 KNearest knn(alldata,labels);  
	 
	 recognizing(image,knn,r,c);  
 
}  
  

  
  

