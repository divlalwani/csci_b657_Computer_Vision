#include <SImage.h>
#include <SImageIO.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <math.h>
#include <limits.h>

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

class Point
{
public:
  Point() { }
  Point(int _r, int _c) : row(_r), col(_c) { }
  int row, col;
};

struct Message
{
	int i; //row
	int j; //col
	int dir; //direction
	int label; //label
};

//declare global variables for mean and std deviation
double mR = 0.0;
double mG = 0.0;
double mB = 0.0;
double s1 = 0.0;
double s2 = 0.0;
double s3 = 0.0;

//calculate mean and standard deviation for a given foreground stroke
//return result i.e. foreground nad background pixels, so that it could be reused
SDoublePlane calMeanStd(const SDoublePlane *img, const vector<Point> &fg, const vector<Point> &bg)
{

	const SDoublePlane &red_plane = img[0], &green_plane = img[1], blue_plane = img[2];
	SDoublePlane result(red_plane.rows(), red_plane.cols());
	for(int i=0; i<red_plane.rows(); i++)
	    for(int j=0; j<red_plane.cols(); j++)
			result[i][j] = 100;
	
	//foreground seed pixels are 1
	  for(vector<Point>::const_iterator iter = fg.begin(); iter != fg.end(); ++iter)
	    result[iter->row][iter->col] = 1;

	  //background seed pixels are 1
	  for(vector<Point>::const_iterator iter = bg.begin(); iter != bg.end(); ++iter)
	    result[iter->row][iter->col] = 0;

	//calculate mean
	double sumR = 0.0, sumG = 0.0, sumB = 0.0;
	SDoublePlane std1(red_plane.rows(), red_plane.cols());
	SDoublePlane std2(green_plane.rows(), green_plane.cols());
	SDoublePlane std3(blue_plane.rows(), blue_plane.cols());

	for(int i=0; i<red_plane.rows(); i++)
	{
		for(int j=0; j<red_plane.cols(); j++)
		{
			if(result[i][j] == 1)
			{
			sumR +=red_plane[i][j];
			sumG +=green_plane[i][j];
			sumB +=blue_plane[i][j];
			}
		}
	}
	
	mR = sumR/(red_plane.rows() * red_plane.cols());
	mG = sumG/(red_plane.rows() * red_plane.cols());
	mB = sumB/(red_plane.rows() * red_plane.cols());


	//calculate std deviation
	for(int i=0; i<red_plane.rows(); i++)
	{
		for(int j=0; j<red_plane.cols(); j++)
		{
			if(result[i][j] == 1)
			{
			std1[i][j] = (mR - red_plane[i][j]) * (mR - red_plane[i][j]);
			std2[i][j] = (mG - green_plane[i][j]) * (mG - green_plane[i][j]);
			std3[i][j] = (mB - blue_plane[i][j]) * (mB - blue_plane[i][j]);
			}
		}
	}
	
	sumR = sumG = sumB = 0.0;

	for(int i=0; i<red_plane.rows(); i++)
	{
		for(int j=0; j<red_plane.cols(); j++)
		{
			
			if(result[i][j] == 1)
			{
				sumR +=std1[i][j];
				sumG +=std2[i][j];
				sumB +=std3[i][j];
			}
		}
	}
	
	s1 = sqrt(sumR/(red_plane.rows() * red_plane.cols()));
	s2 = sqrt(sumG/(red_plane.rows() * red_plane.cols()));
	s3 = sqrt(sumB/(red_plane.rows() * red_plane.cols()));

       return result;
       //result is returned so that this array could be reused
}
	
//calculate D(L(p),I(p)) 
double DCost(const SDoublePlane &red_plane,const SDoublePlane &green_plane,const SDoublePlane &blue_plane,const SDoublePlane &result,int r,int c,int label)
{
	//const SDoublePlane &red_plane = img[0], &green_plane = img[1], blue_plane = img[2];		
	//SDoublePlane result(red_plane.rows(), red_plane.cols());

	double dist = 0.0;	
	double beta = 3930;
	
	//if pixel belongs to background
	  if(result[r][c] == 0)
	  {
		if(label == 0)
			dist = 0;
		else
			dist = UINT_MAX;
	  }
	//if pixel belongs to foreground
	  else if(result[r][c] == 1)
	  {
		if(label == 1)
			dist == 0;
		else 
			dist = UINT_MAX;
	  }
	  else //if pixel doen't belong to foreground or background
	  {
		if(label == 0)
			dist = beta;
		else
			//Guassian probability density function
			dist = ((1/s1)*(red_plane[r][c] - mR) * (red_plane[r][c] - mR)) + ((1/s2)*(green_plane[r][c] - mG) * (green_plane[r][c] - mG)) + ((1/s3)*(blue_plane[r][c] - mB) * (blue_plane[r][c] - mB));
	  }		
	return dist;		

}


