//
//  neighbor_tracing.cpp
//  critical_points
//
//  Created by Allison Badgett on 2/19/15.
//  Copyright (c) 2015 Allison Badgett. All rights reserved.
//

#include "neighbor_tracing.h"
#include "building.h"

extern vector<vector<int>> building;
extern Building full_layout;

vector<vector<int>> border_vec;
vector<vector<int>> r_border_vec;
vector<vector<int>> motion_vec{ {1,0,0,-1,-1,0,0,1,1,0,0,-1,-1,0,0,1}, {0,1,1,0,0,-1,-1,0,0,1,1,0,0,-1,-1,0} };

vector<int> current_pixel, start_pixel, moving_pixel; //pixel[0] = x; pixel[1] = y
int position_id; //will be the index of last border_vec element added
int zero_index;

vector<vector<int>> neighbor_trace(int i){
    int door_x = full_layout.door_loc[i][0];
    int door_y = full_layout.door_loc[i][1];

if (building[building.size()-door_y][door_x]==0)
{
    //check up
    current_pixel.clear();
    current_pixel.push_back(door_x);  //move up one pixel and set to current pixel
    current_pixel.push_back(door_y+1);
    
    start_pixel.clear();
    start_pixel.push_back(door_x); //set start pixel
    start_pixel.push_back(door_y);
    
    border_vec.clear();
    border_vec.push_back(current_pixel); //add to border vector
    current_pixel[0]=door_x;
    current_pixel[1]=door_y;
    position_id=13;
    next_border(i);
    r_border_vec = border_vec;
    
    //check down
    current_pixel[0]=door_x;  //move left one pixel and set to current pixel
    current_pixel[1]=door_y-1;
    
    start_pixel[0]=door_x; //set start pixel
    start_pixel[1]=door_y-1;
    
    border_vec.clear(); //clear border vec
    border_vec.push_back(current_pixel); //add to border vector
    current_pixel[0]=door_x;
    current_pixel[1]=door_y;
    position_id=14;
    next_border(i);
    
    }
    else
    {
        //check right
        current_pixel.clear();
        current_pixel.push_back(door_x+1);  //move right one pixel and set to current pixel
        current_pixel.push_back(door_y);
        
        start_pixel.clear();
        start_pixel.push_back(door_x+1); //set start pixel
        start_pixel.push_back(door_y);
        
        border_vec.clear();
        border_vec.push_back(current_pixel); //add to border vector
        current_pixel[0]=door_x;
        current_pixel[1]=door_y;
        position_id=11;
        next_border(i);
        r_border_vec = border_vec;
        
        //check left
        current_pixel[0]=door_x-1;  //move left one pixel and set to current pixel
        current_pixel[1]=door_y;
        
        start_pixel[0]=door_x-1; //set start pixel
        start_pixel[1]=door_y;
        
        border_vec.clear(); //clear border vec
        border_vec.push_back(current_pixel); //add to border vector
        current_pixel[0]=door_x;
        current_pixel[1]=door_y;
        position_id=12;
        next_border(i);
        
    }

return border_vec;
}

