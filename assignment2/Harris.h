#include <CommonFunctions.h>


using namespace std;


//function to calcuate response score for harris corner detection
SDoublePlane findR(const SDoublePlane &image1)
{
		SDoublePlane filter = get_gaussian(1);

		SDoublePlane filterT = transpose(filter);

		SDoublePlane gaussianout = convolve_separable(image1, filter, filterT);
		SImageIO::write_png_file("gaussian.png", gaussianout, gaussianout, gaussianout);

		SDoublePlane filterixx(1,3);
		SDoublePlane filterixy(3,1);

		SDoublePlane filteriyx(1,3);
		SDoublePlane filteriyy(3,1);

		//x gradient
		filterixx[0][0]=-1;
		filterixx[0][1]=0;
		filterixx[0][2]=1;

		filterixy[0][0]=1;
		filterixy[1][0]=2;
		filterixy[2][0]=1;

		//y gradient
		filteriyx[0][0]=1;
		filteriyx[0][1]=2;
		filteriyx[0][2]=1;

		filteriyy[0][0]=1;
		filteriyy[1][0]=0;
		filteriyy[2][0]=-1;

		SDoublePlane ix = convolve_separable(gaussianout, filterixx, filterixy);

		SDoublePlane iy = convolve_separable(gaussianout, filteriyx, filteriyy); 

		filter = get_gaussian(3);
		filterT = transpose(filter);

		SDoublePlane ix2 = multiply(ix,ix);

		SDoublePlane iy2 = multiply(iy,iy);

		SDoublePlane ixiy = multiply(ix,iy);

		SDoublePlane Six2 = convolve_separable(ix2,filter,filterT);

		SDoublePlane Siy2 = convolve_separable(iy2,filter,filterT);

		SDoublePlane Sixiy = convolve_separable(ixiy,filter,filterT);

		SDoublePlane AB = multiply(Six2,Siy2);

		SDoublePlane C2 = multiply(Sixiy,Sixiy);

		SDoublePlane Det = sub(AB,C2);

		SDoublePlane Trace = kTimesTrace(0.05,Six2,Siy2);

		SDoublePlane R = sub(Det, Trace);

		return R;

}



