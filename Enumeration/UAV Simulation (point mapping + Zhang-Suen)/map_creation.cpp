//
//  map_creation.cpp
//  critical_points
//
//  Created by Nathan Kocmoud on 3/29/15.
//  Copyright (c) 2015 Allison Badgett. All rights reserved.
//

#include "map_creation.h"

#define CELL_SIZE 0.333333			// splits layout into blocks of 4 inches (feet)
#define ERROR_TH 5				// error threshold, block will fill when number of points are above this
#define MM_TO_FOOT 0.00328084	// multiply mm by this to get feet

/*****************GLOBAL VARIABLES********************/

// defines limits of points
float MAX_X = -1000;
float MIN_X = 1000;
float MAX_Y = -1000;
float MIN_Y = 1000;

// grid of building cells
Point origin = Point(0, 0);
//vector<Point> points;

/*******************FUNCTIONS*************************/

void analyze_points(vector<Point> point_vector)
{
    for (int i = 0; i < point_vector.size(); ++i)
    {
        if (point_vector[i].x > MAX_X)
            MAX_X = point_vector[i].x;
        if (point_vector[i].x < MIN_X)
            MIN_X = point_vector[i].x;
        if (point_vector[i].y > MAX_Y)
            MAX_Y = point_vector[i].y;
        if (point_vector[i].y < MIN_Y)
            MIN_Y = point_vector[i].y;
    }
}

// Loads all points from the navigation 2D laser scanner
vector<Point> get_points(string file_name)
{
    ifstream file;
    vector<Point> points;
    
    file.open(file_name);
    char x[100], y[100];		//buffers for temp storage
    
    // Read points from file
    if (file.is_open()) {
        while (file >> x >> y) {
            
            // add point to vector, make sure not end
            if (x[0] != 0 && y[0] != 0){
                
                int x_int = stof(x, NULL);
                int y_int = stof(y, NULL);
                points.push_back(Point(stof(x, NULL)*MM_TO_FOOT, stof(y, NULL)*MM_TO_FOOT));
            }
            
            //cout << "\n" << x << " " << y;
        }
    }
    else
    {
        printf("Could not open file");
    }
    file.close();
    
    // analyze points for max and min
    analyze_points(points);
    
    int x_range = MAX_X - MIN_X;		// building x size (ft)
    int y_range = MAX_Y - MIN_Y;		// building x size (ft)
    int grid_x = x_range / CELL_SIZE;	// number of blocks wide the map will be
    int grid_y = y_range / CELL_SIZE;	// number of blocks wide the map will be
    
    return points;
}

int x_to_grid(float val)
{
    int grid_x = (val - MIN_X) / CELL_SIZE;
    return ++grid_x;    // add 1 for border
}

int y_to_grid(float val)
{
    int grid_y = (val - MIN_Y) / CELL_SIZE;
    return ++grid_y;    // add 1 for border
}

vector< vector<int>> generate_map(string file) {
    vector< vector<int>> map;
    
    // load points from file
    vector<Point> points = get_points(file);
    
    float x_range = MAX_X - MIN_X;
    float y_range = MAX_Y - MIN_Y;
    
    // take max dimension for making the square array
    int map_size = std::max(1 + x_to_grid(MAX_X), 1 + y_to_grid(MAX_Y) + 2);    // add 2 for border of '0's
    
    // initialize the 2D vector map with '1's
    for (int i = 0; i < map_size; ++i) {
        
        vector<int> row;
        for (int j = 0; j < map_size; ++j) {
            row.push_back(1);
        }
        map.push_back(row);
    }
    
    // add border of '0's around
    for (int i = 0; i < map_size; ++i) {
        map[i][0] = 0;
        map[0][i] = 0;
        map[i][map_size-1] = 0;
        map[map_size-1][i] = 0;
    }
    
    // update map with input points
    for (int i = 0; i < points.size(); ++i)
    {
        // get grid coordinates
        int x = x_to_grid(points[i].x);
        int y = y_to_grid(points[i].y);
        
        assert(x >= 0 && x < map.size());
        assert(y >= 0 && y < map.size());
        
        // place '0' if find a point
        //int cell = map[x][y];
        map[x][y] = 0;
    }
    
    origin = Point(x_to_grid(0), y_to_grid(0));
    
    return map;
}

Point get_origin(){
    return origin;
}

vector< vector<int>> prepare(vector< vector<int>> map)
{
    //map does have a border of '0's
    //look for open space '1's in next layer
    
    vector<Cell> frontier;
    vector<Cell> new_frontier;
    vector<Cell> total_frontier;
    
    for (int i = 0; i < map.size(); ++i)
    {
        // for both outer border and layer in, just in case
        for (int j = 0; j < 1; ++j)
        {
            if (map[j][i] == 1)                 //left
            {
                frontier.push_back(Cell(j, i));
                total_frontier.push_back(Cell(j, i));
                map[j][i] = 2;
            }
            if (map[i][j] == 1)                 //top
            {
                frontier.push_back(Cell(i, 0));
                total_frontier.push_back(Cell(i, 0));
                map[i][j] = 2;
            }
            if (map[i][map.size()-j-1] == 1)    //right
            {
                frontier.push_back(Cell(i, map.size()-j-1));
                total_frontier.push_back(Cell(i, map.size()-j-1));
                map[i][map.size()-j-1] = 2;
            }
            if (map[map.size()-j-1][i] == 1)    //bottom
            {
                frontier.push_back(Cell(map.size()-j-1, i));
                total_frontier.push_back(Cell(map.size()-j-1, i));
                map[map.size()-j-1][i] = 2;
            }
        }
    }
    
//    while (frontier.size() != 0)
//    {
//        //populate new frontier
//        for (int i = 0; i < frontier.size(); ++i)
//        {
//            int x = frontier[i].x;
//            int y = frontier[i].y;
//            
//            if (x-1 > 0)
//            {
//                if (map[x-1][y] == 1)    //left of cell
//                {
//                    new_frontier.push_back(Cell(x-1,y));
//                    total_frontier.push_back(Cell(x-1,y));
//                }
//            }
//            if (y-1 > 0)
//            {
//                if (map[x][y-1] == 1)    //above cell
//                {
//                    new_frontier.push_back(Cell(x,y-1));
//                    total_frontier.push_back(Cell(x,y-1));
//                }
//            }
//            if (x+1 < map.size())
//            {
//                if (map[x+1][y] == 1)    //right of cell
//                {
//                    new_frontier.push_back(Cell(x+1,y));
//                    total_frontier.push_back(Cell(x+1,y));
//                }
//            }
//            if (y+1 > 0)
//            {
//                if (map[x][y+1] == 1)    //above cell
//                {
//                    new_frontier.push_back(Cell(x,y+1));
//                    total_frontier.push_back(Cell(x,y+1));
//                }
//            }
//        }
//        
//        //clear old frontier
//        frontier.clear();
//        
//        //move new frontier to old frontier
//        for (int i = 0; i < new_frontier.size(); ++i)
//        {
//            frontier.push_back(Cell(new_frontier[i].x, new_frontier[i].y));
//        }
//        
//        //clear new frontier
//        new_frontier.clear();
//    
//    }
    
    return map;
}


