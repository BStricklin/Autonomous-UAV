//
//  image_manip.h
//  Based on work of Zhang Suen
//
//  Created by Allison Badgett on 4/9/15.
//  Copyright (c) 2015 Allison Badgett. All rights reserved.
//

#include <vector>
using namespace std;

vector<vector<int>> sub_one(vector<vector<int>>&);
vector<vector<int>> sub_two(vector<vector<int>>&);

int num_nonzero(vector<vector<int>> &mod_img, int row, int col);
int zero_one_pattern(vector<vector<int>> &mod_img, int row, int col);
int sub_one_cond_c(vector<vector<int>> &mod_img, int row, int col);
int sub_one_cond_d(vector<vector<int>> &mod_img, int row, int col);
int sub_two_cond_c(vector<vector<int>> &mod_img, int row, int col);
int sub_two_cond_d(vector<vector<int>> &mod_img, int row, int col);

vector<vector<int>> sub_one(vector<vector<int>> &mod_img);
vector<vector<int>> sub_two(vector<vector<int>> &mod_img);
