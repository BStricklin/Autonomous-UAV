//
//  main.cpp
//  Zhang_Suen
//
//  Created by Allison Badgett on 4/8/15.
//  Copyright (c) 2015 Allison Badgett. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "glut_display.h"
#include "image_manip.h"
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
vector<vector<int>> mod_img;
vector<vector<int>> orig_img;
vector<vector<int>> comp_img;


int main(int argc, char * argv[]) {
    
    glutInit(&argc, argv);
    
    ifstream iss;
    iss.open("original.txt");
    
    string line;
    int unit;
    
    //initialize orig_img & mod_img to contents of original.txt
    while(getline(iss, line))
    {
        vector<int> row;
        vector<char> row2;
        stringstream ss;
        ss << line;
        while (ss >> unit)
        {
            row.push_back(unit);
            row2.push_back(unit);
        }
        orig_img.push_back(row);
        mod_img.push_back(row);
        comp_img.push_back(row);
    }
    
    do {

        comp_img = mod_img;
        mod_img = sub_one(mod_img);
    

        for (int i = 0; i < mod_img.size(); i++)
        {
            for (int j =0; j < mod_img[i].size(); j++)
            {
                if(mod_img[i][j]=='*') mod_img[i][j]=0;
            }
        }

        mod_img = sub_two(mod_img);
    

        for (int i = 0; i < mod_img.size(); i++)
        {
            for (int j =0; j < mod_img[i].size(); j++)
            {
                if(mod_img[i][j]=='*') mod_img[i][j]=0;
            }
        }

    } while (comp_img!=mod_img);
    
    create_window();
    
    glutMainLoop();
}
