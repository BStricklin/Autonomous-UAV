//
//  main.cpp
//  Based on work of Zhang Suen
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
#include "map_creation.h"
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
    
    // Load map from 2D scanner, from a sample map of points
    mod_img = generate_map("map1");
    orig_img = generate_map("map1");
    comp_img = generate_map("map1");
    Point start = get_origin();
    
    
    // This portion loads from a sample map text file
    /*
    ifstream iss;
    iss.open("map2.txt");
    
    string line;
    int unit;
    bool first = true;
    
    //initialize orig_img & mod_img to contents of original.txt
    while(getline(iss, line))
    {
        vector<int> row1;
        vector<int> row2;
        row1.push_back(0);          //add left border
        row2.push_back(0);
        stringstream ss;
        ss << line;
        while (ss >> unit)
        {
            row1.push_back(0);      //populate top border
            row2.push_back(unit);
        }
        row1.push_back(0);          //add right border
        row2.push_back(0);
        
        //add top border of 0s if the first row
        if (first == true)
        {
            first = false;
            
            orig_img.push_back(row1);
            mod_img.push_back(row1);
            comp_img.push_back(row1);
        }
        orig_img.push_back(row2);
        mod_img.push_back(row2);
        comp_img.push_back(row2);
    }
    
    //add bottom border of 0s
    vector<int> row;
    for (int i = 0; i < orig_img[0].size(); ++i)
    {
        row.push_back(0);
    }
    orig_img.push_back(row);
    mod_img.push_back(row);
    comp_img.push_back(row);
    Point start = Point(0,0);
    */
    
    // prepare the image
    orig_img = prepare(orig_img);
    
    // set the start location
    //orig_img[start.x][start.y] = 2;
    
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
