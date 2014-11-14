// Returns a 1-d "image" containing a gaussian of the specified sigma.
// Can also provide a width; otherwise it automatically defaults to +/- 3 sigma.
// Mean of the gaussian is in the center of the "image".
//
SDoublePlane get_gaussian(double col_sigma, int width=0)
{
  if(width == 0)
    width = int(ceil(col_sigma * 3.0)) * 2 + 1;
  
  SDoublePlane out(1, width);
  
  double norm = 1.0/(col_sigma * sqrt(2*M_PI));
  int half_width = width/2;
  for(int j=0; j<width; j++)
    out[0][j] = norm * exp(-((j-half_width)*(j-half_width)) / (2.0 * col_sigma * col_sigma));

  return out;
}

SDoublePlane convolve_rows(const SDoublePlane &input, const SDoublePlane &kernel)
{
  assert(kernel.cols() > 1 && kernel.rows() == 1 && kernel.cols() % 2 == 1);

  SDoublePlane result(input.rows(), input.cols());
  double *kernel_ptr = kernel[0];

  for(int i=0; i < input.rows(); i++)
    for(int j=0; j < input.cols(); j++)
      {
	double *input_ptr = input[i];
	double sum = 0;

	for(int l=-kernel.cols()/2; l<=kernel.cols()/2; l++)
	  if(j-l >= 0 && j-l < result.cols())
	    sum += kernel_ptr[l+kernel.cols()/2] * input_ptr[j-l];
	
	result[i][j] = sum;
      }

  return result;
}

SDoublePlane transpose(const SDoublePlane &input)
{
  SDoublePlane result(input.cols(), input.rows());
  for(int i=0; i<input.rows(); i++)
    for(int j=0; j<input.cols(); j++)
      result[j][i] = input[i][j];

  return result;
}

// Convolve input with the two separable kernels and return the result.
// This implementation is not well optimized, but should be fast enough for our purposes.
// (The transposes are not necessarily a good idea, although it may be worth it so that cache
// hits are maximized during convolve_rows).
//
SDoublePlane convolve_separable(const SDoublePlane &input, const SDoublePlane &row_k, const SDoublePlane &col_k)
{
  return transpose(convolve_rows(transpose(convolve_rows(input, row_k)), transpose(col_k)));
}




SDoublePlane kTimesTrace(double k,SDoublePlane &image1, SDoublePlane &image2)
{


	int rows1 = image1.rows();
	int cols1 = image1.cols();

	int rows2 = image2.rows();
	int cols2 = image2.cols();

	SDoublePlane output(rows1,cols2);

	for(int i=0; i<rows1; i++)
	{
		for(int j=0; j<cols1; j++)
		{
			output[i][j] = k * (image1[i][j] + image2[i][j]) * (image1[i][j] + image2[i][j]);
		}
	}

return output;
}

//multiplication of 2 matrices
SDoublePlane multiply(const SDoublePlane &image1,const SDoublePlane &image2)
{
	int rows1 = image1.rows();
	int cols1 = image1.cols();

	int rows2 = image2.rows();
	int cols2 = image2.cols();

	SDoublePlane output(rows1,cols1);
	for(int i=0; i<rows1; i++)
	{
		for(int j=0; j<cols1; j++)
		{
			output[i][j] = image1[i][j] * image2[i][j];
		}
	}

return output;
}

//subtraction of 2 matrices
SDoublePlane sub(const SDoublePlane &image1,const SDoublePlane &image2)
{


	int rows1 = image1.rows();
	int cols1 = image1.cols();

	int rows2 = image2.rows();
	int cols2 = image2.cols();

	SDoublePlane output(rows1,cols1);

	for(int i=0; i<rows1; i++)
	{
		for(int j=0; j<cols1; j++)
		{
			output[i][j] = image1[i][j] - image2[i][j];
		}
	}

return output;
}

//function to calculate x gradient
SDoublePlane CalXgrad(const SDoublePlane &image1)
{
		SDoublePlane filter1 = get_gaussian(3);

	SDoublePlane filter1T = transpose(filter1);

	SDoublePlane gaussianout = convolve_separable(image1, filter1, filter1T);

	//x gradient
	SDoublePlane xgrad_row(1,3);
	SDoublePlane xgrad_cols(3,1);

	xgrad_row[0][0]=-1;
	xgrad_row[0][1]=0;
	xgrad_row[0][2]=1;

	xgrad_cols[0][0]=1;
	xgrad_cols[1][0]=2;
	xgrad_cols[2][0]=1;




	SDoublePlane xgrad= convolve_separable(gaussianout, xgrad_row, xgrad_cols);

	return xgrad;


}


//function to calculate y gradient
SDoublePlane CalYgrad(const SDoublePlane &image1)
{

	SDoublePlane filter1 = get_gaussian(3);

	SDoublePlane filter1T = transpose(filter1);

	SDoublePlane gaussianout = convolve_separable(image1, filter1, filter1T);

	//y gradient
	SDoublePlane ygrad_row(1,3);
	SDoublePlane ygrad_cols(3,1);

	ygrad_row[0][0]=1;
	ygrad_row[0][1]=2;
	ygrad_row[0][2]=1;

	ygrad_cols[0][0]=1;
	ygrad_cols[1][0]=0;
	ygrad_cols[2][0]=-1;

	SDoublePlane ygrad= convolve_separable(gaussianout, ygrad_row, ygrad_cols);

	return ygrad;
}
