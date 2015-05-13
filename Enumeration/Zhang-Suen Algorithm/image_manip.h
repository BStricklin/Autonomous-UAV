//
//  image_manip.h
//  Zhang_Suen
//
//  Created by Allison Badgett on 4/9/15.
//  Copyright (c) 2015 Allison Badgett. All rights reserved.
//

#include <vector>
using namespace std;

vector<vector<int>> sub_one(vector<vector<int>>&);
vector<vector<int>> sub_two(vector<vector<int>>&);

int num_nonzero(vector<vector<int>> &mod_img, int row, int col)
{
    vector<vector<int>> ring = { {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1} };
    int counter=0;
    for(int i = 0; i<ring.size(); i++)
    {
        bool edge_check = (((row!=0)&&((i!=0)||(i!=1)||(i!=7))) && ((col!=0)&&((i!=7)||(i!=6)||(i!=5))) && ((row!=(mod_img.size()-1))&&((i!=3)||(i!=4)||(i!=5))) && ((col!=(mod_img[i].size()-1))&&((i!=1)||(i!=2)||(i!=3))));
        
        if(edge_check){
            if (mod_img[row+ring[i][0]][col+ring[i][1]]==0) counter++;
        }
        else counter++;
    }
    
    return counter;
}

int zero_one_pattern(vector<vector<int>> &mod_img, int row, int col)
{
    vector<vector<int>> ring = { {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1} };
    int counter=0;
    for(int i = 1; i<ring.size(); i++)
    {
        bool edge_check = (((row!=0)&&((i!=0)||(i!=1)||(i!=7))) && ((col!=0)&&((i!=7)||(i!=6)||(i!=5))) && ((row!=(mod_img.size()-1))&&((i!=3)||(i!=4)||(i!=5))) && ((col!=(mod_img[i].size()-1))&&((i!=1)||(i!=2)||(i!=3))));
        
        if (edge_check) {
            if (mod_img[row+ring[i][0]][col+ring[i][1]]&&!mod_img[row+ring[i-1][0]][col+ring[i-1][1]]) counter++;
        }
        if(i==ring.size()-1)
        {
            if (edge_check)
                if (!mod_img[row+ring[i][0]][col+ring[i][1]]&&mod_img[row+ring[0][0]][col+ring[0][1]]) counter++;
        }
        
    }
    
    return counter;
}

int sub_one_cond_c(vector<vector<int>> &mod_img, int row, int col)
{
    vector<vector<int>> ring = { {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1} };
    int counter;
    bool edge_check = (row!=0)&&(row!=(mod_img.size()-1))&&(col!=0)&&(col!=(mod_img[row].size()-1));
    
    if(edge_check)
        counter = mod_img[row+ring[0][0]][col+ring[0][1]] * mod_img[row+ring[2][0]][col+ring[2][1]] * mod_img[row+ring[4][0]][col+ring[4][1]];
    
    return counter;
}

int sub_one_cond_d(vector<vector<int>> &mod_img, int row, int col)
{
    vector<vector<int>> ring = { {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1} };
    int counter;
    bool edge_check = (row!=0)&&(row!=(mod_img.size()-1))&&(col!=0)&&(col!=(mod_img[row].size()-1));
    if (edge_check)
        counter = mod_img[row+ring[2][0]][col+ring[2][1]] * mod_img[row+ring[4][0]][col+ring[4][1]] * mod_img[row+ring[6][0]][col+ring[6][1]];
    
    return counter;
}

int sub_two_cond_c(vector<vector<int>> &mod_img, int row, int col)
{
    vector<vector<int>> ring = { {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1} };
    int counter=0;
    bool edge_check = (row!=0)&&(row!=(mod_img.size()-1))&&(col!=0)&&(col!=(mod_img[row].size()-1));
    if (edge_check)
        counter = mod_img[row+ring[0][0]][col+ring[0][1]] * mod_img[row+ring[2][0]][col+ring[2][1]] * mod_img[row+ring[6][0]][col+ring[6][1]];
    
    return counter;
}

int sub_two_cond_d(vector<vector<int>> &mod_img, int row, int col)
{
    vector<vector<int>> ring = { {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1} };
    int counter=0;
    bool edge_check = (row!=0)&&(row!=(mod_img.size()-1))&&(col!=0)&&(col!=(mod_img[row].size()-1));
    if (edge_check)
        counter = mod_img[row+ring[0][0]][col+ring[0][1]] * mod_img[row+ring[4][0]][col+ring[4][1]] * mod_img[row+ring[6][0]][col+ring[6][1]];
    
    return counter;
}

vector<vector<int>> sub_one(vector<vector<int>> &mod_img)
{
    vector<vector<int>> delete_list;
    
    for (int i = 0; i < mod_img.size(); i++)
    {
        for (int j =0; j < mod_img[i].size(); j++)
        {
            bool sub_one_check = (num_nonzero(mod_img, i, j)>=2)&&(num_nonzero(mod_img, i, j)<=6);
            sub_one_check *= (zero_one_pattern(mod_img, i, j)==1);
            sub_one_check *= !sub_one_cond_c(mod_img, i, j);
            sub_one_check *= !sub_one_cond_d(mod_img, i, j);
            
            if(sub_one_check)
            {
                vector<int> coords;
                coords.push_back(i);
                coords.push_back(j);
                delete_list.push_back(coords);
            }
        }
    }
    
    for (int i = 0; i < delete_list.size(); i++)
    {
        mod_img[delete_list[i][0]][delete_list[i][1]] = '*';
    }
    
    
    return mod_img;
}

vector<vector<int>> sub_two (vector<vector<int>> &mod_img)
{
    vector<vector<int>> delete_list;
    
    for (int i = 0; i < mod_img.size(); i++)
    {
        for (int j =0; j < mod_img[i].size(); j++)
        {
            bool sub_two_check = (num_nonzero(mod_img, i, j)>=2)&&(num_nonzero(mod_img, i, j)<=6);
            sub_two_check *= (zero_one_pattern(mod_img, i, j)==1);
            sub_two_check *= !sub_two_cond_c(mod_img, i, j);
            sub_two_check *= !sub_two_cond_d(mod_img, i, j);
            
            if(sub_two_check)
            {
                vector<int> coords;
                coords.push_back(i);
                coords.push_back(j);
                delete_list.push_back(coords);
            }
        }
    }
    
    for (int i = 0; i < delete_list.size(); i++)
    {
        mod_img[delete_list[i][0]][delete_list[i][1]] = '*';
    }
    
    return mod_img;
}
