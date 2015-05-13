//
//  glut_display.cpp
//  Zhang_Suen
//
//  Created by Allison Badgett on 4/9/15.
//  Copyright (c) 2015 Allison Badgett. All rights reserved.
//

#include "glut_display.h"
#include <iostream>
#include <vector>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#ifdef WIN32
#include <glut.h>
#else
#include <GL/glut.h>
#endif
#endif

using namespace std;

int width;
int height;
int window_id;

extern vector<vector<int>> orig_img, mod_img;

void display(void)  //displays a square on the screen at the location of each point within the specified value range
{
    int image_size = orig_img.size();
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    width = glutGet(GLUT_WINDOW_WIDTH);
    height = glutGet(GLUT_WINDOW_HEIGHT);
    int square_w = (height-1)/image_size;
    int square_h = (height-1)/image_size;
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    
    glBegin(GL_QUADS);
    glColor3f(0.47, 0.20, 0.33);
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_QUADS);
    glColor3f(0.2, 0.0, 1.0);
    for(int i=0; i<mod_img.size(); i++)
    {
        for(int j=0; j<mod_img[i].size();j++)
        {
            glVertex2f(mod_img[i][j]*square_w*(j+1-.5),height - mod_img[i][j]*square_h*(i+1-.5));
            glVertex2f(mod_img[i][j]*square_w*(j+1-.5),height - mod_img[i][j]*square_h*(i+1+.5));
        
            glVertex2f(mod_img[i][j]*square_w*(j+1+.5),height - mod_img[i][j]*square_h*(i+1+.5));
            glVertex2f(mod_img[i][j]*square_w*(j+1+.5),height - mod_img[i][j]*square_h*(i+1-.5));
        }
    }
    
    glColor3f(1.0, 1.0, 0.0);
    for(int i=0; i<orig_img.size(); i++)
    {
        for(int j=0; j<orig_img[i].size();j++)
        {
            glVertex2f(orig_img[i][j]*square_w*(j+1-.5),height - orig_img[i][j]*square_h*(i+1-.5));
            glVertex2f(orig_img[i][j]*square_w*(j+1-.5),height - orig_img[i][j]*square_h*(i+1+.5));
            
            glVertex2f(orig_img[i][j]*square_w*(j+1+.5),height - orig_img[i][j]*square_h*(i+1+.5));
            glVertex2f(orig_img[i][j]*square_w*(j+1+.5),height - orig_img[i][j]*square_h*(i+1-.5));
        }
    }
    glEnd();
    
    glutSwapBuffers();

}
void create_window(void)    //creates window
{
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    glutInitWindowPosition(10, 10);
    glutInitWindowSize(1000, 1000);
    
    window_id = glutCreateWindow("Picture");
    glutDisplayFunc(display);
    
}