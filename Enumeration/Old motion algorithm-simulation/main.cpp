//
//  main.cpp
//  critical_points
//
//  Created by Allison Badgett on 2/12/15.
//  Copyright (c) 2015 Allison Badgett. All rights reserved.
//
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <vector>
#include "building.h"
#include "glut_display.h"
#include "neighbor_tracing.h"
#include "motion.h"
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
Building full_layout;
extern vector<vector<int>> r_border_vec;
vector<vector<int>>zero_loc, hall_loc, l_border_vec, pathway;
int width;
int height;
int window_id;
int l,m;
int m_index=0;

vector<vector<int>> building;

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    
    ifstream map;
    string line;
    building.clear();
    map.open("map3.txt");
    
    while(!map.eof())
    {
        getline(map, line);
        stringstream current_line(line);
        string buffer;
        vector<int> row;
        while(current_line >> buffer)
        {
            row.push_back(stoi(buffer));
        }
        
        building.push_back(row);
    }
    
    //find zeros
    for (int i =0; i<building.size(); i++)
    {
        for (int j =0; j<building.size(); j++)
        {
            if (building[(building.size()-1)-i][j]==0)
            {
                vector<int> temp;
                temp.push_back(j);  //store x
                temp.push_back(i);  //store y
                zero_loc.push_back(temp);
                if(i>0)
                {
                if(((building[building.size()-i][j]==1)&&(building[building.size()-i-2][j]==1))||((building[building.size()-1-i][j-1]==1)&&(building[building.size()-1-i][j+1]==1)))
                   {
                       full_layout.door_loc.push_back(temp);    //find doors
                   }
                }
            }
        }
    }

    for(int i=0; i<full_layout.door_loc.size();i++)
    {
        l_border_vec = neighbor_trace(i);
        
        bool exit_case = false;
        Room temp;
        Hallway temp2;
        
        if(r_border_vec.size()>l_border_vec.size())
        {
            vector<vector<int>>temp_door_list; //for storing doors off of hallway
            //store pointer to hall_border (which would be stored in hall_vec in Building) in Room's hall_border
            temp.hall_border=r_border_vec;
            for (int g=0; g<temp.hall_border.size();g++)
            {
                for (int h=0; h<full_layout.hall_info.size();h++)
                {
                    for (int p=0; p<full_layout.hall_info[h].borders.size();p++)
                    {
#warning changed h to p
                        if((temp.hall_border[g][0]==full_layout.hall_info[h].borders[p][0])&&(temp.hall_border[g][1]==full_layout.hall_info[h].borders[p][1]))
                        {
                            exit_case = true;
                        }
                    }
                }
            }
            temp2.borders=r_border_vec;
            //check for doors off this border (one point off) once all doors are found these doors dont need to be checked again for hallways
            for (int j =0; j<full_layout.door_loc.size(); j++)
            {
                for (int h=0; h<temp.hall_border.size();h++)
                {
                    if (((temp.hall_border[h][0] == full_layout.door_loc[j][0])&&(temp.hall_border[h][1] == full_layout.door_loc[j][1] + 1))||((temp.hall_border[h][0] == full_layout.door_loc[j][0])&&(temp.hall_border[h][1] == full_layout.door_loc[j][1] - 1))||((temp.hall_border[h][1] == full_layout.door_loc[j][1])&&(temp.hall_border[h][0] == full_layout.door_loc[j][0] + 1))||((temp.hall_border[h][1] == full_layout.door_loc[j][1])&&(temp.hall_border[h][0] == full_layout.door_loc[j][0] - 1)))
                    {
                        vector<int> temp_door;
                        temp_door.push_back(full_layout.door_loc[j][0]);
                        temp_door.push_back(full_layout.door_loc[j][1]);
                        temp_door_list.push_back(temp_door);
                    }
                }
            }
            temp2.doors=temp_door_list;
            temp.room_border=l_border_vec;
            temp.door=full_layout.door_loc[i];
            full_layout.room_info.push_back(temp);
            if (!exit_case) full_layout.hall_info.push_back(temp2);
        }
        else
        {
            vector<vector<int>> temp_door_list;
            temp.hall_border=l_border_vec;
            for (int g=0; g<temp.hall_border.size();g++)
            {
                for (int h=0; h<full_layout.hall_info.size();h++)
                {
                    for (int p=0; p<full_layout.hall_info[h].borders.size();p++)
                    {
#warning changed h to p
                        if((temp.hall_border[g][0]==full_layout.hall_info[h].borders[p][0])&&(temp.hall_border[g][1]==full_layout.hall_info[h].borders[p][1]))
                        {
                            exit_case = true;
                        }
                    }
                }
            }

            temp2.borders=l_border_vec;
            for (int j =0; j<full_layout.door_loc.size(); j++)
            {
                for (int h=0; h<temp.hall_border.size();h++)
                {
                    if (((temp.hall_border[h][0] == full_layout.door_loc[j][0])&&(temp.hall_border[h][1] == full_layout.door_loc[j][1] + 1))||((temp.hall_border[h][0] == full_layout.door_loc[j][0])&&(temp.hall_border[h][1] == full_layout.door_loc[j][1] - 1))||((temp.hall_border[h][1] == full_layout.door_loc[j][1])&&(temp.hall_border[h][0] == full_layout.door_loc[j][0] + 1))||((temp.hall_border[h][1] == full_layout.door_loc[j][1])&&(temp.hall_border[h][0] == full_layout.door_loc[j][0] - 1)))
                    {
                        vector<int> temp_door;
                        temp_door.push_back(full_layout.door_loc[j][0]);
                        temp_door.push_back(full_layout.door_loc[j][1]);
                        temp_door_list.push_back(temp_door);
                        break;
                    }
                }
            }

            temp2.doors=temp_door_list;
            temp.room_border=r_border_vec;
            temp.door=full_layout.door_loc[i];
            full_layout.room_info.push_back(temp);
            if (!exit_case) full_layout.hall_info.push_back(temp2);
        }
    }

    vector<int> index;
    
    for(int i=0; i<full_layout.room_info.size();i++)
    {
        full_layout.room_info[i]=find_centroid(full_layout.room_info[i]);
        //check that this centroid hasn't been found already - if so, remove from list.
    }
    
    
    //find rooms off each hallway
    for (int i =0; i<full_layout.hall_info.size();i++)
    {
        for (int j =0; j<full_layout.hall_info[i].doors.size(); j++)
        {
            for ( int k=0; k<full_layout.room_info.size(); k++)
            {
                if ((full_layout.hall_info[i].doors[j][0] == full_layout.room_info[k].door[0])&&(full_layout.hall_info[i].doors[j][1] == full_layout.room_info[k].door[1]))
                {
                    full_layout.hall_info[i].off_rooms.push_back(full_layout.room_info[k]);
                }
            }
        }
    }
    //pathway = travel_path(full_layout.hall_info, 32 ,0);

    for (int i =0; i<pathway.size(); i++)
    {
        cout << pathway[i][0] << ' '<<pathway[i][1]<<'\n';
    }

    //get motion commands from pathway vector
    motion_commands(pathway);
    
    create_window();

    glutMainLoop();
    
}