//naive_segmentation depends only on D(L(p),I(p)) 
//compute dist for label 0 and 1 and assign the label with minimum dist to pixel i,j
SDoublePlane naive_segment(const SDoublePlane *img, const vector<Point> &fg, const vector<Point> &bg,const SDoublePlane &result)
{
	
	const SDoublePlane &red_plane = img[0], &green_plane = img[1], &blue_plane = img[2];
	SDoublePlane label(red_plane.rows(), red_plane.cols());
	
	
	
	for(int i=0; i<red_plane.rows(); i++)
	{
		for(int j=0; j<red_plane.cols(); j++)
		{
				double min = UINT_MAX;
				double dist;
				for(int l=0; l < 2; l++)
				{
					 dist = DCost(img[0],img[1],img[2],result,i,j,l);	
					if(dist < min)
					{
						min = dist;
						label[i][j] = l;
					}
				}
		}
	}
	return label;
}
			
				
			
	
 
//pairwise cost function for MRF
int VCost(int label1,int label2)
{
	return (label1 - label2)*(label1 - label2);
}



//implement MRF segmentation for better results
SDoublePlane mrf_segment(const SDoublePlane *img, const vector<Point> &fg, const vector<Point> &bg,const SDoublePlane &result)
{
	const SDoublePlane &red_plane = img[0], &green_plane = img[1], &blue_plane = img[2];
	SDoublePlane label(red_plane.rows(), red_plane.cols());


	//three dimenisonal array
	//1st and 2nd dimension : row and column of pixel
	//3rd dimension : labels (0 and 1)
	//4th hidden dimension : directions (4 directions + hidden node)

	int dim1 = red_plane.rows(); int dim2 = red_plane.cols(); int dim3 = 2;
	int*** msg;
	
	// initialize msgs to 0;
	msg = new int**[dim1];
	
	for(int x = 0; x < dim1; ++x) {
	    msg[x] = new int*[dim2];
	   
	    for(int y = 0; y < dim2; ++y) {
		msg[x][y] = new int[dim3];
		
		for(int z = 0; z < dim3; ++z) { 
		    msg[x][y][z] = 0;
			
		}
	    }
	}

	
	//int updatedMsg[2]; //for each label

	for(int iter = 0; iter < 5; iter++)
	{

		for(int i=1; i<red_plane.rows()-1; i++)
		{
			for(int j=1; j<red_plane.cols()-1; j++)
			{
				for(int k=0; k < 5; k++)
				{
					double min = UINT_MAX;
					int ll = 0;
					for(int l=0; l < 2; l++)
					{
						double dist = DCost(img[0],img[1],img[2],result,i,j,l);
						for(int l2 = 0; l2 < 2; l2++)
						{
							dist += 3*VCost(l,l2);
							//itself
							if(k == 0)
								dist += msg[i+1][j][l2] + msg[i-1][j][l2] + msg[i][j-1][l2] + msg[i][j+1][l2];
							//RIGHT	
							if(k == 1)
								dist += msg[i][j][l2] + msg[i-1][j][l2] + msg[i][j-1][l2] + msg[i][j+1][l2];
							//LEFT
							else if(k == 2)
								dist += msg[i][j][l2] + msg[i+1][j][l2] + msg[i][j-1][l2] + msg[i][j+1][l2];
							//TOP
							else if(k == 3)
								dist += msg[i][j][l2] + msg[i-1][j][l2] + msg[i+1][j][l2] + msg[i][j+1][l2];
							//BOTTOM
							else if(k == 4)
								dist += msg[i][j][l2] + msg[i-1][j][l2] + msg[i+1][j][l2] + msg[i][j-1][l2];


						}
						//find minimum cost
						if(dist < min)
						{
							min = dist;
							ll = l;
						}
						msg[i][j][l] = min; 
					}
						if(k == 0)
							msg[i][j][ll] = min;
						else if(k == 1)
							msg[i+1][j][ll] = min;
						else if(k == 2)
							msg[i-1][j][ll] = min;
						else if(k == 3)
							msg[i][j-1][ll]  = min;
						else if(k == 4)
							msg[i][j+1][ll] = min;

				}
			}
		}


		//assign labels
		for(int i=1; i<red_plane.rows()-1; i++)
		{
			for(int j=1; j<red_plane.cols()-1; j++)
			{	
				double min = UINT_MAX;
				for(int l = 0; l < 2; l++)
				{	
					double dist = 0;
					dist += msg[i][j][l] + msg[i-1][j][l] + msg[i+1][j][l] + msg[i][j-1][l] + msg[i][j+1][l];

					if(dist < min)
					{
						min = dist;
						label[i][j] = l;
						//cout << label[i][j];
					}
				}
			}
		}
					
	}
	return label;		
		
}



