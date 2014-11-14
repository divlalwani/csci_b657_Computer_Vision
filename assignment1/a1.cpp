#include <SImage.h>
#include <SImageIO.h>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <DrawText.h>
#include <Convolution.h>
#include <CommonFunctions.h>

#include <HammingDistance.h>
#include <DFunction.h>



using namespace std;

// The simple image class is called SDoublePlane, with each pixel represented as
// a double (floating point) type. This means that an SDoublePlane can represent
// values outside the range 0-255, and thus can represent squared gradient magnitudes,
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

// Below is a helper functions that overlays rectangles
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

// DetectedSymbol class may be helpful!
//  Feel free to modify.
//
typedef enum {NOTEHEAD=0, QUARTERREST=1, EIGHTHREST=2} Type;
class DetectedSymbol {
public:
  int row, col, width, height;
  Type type;
  char pitch;
  double confidence;
};

// Function that outputs the ascii detection output file
void  write_detection_txt(const string &filename, const vector<struct DetectedSymbol> &symbols)
{
  ofstream ofs(filename.c_str());

  for(int i=0; i<symbols.size(); i++)
    {
      const DetectedSymbol &s = symbols[i];
      ofs << s.row << " " << s.col << " " << s.width << " " << s.height << " ";
      if(s.type == NOTEHEAD)
	ofs << "filled_note " << s.pitch;
      else if(s.type == EIGHTHREST)
	ofs << "eighth_rest _";
      else 
	ofs << "quarter_rest _";
      ofs << " " << s.confidence << endl;
    }
}

// Function that outputs a visualization of detected symbols
void  write_detection_image(const string &filename, const vector<DetectedSymbol> &symbols, const SDoublePlane &input)
{
  SDoublePlane output_planes[3];
  for(int i=0; i<3; i++)
    output_planes[i] = input;

  for(int i=0; i<symbols.size(); i++)
    {
      const DetectedSymbol &s = symbols[i];

      overlay_rectangle(output_planes[s.type], s.row, s.col, s.row+s.height-1, s.col+s.width-1, 255, 2);
      //overlay_rectangle(output_planes[(s.for(int i=0; i < rows; ++i)  type+1) % 3], s.row, s.col, s.row+s.height-1, s.col+s.width-1, 0, 2);
      overlay_rectangle(output_planes[(s.type+2) % 3], s.row, s.col, s.row+s.height-1, s.col+s.width-1, 0, 2);

      if(s.type == NOTEHEAD)
	{
	  char str[] = {s.pitch, 0};
	  draw_text(output_planes[0], str, s.row, s.col+s.width+1, 0, 2);
	  draw_text(output_planes[1], str, s.row, s.col+s.width+1, 0, 2);
	  draw_text(output_planes[2], str, s.row, s.col+s.width+1, 0, 2);
	}
    }

  SImageIO::write_png_file(filename.c_str(), output_planes[0], output_planes[1], output_planes[2]);
}

int detectedlines[20];


void hough(const SDoublePlane &input)
{
	int rows=input.rows();
	int cols=input.cols();
	int count=0;
	// SDoublePlane hough_mat(rows,cols);
	int hough_mat[rows];

	for(int i = 0; i < rows; i++)
	{
		for(int j=0; j < cols; j++)
		{
			hough_mat[i]=0;
		}
	}


	//Edges detected from Sobel and on conversion to binary are white
	for(int i = 1; i < (rows-1); i++)
	{
		int numwhite=0;
		int numblack=0;
		for(int j=0; j < (cols); j++)
		{
			if(input[i][j]==255)
			++numwhite;
			else
			++numblack;

		}

		if(numwhite>numblack)
		{
			
			++hough_mat[i];
			
		}

		else
		continue;

	}

//bool flag1=false;

	int j=0;
	for(int i=0;i<(rows-1);i++)
	{

		if(hough_mat[i]==1 && hough_mat[i]!=hough_mat[i+1])
		{
		detectedlines[j]=i+1;
		j++;
		
		}

		else
		continue;

	}


}








