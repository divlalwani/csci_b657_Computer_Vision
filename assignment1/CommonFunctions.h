using namespace std;

//Create a binary image
SDoublePlane blackandwhite(const SDoublePlane &input)
{
		SDoublePlane output(input.rows(), input.cols());
		int rows = input.rows();
		  int cols = input.cols();
		for(int j=0; j <cols; j++)            
		{
			for(int i=0; i<rows; i++) 
			{
				if (input[i][j]>=0 && input[i][j]<70) //thresholding
				output[i][j]=0;

				else
				output[i][j]=1;
			}

		}
		return output;
}


//find max distance from the scores generated in step 4 and 5
double findmax(SDoublePlane hamdist)
{
		double max = 0;
		int rows = hamdist.rows();
		int cols = hamdist.cols();
		for(int i=0;i < rows; i++)
		{
			for(int j=0; j < cols ; j++)
			{
				if(hamdist[i][j] > max)
				max = hamdist[i][j];
			}
		}
		return max;

}

