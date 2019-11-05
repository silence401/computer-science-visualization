#include <GL/glut.h>
#include<iostream>
#include<vector>
#include<math.h>
#include<fstream>
#include<ostream>
using namespace std;
struct voxel{
    float x,y,z;
};
vector<voxel> sphere;
float step = 0.04;
//create the data of sphere by given radius and the center of the sphere  and save it to .txt
void CreateSphere(float radius,vector<float>center)
{
    ofstream ofile("sphere.txt");
    float x = center[0];
    float y = center[1];
    float z = center[2];
    for(float xx = x-radius; xx<=x+radius; xx+=step)
    for(float yy = y-radius; yy<=y+radius; yy+=step)
    {
       
       if(xx*xx+yy*yy>radius*radius)
       continue;
       float  zz = sqrt(radius*radius-xx*xx-yy*yy);
       voxel tmp ;
       tmp.x = xx;
       tmp.y = yy;
       tmp.z = zz;
       sphere.push_back(tmp);
       ofile<<xx<<' '<<yy<<' '<<zz<<endl;
       //cout<<xx<<' '<<yy<<' '<<zz<<endl;

    }
    ofile.close();
    return ;
}
void reshape(GLsizei w,GLsizei h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(10,1,0,6);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,w,h);

}
void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
   // glOrtho(-5, 5, -5, 5, 5, 15);
    gluPerspective(10,1,0,10);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(2, 2, 20, 0, 0, 0, 0, 1, 0);

    return;
}
//void reshape()
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POINTS);
  //  glMatrixMode(GL_PROJECTION);
  //  glLoadIdentity();
   // glOrtho(-5, 5, -5, 5, 5, 15);
//    gluPerspective(20,1,0,3);
 //   glMatrixMode(GL_MODELVIEW);
 //   gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);
    for(int i=0;i<sphere.size();i++)
    {
        glVertex3f(sphere[i].x,sphere[i].y,sphere[i].z);
    }
    glEnd();
    //glutWireTeapot(3);
    glFlush();

    return;
}

int main(int argc, char *argv[])
{
    vector<float> center;
    center.push_back(0.0);
    center.push_back(0.0);
    center.push_back(0.0);
    CreateSphere(1,center);
    glutInit(&argc, argv);
  //./  init();
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(500, 500);
    glutCreateWindow("OpenGL 3D View");
    init(); 
 //   glutReshapeFunc(reshape);
    glutDisplayFunc(display);
 // glutKeyboardFunc(GLUT_UP,Up);
    glutMainLoop();
    return 0;
}
