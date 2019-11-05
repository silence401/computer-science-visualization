#include<iostream>
#include<math.h>
#include<GL/glut.h>
using namespace std;
#define EPSILON 0.000001
//the scale of this data field
int dim[]={200,200,200};
void Cross(float *x,float *y,float *z)
{
         float a,b,c;
         a = y[1]*z[2]-z[1]*y[2];
         b = y[2]*z[0]-z[2]*y[0];
         c = y[0]*z[1]-z[0]*y[1];
         x[0] = a;
         x[1] = b;
         x[2] = c;

}
void Normalize(float *x)
{
    float len = sqrt(x[0]*x[0]+x[1]*x[1]+x[2]*x[2]);
    x[0] = x[0]/len;
    x[1] = x[1]/len;
    x[2] = x[2]/len;
}
/*
*/
void MatrixProd(float *c,float *a,float *b)
{
    float x,y,z;
	x=a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
	y=a[3]*b[0]+a[4]*b[1]+a[5]*b[2];
	z=a[6]*b[0]+a[7]*b[1]+a[8]*b[2];
	c[0]=x;
	c[1]=y;
	c[2]=z;
}
/*
create a cube
{x,y,z}:the coordinate of the left-buttom
lehgth: this length of this cube
value:  the data vlaue of this cube(suppose it has the same data value)
*data:  save this data of cube 
*/
void CreateCube(int x,int y,int z,int length,int *rgba,int *data)
{   
    int *data_ptr = data;
    for(int k=z;k<=z+length;k++)
    for(int i=x;i<=x+length;i++)
    for(int j=y;j<=j+length;j++)
    {
      data_ptr[0] = rgba[0];
      data_ptr[1] = rgba[1];
      data_ptr[2] = rgba[2];
      data_ptr[3] = rgba[3];
      data_ptr+=4;
    }

}
/*
create a sphere
{x,y,z}:the coordinate of the left-buttom of the data field 
radius: this radius of this sphere
*rgba:  the data vlaue of this sphere(suppose it has the same data value)
*data:  save this data of sphere
*/
void CreateSphere(int x,int y,int z,int radius,int *rgba,int *data)
{
    int *data_ptr = data;
    for(int k=z;k<dim[2];k++)
    for(int i=x;i<dim[0];i++)
    for(int j=y;j<dim[1];j++)
    {
        if((k-z)*(k-z)+(i-x)*(i-x)+(j-y)*(j-y)<=radius*radius)
        {
           data_ptr[0] = rgba[0];
           data_ptr[1] = rgba[1];
           data_ptr[2] = rgba[2];
           data_ptr[3] = rgba[3];
           data_ptr+=4;
        }
    }
}
/*

*/
void GetRotationMatrix(float *R,float *eye,float *center,float *up)
{
    float XX[3],YY[3],ZZ[3];
    ZZ[0]=eye[0]-center[0];
    ZZ[1]=eye[1]-center[1];
    ZZ[2]=eye[2]-center[2];
    Cross(XX,up,ZZ);
    Cross(YY,ZZ,XX);
    Normalize(XX);
    Normalize(YY);
    Normalize(ZZ);
    R[0]=XX[0];R[1]=YY[0];R[2]=ZZ[0];
    R[3]=XX[1];R[4]=YY[1];R[5]=ZZ[1];
    R[6]=XX[2];R[7]=YY[2];R[8]=ZZ[2];

}
bool CheckinBox(float *point)
{
   if (point[0] < 0||point[0] >= dim[0]||point[1] < 0||point[1] >= dim[1]||point[2] < 0||point[2] >= dim[2]) 
		return false;
	else
		return true;
}
bool Intersection(float *startpos,float *pos,float *dir)
{
     float nearscale = -1000000;
     float scale1,scale2;
     if((dir[0]<=-EPSILON)||(dir[0]>=EPSILON))
     {
         scale1 = (0-pos[0])/dir[0];
         scale2 = (dim[0]-1-pos[0])/dir[0];
         if(scale1 < scale2)
         {
             if(scale1 > nearscale)
                nearscale = scale1;
         }
         else
         {
             if(scale2 > nearscale)
             nearscale = scale2;
         }
     }
     if((dir[1]<=-EPSILON)||(dir[1]>=EPSILON))
     {
         scale1 = (0-pos[1])/dir[1];
         scale2 = (dim[1]-1-pos[1])/dir[1];
         if(scale1 < scale2)
         {
             if(scale1 > nearscale)
                nearscale = scale1;
         }
         else
         {
             if(scale2 > nearscale)
             nearscale = scale2;
         }
     }
     if((dir[2]<=-EPSILON)||(dir[2]>=EPSILON))
     {
         scale1 = (0-pos[2])/dir[2];
         scale2 = (dim[2]-1-pos[2])/dir[2];
         if(scale1 < scale2)
         {
             if(scale1 > nearscale)
                nearscale = scale1;
         }
         else
         {
             if(scale2 > nearscale)
             nearscale = scale2;
         }
     }
    startpos[0] = pos[0] + nearscale * dir[0] ;
	startpos[1] = pos[1] + nearscale * dir[1] ;
	startpos[2] = pos[2] + nearscale * dir[2] ;
    return CheckinBox(startpos);
}
void GetImagePixel(float *image_ptr,int x,int y,int *data,float *R,float *T)
{
    int stepsize = 1;
    float color[4];
    color[0]=color[1]=color[2]=color[3]=0.0;
    float pos[3],dir[3];
    float startpos[3];
    float samplepos[3];
    float samplecolor[4];
    pos[0]=x;pos[1]=y;pos[2]=0;

    dir[0]=-R[2];dir[1]=-R[5];dir[2]=-R[8];

    MatrixProd(pos,R,pos);

    pos[0]+=T[0];
    pos[1]+=T[1];
    pos[2]+=T[2];

    if(Intersection(startpos,pos,dir))
    {
       samplepos[0] = startpos[0];
       samplepos[1] = startpos[1];
       samplepos[2] = startpos[2];
       while(CheckinBox(samplepos)&&color[3]<1)
       {
           TriInterpolation(samplecolor,samplepos,data);

           color[0] += samplecolor[0]*samplecolor[3]*(1-color[3]);
           color[1] += samplecolor[1]*samplecolor[3]*(1-color[3]);
           color[2] += samplecolor[2]*samplecolor[3]*(1-color[3]);
           color[3] += samplecolor[3]*(1-color[3]);
           samplepos[0]+=dir[0]*stepsize;
		   samplepos[1]+=dir[1]*stepsize;
		   samplepos[2]+=dir[2]*stepsize;

       } 
       image_ptr[0]=color[0];
       image_ptr[1]=color[1];
       image_ptr[2]=color[2];
       image_ptr[3]=color[3];
       return;
    }
    image_ptr[0]=image_ptr[1]=image_ptr[2]=image_ptr[3]=1.0;


    
}
int main(int argc,char **argv)
{
   float image[400*400];
   int *data = (int*)malloc(sizeof(dim[0]*dim[1]*dim[2]*4));
   float R[9];
   float T[3]={0,0,450};
   float eye[3] = {0.5,0.5,1};
   float center[3] = {0,0,0};
   float up[3] = {0,1,0};
   GetRotationMatrix(R,eye,center,up);
   float *image_ptr = image;
   for(int i=0;i<400;i++)
   for(int j=0;j<400;j++)
   {
       GetImagePixel(image_ptr,i,j,data,R,T);
       image_ptr+=4;
   }
   

}