//
//  map_creation.h
//  critical_points
//
//  Created by Nathan Kocmoud on 3/29/15.
//  Copyright (c) 2015 Allison Badgett. All rights reserved.
//

#ifndef __critical_points__map_creation__
#define __critical_points__map_creation__

#include <stdio.h>
//#include "stdafx.h"
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <assert.h>
#include <algorithm>			// std::max

using std::string;
using std::ifstream;
using std::vector;
using std::cout;
using std::endl;
using std::stof;

class Point{
public:
    float x, y;
    
    Point(float nx, float ny)
    {
        x = nx;
        y = ny;
    }
    void print(void) {
        cout << "\n\t" << x << "\t" << y;
    }
};

class Cell{
public:
    int x, y;
    
    Cell(int nx, int ny)
    {
        x = nx;
        y = ny;
    }
};

// Extracts the points from a file, usually created by the 2D scanner
void analyze_points(void);
vector<Point> get_points(string file_name);
int x_to_grid(float val);
int y_to_grid(float val);
// Creates the vector array of characters for Allison's code
vector< vector<int>> generate_map(string file);
// Prepares the map, given map and threshhold
vector< vector<int>> prepare(vector< vector<int>> map);

Point get_origin();

#endif /* defined(__critical_points__map_creation__) */
