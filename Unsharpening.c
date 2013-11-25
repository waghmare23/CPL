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
IMAGE *myfunction(IMAGE *image_name);

int main()
{
    IMAGE *image1 = image_read("pc2.bmp");
    IMAGE *image2 = myfunction(image1);
    image_write("UnsharpenImage.bmp",image2);
    system("UnsharpenImage.bmp");
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
// Function for Image Unsharping
IMAGE *myfunction(IMAGE *image_name)
{
    int i,j,k,l;
//  Variable Declaration
    IMAGE *ptr;
	unsigned char **data; 
	float tmp;
	float h[3][3] = {1.0/9.0, 1.0/9.0, 1.0/9.0,
                     1.0/9.0, 1.0/9.0, 1.0/9.0,
                     1.0/9.0, 1.0/9.0, 1.0/9.0};
//  Memory Allocation
    data = malloc((image_name->w) * sizeof(unsigned char *));
	for(i = 0; i < image_name->w; i++)
      data[i] = malloc((image_name->h) * sizeof(unsigned char));
    for(i=0;i<image_name->h;i++)
    for(j=0;j<image_name->w;j++)
         data[i][j] = image_name->dat[i][j];
         
	for(i=1;i<(image_name->w-1);i++)
	{
      for(j=1;j<(image_name->h-1);j++)
      {
         tmp=0;            
         for(k=0;k<3;k++)
         {
          for(l=0;l<3;l++)  
          {
            tmp += (float) h[k][l]*image_name->dat[i+k-1][j+l-1]/255.0;
          }
         }
         if(tmp>1)
         data[i][j]=255;
         else if(tmp<0)
         data[i][j]=0;
         else
         data[i][j]=tmp*255;     
      }
    }      

    ptr=(IMAGE *)malloc(sizeof(IMAGE));
	ptr->h=image_name->h;
	ptr->w=image_name->w;
	ptr->d=image_name->d;
	ptr->headr=image_name->headr;
	ptr->dat=data;
    image_write("SmoothImage.bmp",ptr);
    system("SmoothImage.bmp");
    for(i=1;i<(image_name->w-1);i++)
	{
      for(j=1;j<(image_name->h-1);j++)
      {
         tmp = (image_name->dat[i][j] - data[i][j]);   
         if(tmp>255)
         data[i][j]=255;
         else
         data[i][j]=tmp;  
      }
    }      
   	ptr->dat=data;
    image_write("DifferenceImage.bmp",ptr);
    system("DifferenceImage.bmp");
    for(i=1;i<(image_name->w-1);i++)
	{
      for(j=1;j<(image_name->h-1);j++)
      {
         tmp = (float) image_name->dat[i][j] + 0.09* data[i][j];  
         if(tmp>255)
         data[i][j]=255;
         else
         data[i][j]=tmp; 
      }
    }      
   	ptr->dat=data;
	return ptr;
}
