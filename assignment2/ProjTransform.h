using namespace std;


//transformation
SDoublePlane transform(SDoublePlane &image1)
{
	SDoublePlane mat(3,3);
/*
	mat[0][0] = 0.907;
	mat[0][1] = 0.258;
	mat[0][2] = -182;

	mat[1][0] = -0.153;
	mat[1][1] = 1.44;
	mat[1][2] = 58;

	mat[2][0] = -0.000306;
	mat[2][1] = 0.000731;
	mat[2][2] = 1;
*/

//Code to calculate inverse of matrix
/*
	mat[0][0] = 1.124668580536120;
	mat[0][1] = -0.314676603975957;
	mat[0][2] = 222.940924688179000;

	mat[1][0] = 0.108839050641507;
	mat[1][1] = 0.685058664740780;
	mat[1][2] = -19.924695338211000;

	mat[2][0] = 0.000264587239625;
	mat[2][1] = -0.000597068924742;
	mat[2][2] =  1.082784875246820;
*/
	double a,b,c,d,e,f,g,h,i;
/*
	a = 0.907;
	b = 0.258;
	c = -182;

	d = -0.153;
	e = 1.44;
	f = 58;

	g = -0.000306;
	h = 0.000731;
	i = 1;

*/
//inverse of matrix
	a = 1.124668580536120;
	b = -0.314676603975957;
	c = 222.940924688179000;

	d = 0.108839050641507;
	e = 0.685058664740780;
	f = -19.924695338211000;

	g = 0.000264587239625;
	h = -0.000597068924742;
	i =  1.082784875246820;

	double x, y;
	//int x, y;

	SDoublePlane output(image1.rows(),image1.cols());

	cout << "Rows: ";
	cout << image1.rows();

	cout << "Cols: ";
	cout << image1.cols();	 

	//initialize all pixels to 0	
	for(int xp=0; xp<image1.rows(); xp++)
	{
		for(int yp = 0; yp<image1.cols(); yp++)
		{
			output[xp][yp] = 0;
		}
	}

	for(int xp=0; xp<image1.rows(); xp++)
	{
		for(int yp = 0; yp<image1.cols(); yp++)
		{
					//projective transformation
					y = (yp*a + xp*b + c) / (yp*g + xp*h + i);
					x = (yp*d + xp*e + f) / (yp*g + xp*h + i);
		
					int x1 = (int)floor(x);
					int y1 = (int)floor(y);

					int x2 = (int)ceil(x);
					int y2 = (int)ceil(y);
					
			//bilinear interpolation
			if(0 <= x1 && x1 < image1.rows() && 0 <= y1  && y1 < image1.cols())
			{
				//output[xp][yp] = image1[x][y];
				output[xp][yp] = (1/((x2-x1)*(y2-y1))) * 
						 ((image1[x1][y1] * (x2 - x) * (y2 - y)) + (image1[x2][y1] * (x - x1) * (y2 - y)) 
						   + (image1[x1][y2] * (x2 - x) * (y - y1)) + (image1[x2][y2] * (x - x1) * (y - y1)));

			}	
		}
	}

				
	return output;		

}

