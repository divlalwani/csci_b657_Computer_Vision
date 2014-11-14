using namespace std;

//calculate haiimg distance between input image and template 
//Both image and template must be binary images (with pixels corresponding to either 0 or 1)
SDoublePlane hamming(const SDoublePlane &input, const SDoublePlane &temp)
{


  int rows=input.rows();
  int cols=input.cols();

  int trows=temp.rows();
  int tcols=temp.cols();
  SDoublePlane scores(rows, cols);

	for(int i=0; i<rows-trows; i++)
	{
		for(int j=0; j<cols-tcols; j++)
		{

			double sum=0;
			for(int n=0; n<trows; n++)
			{
				for(int m=0; m<tcols; m++)
				{
					sum += (input[i+n][j+m]*temp[n][m]) + ((1-input[i+n][j+m])*(1-temp[n][m]));
				}
			}
			scores[i][j]=sum;
		}
	}
	return scores;
}