// Take in an input image and a binary segmentation map. Use the segmentation map to split the 
//  input image into foreground and background portions, and then save each one as a separate image.
//
// This code should work okay already, but feel free to modify if you want.
//
void output_segmentation(const SDoublePlane *img, const SDoublePlane &labels, const string &fname)
{
  // sanity checks. If one of these asserts fails, you've given this function invalid arguments!
  assert(img[0].rows() == labels.rows());
  assert(img[0].cols() == labels.cols());

  SDoublePlane img_fg[3], img_bg[3];

  for(int i=0; i<3; i++)
    {
      img_fg[i] = img[i];
      img_bg[i] = img[i];
    }

  for(int i=0; i<labels.rows(); i++)
    for(int j=0; j<labels.cols(); j++)
      {
	if(labels[i][j] == 0)
	  img_fg[0][i][j] = img_fg[1][i][j] = img_fg[2][i][j] = 0; 
	else if(labels[i][j] == 1)
	  img_bg[0][i][j] = img_bg[1][i][j] = img_bg[2][i][j] = 0;
	else
	  assert(0);
      }

  SImageIO::write_png_file((fname + "_fg.png").c_str(), img_fg[0], img_fg[1], img_fg[2]);
  SImageIO::write_png_file((fname + "_bg.png").c_str(), img_bg[0], img_bg[1], img_bg[2]);
}

int main(int argc, char *argv[])
{
  if(argc != 3)
    {
      cerr << "usage: " << argv[0] << " image_file seeds_file" << endl;
      return 1;
    }

  string input_filename1 = argv[1], seeds_file = argv[2];

  // read in images and gt
  SDoublePlane image_rgb[3], seeds_rgb[3]; 
  SImageIO::read_png_file_rgb(input_filename1.c_str(), image_rgb);
  SImageIO::read_png_file_rgb(seeds_file.c_str(), seeds_rgb);

  SDoublePlane result;
  // figure out seed points 
  vector<Point> fg_pixels, bg_pixels;
  for(int i=0; i<seeds_rgb[0].rows(); i++)
    for(int j=0; j<seeds_rgb[0].cols(); j++)
      {
	// blue --> foreground
	if(seeds_rgb[0][i][j] < 100 && seeds_rgb[1][i][j] < 100 && seeds_rgb[2][i][j] > 100)
	  fg_pixels.push_back(Point(i, j));

	// red --> background
	if(seeds_rgb[0][i][j] > 100 && seeds_rgb[1][i][j] < 100 && seeds_rgb[2][i][j] < 100)
	  bg_pixels.push_back(Point(i, j));
      }


  // do naive segmentation
  //find mean and std deviation and store them in global vars
  result = calMeanStd(image_rgb, fg_pixels, bg_pixels);

  SDoublePlane labels = naive_segment(image_rgb, fg_pixels, bg_pixels,result);
  output_segmentation(image_rgb, labels, "naive_segment_result");

  // do mrf segmentation
 labels = mrf_segment(image_rgb, fg_pixels, bg_pixels, result);
  output_segmentation(image_rgb, labels, "mrf_segment_result");

  return 0;
}
