//
//  building.cpp
//  critical_points
//
//  Created by Allison Badgett on 2/26/15.
//  Copyright (c) 2015 Allison Badgett. All rights reserved.
//

#include "building.h"
Room find_centroid(Room r)
{
    int lowest_y=building.size();
    int highest_y=0;
    for(int i=0;i<r.room_border.size(); i++)
    {
        //find element with lowest y
        if(r.room_border[i][1]<lowest_y)
            lowest_y=r.room_border[i][1];
        
        //find element with highest y
        if(r.room_border[i][1]>highest_y)
            highest_y=r.room_border[i][1];
    }
    
    int current_y=lowest_y;
    
    double counter=0; //set counter representing section width to one
    double cumulative_x_moment=0;
    double cumulative_y_moment=0;
    double cumulative_area =0;
    double center=0;
    int current_x;
    while(current_y<highest_y-1)
    {
        counter=0;
        int lowest_x=building.size();    //set lowest_x that must change
        for(int i=0; i<r.room_border.size();i++)
        {
            if(r.room_border[i][1]==lowest_y)
            {
                if(r.room_border[i][0]<lowest_x) lowest_x=r.room_border[i][0];
            }
        }
        
        current_x=lowest_x; //set current x to lowest x on row
        
        while((building[(building.size()-1)-current_y][current_x]!=1)&&!((current_x==r.door[0])&&(current_y==r.door[1])))
        {
            //move right until 1 or door is reached
            counter++;
            current_x++;
        }
        center = lowest_x + (counter-1)/2;  //find x-coord of center of row
        cumulative_x_moment+= counter * center; //find x_moment and add to total
        cumulative_y_moment += counter * (current_y+1);   //find y_moment and add to total
        cumulative_area += counter; //add area (counter) to total area
        current_y++;
    }
    r.centroid.push_back(cumulative_x_moment/cumulative_area);
    r.centroid.push_back(cumulative_y_moment/cumulative_area);
    //move up one y and lowest x of that y
    //move right until 1 or door is reached
    //continue until highest y is complete
    //do centroid calc
    return r;
}
vector<vector<int>> d_to_c(Room r){
    vector<vector<int>> path;
    vector<vector<int>> ring;
    int cx = r.centroid[0];
    int cy = r.centroid[1];
    int x = r.door[0];
    int y = r.door[1];
    char x_des, y_des;
    
    vector<int> temp;
    temp.push_back(x);
    temp.push_back(y);
    path.push_back(temp);
    
    while (!((x==cx)&&(y==cy)))   //centroid
    {

    //populate ring
        //index 0 - up
        //index 1 - right
        //index 2 - down
        //index 3 - left
        vector<int> coord;
        ring.clear();
        coord.push_back(y+1);
        coord.push_back(x);
        ring.push_back(coord);
        coord.clear();
        coord.push_back(y);
        coord.push_back(x+1);
        ring.push_back(coord);
        coord.clear();
        coord.push_back(y-1);
        coord.push_back(x);
        ring.push_back(coord);
        coord.clear();
        coord.push_back(y);
        coord.push_back(x-1);
        ring.push_back(coord);
        coord.clear();
    
    //finds desired motion
        if((cx - x)>0)
        {
            //desired motion ->true right
            if (building[(building.size()-1)-ring[1][0]][ring[1][1]]!=1)
            {
                x++;
            }
            cout << (building.size()-1)-ring[1][0] << ' ' << ring[1][1]<<'\n';
            
        }
        else if ((cx - x)<0)
        {
            //desired motion -> true left
            if (building[(building.size()-1)-ring[3][0]][ring[3][1]]!=1){
                x--;
            }
        }
        else
        {
            //desired motion -> no motion
            
        }
    
        if((cy - y)>0)
        {
            //desired motion ->true up
            if (building[(building.size()-1)-ring[0][0]][ring[0][1]]!=1)
            {
                y++;
            }
        }
        else if ((cy - y)<0)
        {
            //desired motion -> true down
            if (building[(building.size()-1)-ring[2][0]][ring[2][1]]!=1){
                y--;
            }
        }
        else
        {
            //desired motion -> no motion
        }
        vector<int> temp;
        temp.push_back(x);
        temp.push_back(y);
        path.push_back(temp);
    }
    int size = path.size();
    for (int i=(size-2); i>-1; i--)
    {
        path.push_back(path[i]);
    }

    return path;
}
vector<vector<int>> travel_path(vector<Hallway> h, int start_x, int start_y){
    int st_index;   //contains index of starting cell in first hallway
    int hall_num;   //contains index of first hallway to be traversed
    vector<vector<int>> path_coords; //contains x,y coordinates of cells to be traveled through
        
    for (int i =0; i<h.size(); i++)
    {
        for (int j=0;j<h[i].borders.size(); j++)
        {
            if((h[i].borders[j][0]==start_x)&&(h[i].borders[j][1]==start_y))
            {
                hall_num=i;
                st_index=j;
                break;
            }
        }
    }
    //lists hallways not used
    vector<Hallway> sub_halls;
    for (int q=0; q<h.size(); q++)
    {
        if(q!=hall_num) sub_halls.push_back(h[q]);
    }
    
    
    for (int i = st_index; i<h[hall_num].borders.size(); i++)
    {
        //store location in vector
        vector<int> temp;
        temp.push_back(h[hall_num].borders[i][0]);
        temp.push_back(h[hall_num].borders[i][1]);
        path_coords.push_back(temp);
        
        for (int j=0; j<h[hall_num].off_rooms.size(); j++)
        {
            if (((path_coords[path_coords.size()-1][0] == h[hall_num].off_rooms[j].door[0])&&(path_coords[path_coords.size()-1][1] == h[hall_num].off_rooms[j].door[1] + 1))||((path_coords[path_coords.size()-1][0] == h[hall_num].off_rooms[j].door[0])&&(path_coords[path_coords.size()-1][1] == h[hall_num].off_rooms[j].door[1] - 1))||((path_coords[path_coords.size()-1][1] == h[hall_num].off_rooms[j].door[1])&&(path_coords[path_coords.size()-1][0] == h[hall_num].off_rooms[j].door[0] + 1))||((path_coords[path_coords.size()-1][1] == h[hall_num].off_rooms[j].door[1])&&(path_coords[path_coords.size()-1][0] == h[hall_num].off_rooms[j].door[0] - 1)))
            {
                //adds motion inside room to vector
                vector<vector<int>> room_motion = d_to_c(h[hall_num].off_rooms[j]);
                path_coords.insert(path_coords.end(), room_motion.begin(), room_motion.end());
            }
        }
        //check if we are next to a subhall and go through it
        for (int p=0; p<sub_halls.size(); p++)
        {
            for (int q=0; q<sub_halls[p].borders.size();q++)
            {
                if (((path_coords[path_coords.size()-1][0] == sub_halls[p].borders[q][0])&&(path_coords[path_coords.size()-1][1] == sub_halls[p].borders[q][1] + 1))||((path_coords[path_coords.size()-1][0] == sub_halls[p].borders[q][0])&&(path_coords[path_coords.size()-1][1] == sub_halls[p].borders[q][1] - 1))||((path_coords[path_coords.size()-1][1] == sub_halls[p].borders[q][1])&&(path_coords[path_coords.size()-1][0] == sub_halls[p].borders[q][0] + 1))||((path_coords[path_coords.size()-1][1] == sub_halls[p].borders[q][1])&&(path_coords[path_coords.size()-1][0] == sub_halls[p].borders[q][0] - 1)))
                    {
                        //go through subhall
                        vector<vector<int>> sub_list = make_list(sub_halls[p], q);
                        path_coords.insert(path_coords.end(), sub_list.begin(), sub_list.end());
                    
                        //remove sub_hall[p] from vector
                        vector<Hallway> tlist;
                        for (int z=0; z<sub_halls.size(); z++)
                        {
                            if (z!=p) tlist.push_back(sub_halls[z]);
                        }
                        sub_halls = tlist;
                    }

            }
        }
        
    }
    for (int i = 1; i < st_index; i++)  //starts at one because first and last element of vector are the same
    {
        //store location in vector
        vector<int> temp;
        temp.push_back(h[hall_num].borders[i][0]);
        temp.push_back(h[hall_num].borders[i][1]);
        path_coords.push_back(temp);
        
        for (int j=0; j<h[hall_num].off_rooms.size(); j++)
        {
            if (((path_coords[path_coords.size()-1][0] == h[hall_num].off_rooms[j].door[0])&&(path_coords[path_coords.size()-1][1] == h[hall_num].off_rooms[j].door[1] + 1))||((path_coords[path_coords.size()-1][0] == h[hall_num].off_rooms[j].door[0])&&(path_coords[path_coords.size()-1][1] == h[hall_num].off_rooms[j].door[1] - 1))||((path_coords[path_coords.size()-1][1] == h[hall_num].off_rooms[j].door[1])&&(path_coords[path_coords.size()-1][0] == h[hall_num].off_rooms[j].door[0] + 1))||((path_coords[path_coords.size()-1][1] == h[hall_num].off_rooms[j].door[1])&&(path_coords[path_coords.size()-1][0] == h[hall_num].off_rooms[j].door[0] - 1)))
            {
                //adds motion inside room to vector
                vector<vector<int>> room_motion = d_to_c(h[hall_num].off_rooms[j]);
                path_coords.insert(path_coords.end(), room_motion.begin(), room_motion.end());
            }
        }
        //check if we are next to a subhall and go through it
        for (int p=0; p<sub_halls.size(); p++)
        {
            for (int q=0; q<sub_halls[p].borders.size();q++)
            {
                if (((path_coords[path_coords.size()-1][0] == sub_halls[p].borders[q][0])&&(path_coords[path_coords.size()-1][1] == sub_halls[p].borders[q][1] + 1))||((path_coords[path_coords.size()-1][0] == sub_halls[p].borders[q][0])&&(path_coords[path_coords.size()-1][1] == sub_halls[p].borders[q][1] - 1))||((path_coords[path_coords.size()-1][1] == sub_halls[p].borders[q][1])&&(path_coords[path_coords.size()-1][0] == sub_halls[p].borders[q][0] + 1))||((path_coords[path_coords.size()-1][1] == sub_halls[p].borders[q][1])&&(path_coords[path_coords.size()-1][0] == sub_halls[p].borders[q][0] - 1)))
                {
                    //go through subhall
                    vector<vector<int>> sub_list = make_list(sub_halls[p], q);
                    path_coords.insert(path_coords.end(), sub_list.begin(), sub_list.end());
                    
                    //remove sub_hall[p] from vector
                    vector<Hallway> tlist;
                    for (int z=0; z<sub_halls.size(); z++)
                    {
                        if (z!=p) tlist.push_back(sub_halls[z]);
                    }
                    sub_halls = tlist;
                }
                
            }
        }

        
    }
        //check for next hall
        //if exists, transition to next hall

    return path_coords;     //tracing algorithm makes diagonal corners, correct for this here
}
vector<string> motion_commands(){
    vector<string> motion_list; //stores a list of strings describing motion of the UAV through each square
    return motion_list;
}

