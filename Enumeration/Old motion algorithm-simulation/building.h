//
//  building.h
//  critical_points
//
//  Created by Allison Badgett on 2/26/15.
//  Copyright (c) 2015 Allison Badgett. All rights reserved.
//

#include <vector>
#include <iostream>
using namespace std;

//extern int building_size;
extern vector<vector<int>> building;
class Room{
public:
    vector<int> door;
    vector<int> centroid;
    vector<vector<int>> door_to_centroid;
    vector<vector<int>> room_border;
    vector<vector<int>> hall_border;
    //add pointer to hallway object
};


class Hallway{
public:
    vector<vector<int>> borders;   //hall_border
    vector<vector<int>> doors;     //vector containing coordinates of doors on this hallway
    vector<Room> off_rooms;        //vector containing rooms on this hallway
    
};

class Building {
public:
    vector<Room> room_info;
    vector<Hallway> hall_info;
    vector<vector<int>> door_loc;
    vector<vector<int>> travel_path();      //function that outputs the path the UAV should follow
    vector<string> motion_commands();  //function that outputs the motion commands for the UAV to follow path
};

Room find_centroid(Room);

vector<Hallway> compare_halls(vector<Hallway>, Hallway);    //compares current hall list with potential addition
vector<vector<int>> make_list(Hallway, int);                     //returns the new hallway list for building
vector<vector<int>> d_to_c(Room);
vector<vector<int>> travel_path(vector<Hallway>, int, int);
