#include<stdio.h>
#include<stdlib.h>
#include<complex.h>
#include<math.h>
#include<conio.h>

typedef struct image
{
	unsigned char *headr,**dat;
	int w;
	int h;
	int d;
}IMAGE;

IMAGE *image_read(char *filename);
void image_write(char *filename,IMAGE *image_name);
IMAGE *myfunction1(IMAGE *image_name);
IMAGE *myfunction2(IMAGE *image_name);

int main()
{
    IMAGE *image1 = image_read("pc1.bmp");
    IMAGE *image2 = myfunction1(image1);
    image_write("Otsu.bmp",image2);
    system("Otsu.bmp");
    IMAGE *image3 = myfunction2(image1);
    image_write("Adaptive.bmp",image3);
    system("Adaptive.bmp");
    return 0;
}

IMAGE *image_read(char *filename)
{
	int width,height,depth,i,j,k;
	unsigned char *header,dummy[4];
	unsigned char **data;
	FILE *ip;
	IMAGE *ptr,im;
	ptr=&im;
	ip=fopen(filename,"rb");
	header=(unsigned char *) malloc(1078);

	fseek(ip,0L,SEEK_SET);
	fread(header,1,1078,ip);
	for(k=0;k<4;k++)
	dummy[k]=header[k+18];
	int *wp,*hp,*dp;
	wp=(int*)dummy;
	width=*wp;
	
	for(k=0;k<4;k++)
    	dummy[k]=header[k+22];
	
	hp=(int*)dummy;
	height=*hp;
	
	for(k=0;k<2;k++)
    	dummy[k]=header[k+28];
    dp=(int*)dummy;
    depth=*dp;
	
	data = malloc(width * sizeof(unsigned char *));
	for(i = 0; i < width; i++)
    	data[i] = malloc(height * sizeof(unsigned char));
	
	fseek(ip,1078L,SEEK_SET);
	for(i=0;i<height;i++)
	{
     for(j=0;j<width;j++)
	 {
       fscanf(ip,"%c",&data[i][j]);
     }
    }		

	ptr=(IMAGE *)malloc(sizeof(IMAGE));
	ptr->h=height;
	ptr->w=width;
	ptr->d=depth;
	ptr->headr=header;
	ptr->dat=data;
	return ptr;
}

void image_write(char *filename,IMAGE *image_name)
{
	FILE *op;
	int i,j;
	op=fopen(filename,"wb");
	fseek(op,0L,SEEK_SET);	
	fwrite(image_name->headr,1,1078,op);	
	fseek(op,1078L,SEEK_SET);

    for(i=0;i<image_name->h;i++)
	  for(j=0;j<image_name->w;j++)
         fprintf(op,"%c",image_name->dat[i][j]);
	fclose(op);
}
// Function for Otsu's Thresholding
IMAGE *myfunction1(IMAGE *image_name)
{
    int i,j,k,l;
//  Variable Declaration
    IMAGE *ptr;
	unsigned char **data; 
//  Memory Allocation
    data = malloc((image_name->w) * sizeof(unsigned char *));
	for(i = 0; i < image_name->w; i++)
      data[i] = malloc((image_name->h) * sizeof(unsigned char));
    for(i=0;i<image_name->h;i++)
    for(j=0;j<image_name->w;j++)
         data[i][j] = image_name->dat[i][j];
         
	for(i=0;i<(image_name->w);i++)
	{
      for(j=0;j<(image_name->h);j++)
      {
         if(data[i][j]>142)
            data[i][j] = 255;
         else
            data[i][j] = 0;
      }
    }      

    ptr=(IMAGE *)malloc(sizeof(IMAGE));
	ptr->h=image_name->h;
	ptr->w=image_name->w;
	ptr->d=image_name->d;
	ptr->headr=image_name->headr;
	ptr->dat=data;
	return ptr;
}

// Function for Adaptive Thresholding
IMAGE *myfunction2(IMAGE *image_name)
{
    int i,j,k,l;
//  Variable Declaration
    float tmp;
    IMAGE *ptr;
	unsigned char **data,**z; 

//  Memory Allocation
    data = malloc((image_name->w) * sizeof(unsigned char *));
	for(i = 0; i < image_name->w; i++)
      data[i] = malloc((image_name->h) * sizeof(unsigned char));
    
    z = malloc((image_name->w) * sizeof(unsigned char *));
	for(i = 0; i < image_name->w; i++)
      z[i] = malloc((image_name->h) * sizeof(unsigned char));

    for(i=0;i<image_name->h;i++)
    {
       for(j=0;j<image_name->w;j++)
       {
          data[i][j] = image_name->dat[i][j];
       }
    }
            
	for(i=3;i<(image_name->w-3);i++)
	{
      for(j=3;j<(image_name->h-3);j++)
      {
         tmp = 0;
         for(k=0;k<7;k++)
         {
            for(l=0;l<7;l++)
            {
               tmp += data[i-3+k][j-3+l];
            }
         }
         if(data[i][j]>tmp/49-3)
           z[i][j] = 255;
         else
           z[i][j] = 0;
      }
    }
    
    ptr=(IMAGE *)malloc(sizeof(IMAGE));
	ptr->h=image_name->h;
	ptr->w=image_name->w;
	ptr->d=image_name->d;
	ptr->headr=image_name->headr;
	ptr->dat=z;
	return ptr;
}