void next_border(int i){
    
    //move down when starting
    if(position_id==11) current_pixel[1]-=1;
    if(position_id==12) current_pixel[1]+=1;
    if(position_id==13) current_pixel[0]+=1;
    if(position_id==14) current_pixel[0]-=1;
    
    while( check_door(i) )
    {
    switch (position_id){

        case(0):
            position_id=6;
            while( check_edges() )
            {
                current_pixel[0] = current_pixel[0]+motion_vec[0][position_id];
                current_pixel[1] = current_pixel[1]+motion_vec[1][position_id];
                position_id++;
            }
            break;

        case(1):
            position_id=0;
            while( check_edges() )
            {
                current_pixel[0] = current_pixel[0]+motion_vec[0][position_id];
                current_pixel[1] = current_pixel[1]+motion_vec[1][position_id];
                position_id++;
            }
            break;

        case(2):
            position_id=0;
            while( check_edges() )
            {
                current_pixel[0] = current_pixel[0]+motion_vec[0][position_id];
                current_pixel[1] = current_pixel[1]+motion_vec[1][position_id];
                position_id++;
            }
            break;

        case(3):
            position_id=2;
            while( check_edges() )
            {
                current_pixel[0] = current_pixel[0]+motion_vec[0][position_id];
                current_pixel[1] = current_pixel[1]+motion_vec[1][position_id];
                position_id++;
            }
            break;

        case(4):
            position_id=2;
            while( check_edges() )
            {
                current_pixel[0] = current_pixel[0]+motion_vec[0][position_id];
                current_pixel[1] = current_pixel[1]+motion_vec[1][position_id];
                position_id++;
            }
            break;

        case(5):
            position_id=4;
            while( check_edges() )
            {
                current_pixel[0] = current_pixel[0]+motion_vec[0][position_id];
                current_pixel[1] = current_pixel[1]+motion_vec[1][position_id];
                position_id++;
            }
            break;

        case(6):
            position_id=4;
            while( check_edges() )
            {
                current_pixel[0] = current_pixel[0]+motion_vec[0][position_id];
                current_pixel[1] = current_pixel[1]+motion_vec[1][position_id];
                position_id++;
            }
            break;

        case(7):
            position_id=6;
            while( check_edges() )
            {
                current_pixel[0] = current_pixel[0]+motion_vec[0][position_id];
                current_pixel[1] = current_pixel[1]+motion_vec[1][position_id];
                position_id++;
            }
            break;

        case(8):
            position_id=6;
            while( check_edges() )
            {
                current_pixel[0] = current_pixel[0]+motion_vec[0][position_id];
                current_pixel[1] = current_pixel[1]+motion_vec[1][position_id];
                position_id++;
            }
            break;

        case(9):
            position_id=0;
            while( check_edges() )
            {
                current_pixel[0] = current_pixel[0]+motion_vec[0][position_id];
                current_pixel[1] = current_pixel[1]+motion_vec[1][position_id];
                position_id++;
            }
            break;

        case(10):
            position_id=0;
            while( check_edges() )
            {
                current_pixel[0] = current_pixel[0]+motion_vec[0][position_id];
                current_pixel[1] = current_pixel[1]+motion_vec[1][position_id];
                position_id++;
            }
            break;
        case(11):
            position_id=7;
            while( check_edges() )
            {
                current_pixel[0] = current_pixel[0]+motion_vec[0][position_id];
                current_pixel[1] = current_pixel[1]+motion_vec[1][position_id];
                position_id++;
            }
            break;
        case(12):
            position_id=3;
            while( check_edges() )
            {
                current_pixel[0] = current_pixel[0]+motion_vec[0][position_id];
                current_pixel[1] = current_pixel[1]+motion_vec[1][position_id];
                position_id++;
            }
            break;
        case(13):
            position_id=1;
            while( check_edges() )
            {
                current_pixel[0] = current_pixel[0]+motion_vec[0][position_id];
                current_pixel[1] = current_pixel[1]+motion_vec[1][position_id];
                position_id++;
            }
            break;
        case(14):
            position_id=5;
            while( check_edges() )
            {
                current_pixel[0] = current_pixel[0]+motion_vec[0][position_id];
                current_pixel[1] = current_pixel[1]+motion_vec[1][position_id];
                position_id++;
            }
            break;
        default:
            cout << "case not covered.";
            break;
    }
    if((current_pixel[0]!=full_layout.door_loc[i][0])||(current_pixel[1]!=full_layout.door_loc[i][1]))
    {
        border_vec.push_back(current_pixel);
        --position_id;
        current_pixel[0] = current_pixel[0]-motion_vec[0][position_id];
        current_pixel[1] = current_pixel[1]-motion_vec[1][position_id];
    }
    }
    for(int j=0; j<border_vec.size(); j++)
    {
        //cout << border_vec[j][0]<<','<<border_vec[j][1]<<'\n';
    }

}

bool check_edges(){
    bool param=true;
    for(int i=0; i<full_layout.door_loc.size(); i++)
    {
        if((full_layout.door_loc[i][1]==current_pixel[1])&&(full_layout.door_loc[i][0]==current_pixel[0]))
           return true;
        else if((current_pixel[1]<0)||(current_pixel[0]<0))
            return true;
        else if((current_pixel[1]>(building.size()-1))||(current_pixel[0]>(building.size()-1)))
            return true;
        else if (building[(building.size()-1)-current_pixel[1]][current_pixel[0]]!=1)
           param=false;
    }
    
    return param;
}
bool check_door(int i){
        if((full_layout.door_loc[i][1]==(current_pixel[1]))&&(full_layout.door_loc[i][0]==current_pixel[0]))
            return false;
        else return true;

}
