

using namespace std;

//convolve an image using separable filter 
SDoublePlane convolve_separable(const SDoublePlane &input)
{
  SDoublePlane output(input.rows(), input.cols());
  SDoublePlane output_horz(input.rows(), input.cols());
  SDoublePlane output_vert(input.rows(), input.cols());
  int rows = input.rows();
  int cols = input.cols();

//SDoublePlane row_filter(3,0);
//SDoublePlane col_filter(3,0);
float row_filter[3];
row_filter[0]=1/3.0;
row_filter[1]=1/3.0;
row_filter[2]=1/3.0;

float col_filter[3];
col_filter[0]=1/3.0;
col_filter[1]=1/3.0;
col_filter[2]=1/3.0;


//Applying horizontal seperable kernel
for(int i=0; i<rows; i++)
{   
    for(int j=0; j<(cols-3); j++)
    {
        float sum=0;
        for(int n=0; n<3; n++)
        {       //cout<<i<<" "<<j<<" "<<n<<"";
            sum +=input[i][j+n]*row_filter[n];
                  
        }
        output_horz[i][j+1]=sum;
//cout<<sum<<endl;

}
}

////Applying vertical seperable kernel
for(int j=0; j<cols; j++)
{   
    for(int i=0; i<(rows-3); i++)
    {
        float sum=0;
        for(int n=0; n<3; n++)
        {      
            sum +=output_horz[i+n][j]*col_filter[n];
                  
        }
        output_vert[i+1][j]=sum;


}
}



return output_vert;

}

  


// Convolve an image with a 2-D convolution kernel



SDoublePlane convolve_general(const SDoublePlane &input, const SDoublePlane &filter)
{
 SDoublePlane output(input.rows(), input.cols());
  SDoublePlane outputq(input.rows(), input.cols());
  int rows = input.rows();
  int cols = input.cols();

  int krows = filter.rows();
  int kcols = filter.cols();
  
 

for(int i=0; i < (rows-krows); i++){
for(int j=0; j < (cols-kcols); j++)     //h        
{
    
        int sum=0;
        for(int n=0; n <krows;n++) //f
        {
            for(int m=0; m <kcols; m++) //f
            {
            int t=filter[n][m]*input[i+n][j+m];
            sum=sum+t;
            }
        }
        
     
	output[i+(krows/2)][j+(kcols/2)]=sum;
}
}


  return output;
}