//
// This main file just outputs a few test images. You'll want to change it to do 
//  something more interesting!
//
int main(int argc, char *argv[])
{
  if(!(argc == 2))
    {
      cerr << "usage: " << argv[0] << " input_image" << endl;
      return 1;
    }

  string input_filename(argv[1]);
  string input_templatename1("template1.png"); //new
  string input_templatename2("template2.png");
  string input_templatename3("template3.png");
  SDoublePlane input_image= SImageIO::read_png_file(input_filename.c_str());
  int rows=input_image.rows();
  int cols=input_image.cols();
 // SDoublePlane input_image= SImageIO::read_png_file(input_filename.c_str());

  SDoublePlane template_image1= SImageIO::read_png_file(input_templatename1.c_str());
SDoublePlane template_image2= SImageIO::read_png_file(input_templatename2.c_str());
SDoublePlane template_image3= SImageIO::read_png_file(input_templatename3.c_str());
  int count = 1;
  // test step 2 by applying mean filters to the input image
 

SDoublePlane filter_convolve(3,3);
SDoublePlane filter_sobelx(3,3);
SDoublePlane filter_sobely(3,3);
 
//generate filter for convolution of step 2
for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
{
      filter_convolve[i][j] = 1/9.0;
	

}

//x gradient of sobel
filter_sobelx[0][0]=-1;
filter_sobelx[0][1]=0;
filter_sobelx[0][2]=1;
filter_sobelx[1][0]=-2;
filter_sobelx[1][1]=0;
filter_sobelx[1][2]=2;
filter_sobelx[2][0]=-1;
filter_sobelx[2][1]=0;
filter_sobelx[2][2]=1;

//y gradient of sobel
filter_sobely[0][0]=1;
filter_sobely[0][1]=2;
filter_sobely[0][2]=1;
filter_sobely[1][0]=0;
filter_sobely[1][1]=0;
filter_sobely[1][2]=0;
filter_sobely[2][0]=-1;
filter_sobely[2][1]=-2;
filter_sobely[2][2]=-1;



//Convolving image and template
//step 2
SDoublePlane image_con = convolve_general(input_image, filter_convolve);

SDoublePlane temp_con1 = convolve_general(template_image1, filter_convolve);
SDoublePlane temp_con2 = convolve_general(template_image2, filter_convolve);
SDoublePlane temp_con3 = convolve_general(template_image3, filter_convolve);


//step 3
 SDoublePlane seperable = convolve_separable(input_image);

//step 4
//create binary images for step 4
SDoublePlane bwinput=blackandwhite(input_image);

SDoublePlane bwtemp1=blackandwhite(template_image1);
SDoublePlane bwtemp2=blackandwhite(template_image2);
SDoublePlane bwtemp3=blackandwhite(template_image3);



//hamming distance
 SDoublePlane hamdis1=hamming(bwinput,bwtemp1);
 SDoublePlane hamdis2=hamming(bwinput,bwtemp2);
 SDoublePlane hamdis3=hamming(bwinput,bwtemp3);

double max1 = findmax(hamdis1);
double max2 = findmax(hamdis2);
double max3 = findmax(hamdis3);


hough(input_image);

vector<DetectedSymbol> symbols;
  
  for(int i=0; i<rows; i++)
    {
	for(int j=0;j<cols;j++)
	{
		if(hamdis1[i][j] > 0.88*max1) //template1
		{
		      DetectedSymbol s;
		      s.row = i;
		      s.col = j;
		      s.width = template_image1.cols();
		      s.height = template_image1.rows();
		      s.type = (Type)0;
		      s.confidence = 0;
		      s.pitch = (rand() % 7) + 'A';
		      symbols.push_back(s);
		}
		if(hamdis2[i][j] > 0.97*max2) //template2
		{
		      DetectedSymbol s;
		      s.row = i;
		      s.col = j;
		      s.width = template_image2.cols();
		      s.height = template_image2.rows();
		      s.type = (Type)1;
		      s.confidence = 0;
		     // s.pitch = (rand() % 7) + 'A';
		      symbols.push_back(s);
		}
		if(hamdis3[i][j] > 0.97*max3) //template3
		{
		      DetectedSymbol s;
		      s.row = i;
		      s.col = j;
		      s.width = template_image3.cols();
		      s.height = template_image3.rows();
		      s.type = (Type)2;
		      s.confidence = 0;
		     // s.pitch = (rand() % 7) + 'A';
		      symbols.push_back(s);
		}
	}
    }

//step5


//Since this step is taking a long time, we have performed this operation only on template1. Same operation can be performed on template2 and template3

//Using the sobel on image
 SDoublePlane sobelx = convolve_general(input_image, filter_sobelx);
 SDoublePlane sobely = convolve_general(input_image, filter_sobely);
 SDoublePlane imagesobel = sobel1(sobelx, sobely);

//using sobel on template
SDoublePlane tempsobelx = convolve_general(template_image1, filter_sobelx);
SDoublePlane tempsobely = convolve_general(template_image1, filter_sobely);
SDoublePlane templatesobel = sobel1(tempsobelx, tempsobely);


//use dfunction to calculate scores

// To stop step5 distance function start comment here
 SDoublePlane sobeldist = Distance(imagesobel, templatesobel);

double sobelmax = findmax(sobeldist);
vector<DetectedSymbol> symbols1;


  for(int i=0; i<rows; i++)
    {
	for(int j=0;j<cols;j++)
	{
		if(sobeldist[i][j] >= 0.95*sobelmax) //template1
		{
		      DetectedSymbol s;
		      s.row = i;
		      s.col = j;
		      s.width = template_image1.cols();
		      s.height = template_image1.cols();
		      s.type = (Type)0;
		      s.confidence = 0;
		      s.pitch = (rand() % 7) + 'A';
		      symbols1.push_back(s);
		}
	}
   }

// To stop step5 distance function end comment here

//hough

hough(input_image);

vector<DetectedSymbol> symbols2;
  
  for(int i=0; i<rows; i++)
    {
	for(int j=0 ;j<20; j++)
	{
		if(detectedlines[j] == i) //template1
		{
		      DetectedSymbol s;
		      s.row = i;
		      s.col = 20;
		      s.width = input_image.cols() - 60;
		      s.height = 5;
		      symbols2.push_back(s);
		}
	}
}
 

SImageIO::write_png_file("scores4.png", image_con, image_con, image_con);

write_detection_image("detected4.png", symbols, input_image);


SImageIO::write_png_file("edges.png", imagesobel, imagesobel, imagesobel);

// To stop step5 distance function comment here
write_detection_image("detected5.png", symbols1, input_image);

write_detection_image("staves.png", symbols2, imagesobel);

write_detection_txt("detected7.txt", symbols);

write_detection_image("detected7.png", symbols, input_image);

}