vector<vector<int>> make_list(Hallway hall, int st_index)
{
    vector<vector<int>> path_coords;
    for (int i = st_index; i<hall.borders.size(); i++)
    {
        //store location in vector
        vector<int> temp;
        temp.push_back(hall.borders[i][0]);
        temp.push_back(hall.borders[i][1]);
        path_coords.push_back(temp);
        
        for (int j=0; j<hall.off_rooms.size(); j++)
        {
            if (((path_coords[path_coords.size()-1][0] == hall.off_rooms[j].door[0])&&(path_coords[path_coords.size()-1][1] == hall.off_rooms[j].door[1] + 1))||((path_coords[path_coords.size()-1][0] == hall.off_rooms[j].door[0])&&(path_coords[path_coords.size()-1][1] == hall.off_rooms[j].door[1] - 1))||((path_coords[path_coords.size()-1][1] == hall.off_rooms[j].door[1])&&(path_coords[path_coords.size()-1][0] == hall.off_rooms[j].door[0] + 1))||((path_coords[path_coords.size()-1][1] == hall.off_rooms[j].door[1])&&(path_coords[path_coords.size()-1][0] == hall.off_rooms[j].door[0] - 1)))
            {
                //adds motion inside room to vector
                vector<vector<int>> room_motion = d_to_c(hall.off_rooms[j]);
                path_coords.insert(path_coords.end(), room_motion.begin(), room_motion.end());
            }
        }
        
    }
    for (int i = 1; i < st_index; i++)  //starts at one because first and last element of vector are the same
    {
        //store location in vector
        vector<int> temp;
        temp.push_back(hall.borders[i][0]);
        temp.push_back(hall.borders[i][1]);
        path_coords.push_back(temp);
        
        for (int j=0; j<hall.off_rooms.size(); j++)
        {
            if (((path_coords[path_coords.size()-1][0] == hall.off_rooms[j].door[0])&&(path_coords[path_coords.size()-1][1] == hall.off_rooms[j].door[1] + 1))||((path_coords[path_coords.size()-1][0] == hall.off_rooms[j].door[0])&&(path_coords[path_coords.size()-1][1] == hall.off_rooms[j].door[1] - 1))||((path_coords[path_coords.size()-1][1] == hall.off_rooms[j].door[1])&&(path_coords[path_coords.size()-1][0] == hall.off_rooms[j].door[0] + 1))||((path_coords[path_coords.size()-1][1] == hall.off_rooms[j].door[1])&&(path_coords[path_coords.size()-1][0] == hall.off_rooms[j].door[0] - 1)))
            {
                //adds motion inside room to vector
                vector<vector<int>> room_motion = d_to_c(hall.off_rooms[j]);
                path_coords.insert(path_coords.end(), room_motion.begin(), room_motion.end());
            }
        }
        
    }
    //check for next hall
    //if exists, transition to next hall
    
    return path_coords;     //tracing algorithm makes diagonal corners, correct for this here
}
vector<Hallway> compare_halls(vector<Hallway>current_list, Hallway potential){
    //check potential against current list
    //if not in current list, add in potential
    return current_list;    //return new current list
}

