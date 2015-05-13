//
//  motion.cpp
//  critical_points
//
//  Created by Allison Badgett on 3/5/15.
//  Copyright (c) 2015 Allison Badgett. All rights reserved.
//

#include "motion.h"
#include "glut_display.h"
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

using namespace std;
extern Building full_layout;

extern vector<vector<int>>zero_loc, door_loc, hall_loc;
extern int width;
extern int height;
extern int window_id;


int x,y;
int index_j=0;

void motion(){
    Room current;
    vector<vector<int>> uav_vec;
    int start_x, start_y, door_x, door_y;
    int current_hall=0, hall_place=0;
    bool at_door=false;
    
// Turn hallways into loops, append whole border vector to itself
    cout << "Desired start point: (7 0)";
    cin >> start_x >> start_y;
    
for (int i =0; i<full_layout.hall_info.size(); i++)
{
    vector<vector<int>> hall_size = full_layout.hall_info[i].borders;
    for (int j=0; j<hall_size.size(); j++)
    {
        full_layout.hall_info[i].borders.push_back(hall_size[j]);
        if((full_layout.hall_info[i].borders[j][0]==start_x)&&(full_layout.hall_info[i].borders[j][1]==start_y))
        {
            current_hall=i; //indicate which hall holds the start point
            hall_place =j;  //gives index of that hall where the start point is
        }
    }
}

    vector<int> temp;   //temporary vector to store coordinates of first point
    temp.push_back(full_layout.hall_info[current_hall].borders[hall_place][0]);
    temp.push_back(full_layout.hall_info[current_hall].borders[hall_place][1]);
    uav_vec.push_back(temp);
    temp.clear();
    
    //check if we are door adjacent
    for(int j=0; j<full_layout.hall_info[current_hall].doors.size();j++)
    {
        if(((x == full_layout.hall_info[current_hall].doors[j][0])&&(y == full_layout.hall_info[current_hall].doors[j][1] + 1))||((x == full_layout.hall_info[current_hall].doors[j][0])&&(y == full_layout.hall_info[current_hall].doors[j][1] - 1))||((y == full_layout.hall_info[current_hall].doors[j][1])&&(x == full_layout.hall_info[current_hall].doors[j][0] + 1))||((y == full_layout.hall_info[current_hall].doors[j][1])&&(x == full_layout.hall_info[current_hall].doors[j][0] - 1)))
        {
            //find current room
            door_x = full_layout.hall_info[current_hall].doors[j][0];
            door_y = full_layout.hall_info[current_hall].doors[j][1];
            for (int i =0; i<full_layout.room_info.size(); i++)
            {
                if ((full_layout.room_info[i].door[0]==door_x) && (full_layout.room_info[i].door[1]==door_y))
                {
                    current = full_layout.room_info[i];
                    break;
                }
            }
            at_door=true;
        }
    }
    
    if(at_door)
    {
        //put door coordinate in uav_vec
        temp.push_back(door_x);
        temp.push_back(door_y);
        uav_vec.push_back(temp);
        temp.clear();
        
        //go into room
        
    }
    else
    {
        //move to next hall border point
        hall_place++;
        temp.push_back(full_layout.hall_info[current_hall].borders[hall_place][0]);
        temp.push_back(full_layout.hall_info[current_hall].borders[hall_place][1]);
        uav_vec.push_back(temp);
        temp.clear();
    }

}

vector<char> motion_commands(vector<vector<int>> m)
{
    vector<char> commands;
    return commands;
}

