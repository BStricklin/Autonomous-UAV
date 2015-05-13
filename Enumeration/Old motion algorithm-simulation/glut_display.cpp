//
//  glut_display.cpp
//  critical_points
//
//  Created by Allison Badgett on 2/12/15.
//  Copyright (c) 2015 Allison Badgett. All rights reserved.
//

#include "glut_display.h"
#include "motion.h"
#include <unistd.h>
#include "building.h"
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

#define BUILDING_SIZE 100

using namespace std;
extern Building full_layout;

extern vector<vector<int>>zero_loc, door_loc, hall_loc, pathway;
extern int width;
extern int height;
extern int window_id;
extern int m_index;
bool display_wait;

void display(void)  //displays a square on the screen at the location of each point within the specified value range
{
    
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    width = glutGet(GLUT_WINDOW_WIDTH);
    height = glutGet(GLUT_WINDOW_HEIGHT);
    int square_w = width/BUILDING_SIZE;
    int square_h = height/BUILDING_SIZE;
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
    
    width = glutGet(GLUT_WINDOW_WIDTH);
    height = glutGet(GLUT_WINDOW_HEIGHT);
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    
    glBegin(GL_QUADS);
   /*
    glColor3f(0.1, 0.0, 0.9);
    {
        glVertex2f(square_w*(pathway[m_index][0]-.5),square_h*(pathway[m_index][1]-.5));
        glVertex2f(square_w*(pathway[m_index][0]-.5),square_h*(pathway[m_index][1]+.5));
        
        glVertex2f(square_w*(pathway[m_index][0]+.5),square_h*(pathway[m_index][1]+.5));
        glVertex2f(square_w*(pathway[m_index][0]+.5),square_h*(pathway[m_index][1]-.5));
    }
    */
    glColor3f(0.8, 0.5, 0.8);
    for(int i=0; i<full_layout.room_info.size(); i++)
        
    {
            glVertex2f((square_w*(full_layout.room_info[i].centroid[0]-.5)),square_h*(full_layout.room_info[i].centroid[1]-.5));
            glVertex2f((square_w*(full_layout.room_info[i].centroid[0]-.5)),square_h*(full_layout.room_info[i].centroid[1]+.5));
            
            glVertex2f(square_w*((full_layout.room_info[i].centroid[0]+.5)),square_h*(full_layout.room_info[i].centroid[1]+.5));
            glVertex2f(square_w*(full_layout.room_info[i].centroid[0]+.5),square_h*(full_layout.room_info[i].centroid[1]-.5));
    }
    
    glColor3f(0.8, 0.5, 0.8);
    for(int i=0; i<full_layout.hall_info.size(); i++)
    {
        for ( int j=0; j<full_layout.hall_info[i].borders.size();j++){
            glVertex2f(square_w*(full_layout.hall_info[i].borders[j][0]-.5),square_h*(full_layout.hall_info[i].borders[j][1]-.5));
            glVertex2f(square_w*(full_layout.hall_info[i].borders[j][0]-.5),square_h*(full_layout.hall_info[i].borders[j][1]+.5));
            
            glVertex2f(square_w*(full_layout.hall_info[i].borders[j][0]+.5),square_h*(full_layout.hall_info[i].borders[j][1]+.5));
            glVertex2f(square_w*(full_layout.hall_info[i].borders[j][0]+.5),square_h*(full_layout.hall_info[i].borders[j][1]-.5));
        }
    }
    
    glColor3f(0.5, 0.5, 0.5);
    for(int i=0; i<full_layout.room_info.size(); i++)
        
    {
        for (int j =0; j<full_layout.room_info[i].room_border.size(); j++)
        {
            glVertex2f((square_w*(full_layout.room_info[i].room_border[j][0]-.5)),square_h*(full_layout.room_info[i].room_border[j][1]-.5));
            glVertex2f((square_w*(full_layout.room_info[i].room_border[j][0]-.5)),square_h*(full_layout.room_info[i].room_border[j][1]+.5));
    
            glVertex2f(square_w*((full_layout.room_info[i].room_border[j][0]+.5)),square_h*(full_layout.room_info[i].room_border[j][1]+.5));
            glVertex2f(square_w*(full_layout.room_info[i].room_border[j][0]+.5),square_h*(full_layout.room_info[i].room_border[j][1]-.5));
        }
    }
    
    glColor3f(0.0, 1.0, 1.0);
    for(int i=0; i<full_layout.door_loc.size(); i++)
        
    {
        
        glVertex2f(square_w*(full_layout.door_loc[i][0]-.5),square_h*(full_layout.door_loc[i][1]-.5));
        glVertex2f(square_w*(full_layout.door_loc[i][0]-.5),square_h*(full_layout.door_loc[i][1]+.5));
        
        glVertex2f(square_w*(full_layout.door_loc[i][0]+.5),square_h*(full_layout.door_loc[i][1]+.5));
        glVertex2f(square_w*(full_layout.door_loc[i][0]+.5),square_h*(full_layout.door_loc[i][1]-.5));
        
    }
    
    
    glColor3f(1.0, 1.0, 0.0);
    for(int i=0; i<zero_loc.size(); i++)
        
    {
        
        glVertex2f(square_w*(zero_loc[i][0]-.5),square_h*(zero_loc[i][1]-.5));
        glVertex2f(square_w*(zero_loc[i][0]-.5),square_h*(zero_loc[i][1]+.5));
        
        glVertex2f(square_w*(zero_loc[i][0]+.5),square_h*(zero_loc[i][1]+.5));
        glVertex2f(square_w*(zero_loc[i][0]+.5),square_h*(zero_loc[i][1]-.5));
        
    }
    

    glEnd();

    glutSwapBuffers();
    usleep(350000);
    if(m_index<(pathway.size()-1))
    {
        m_index++;
    }
    glutPostRedisplay();
}
void create_window(void)    //creates window
{
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
   
    glutInitWindowPosition(10, 10);
    glutInitWindowSize(1000, 1000);

    window_id = glutCreateWindow("Picture");
    display_wait=true;
    //motion();
    glutDisplayFunc(display);
    
}