using namespace std;

//sobel operator 
SDoublePlane sobel1(const SDoublePlane &input1,const SDoublePlane &input2 )
{

  int rows1=input1.rows();
  int cols1=input1.cols();

  int rows2=input2.rows();
  int cols2=input2.cols();
  //int count = 0;

  SDoublePlane output(rows1, cols1);
  SDoublePlane temp(rows1, cols1);
  SDoublePlane sobel(rows1, cols1);
  SDoublePlane D(rows1, cols1);


	for(int i=0; i<rows1; i++)
	{
		for(int j=0; j<cols1; j++)
		{
			input1[i][j] = input1[i][j] * input1[i][j];
			input2[i][j] = input2[i][j] * input2[i][j];
		}
	}

	for(int i=0; i<rows1; i++)
	{
		for(int j=0; j<cols1; j++)
		{
			sobel[i][j] = sqrt(input1[i][j] + input2[i][j]);
			 if(sobel[i][j] >= 0 && sobel[i][j] < 127)
				sobel[i][j] = 0;
			else
				sobel[i][j] = 255; 
			//cout << sobel[i][j];
		}
	}

	return sobel;
}

//generates scoring function using formula given in step 5
SDoublePlane Distance(const SDoublePlane &input,const SDoublePlane &T)
{

  int rows1=input.rows();
  int cols1=input.cols();

  int rows2=T.rows();
  int cols2=T.cols();



  SDoublePlane output(rows1, cols1);
  SDoublePlane temp(rows1, cols1);
  
  SDoublePlane D(rows1, cols1);

	double currcolmina[rows1];


	int M= rows1;
	int N= cols1;


	for(int i=0;i<rows1;i++)
	{
		for(int j=0;j<cols1;j++)
		{
			double currrowmin = 1;
			for(int a=0; a <M; a++)
			{
				double currcolmin = 1;
				for(int b=0; b<N; b++)
				{
					if(input[a][b] == 0)
						temp[a][b] = sqrt(((i-a) * (i-a)) + ((j-b) * (j-b))); 
					else
						temp[a][b] = 1;

					if(temp[a][b] < currcolmin)
						currcolmin = temp[a][b];
				}
				currcolmina[a] = currcolmin;
				if(currcolmina[a] < currrowmin)
					currrowmin = currcolmina[a];

			}
			D[i][j] = currrowmin;
		}
	}

	for(int i=0;i<rows1 - rows2;i++)
	{
		for(int j=0;j<cols1 - cols2;j++)
		{
			for(int k=0; k < rows2; k++)
			{
				for(int l=0; l < cols2; l++)
				{
					output[i][j] += T[k][l] * D[i+k][j+l];
				}
			}
		}
	}

  return output;

}

