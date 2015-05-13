//
//  neighbor_tracing.h
//  critical_points
//
//  Created by Allison Badgett on 2/19/15.
//  Copyright (c) 2015 Allison Badgett. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include "math.h"

using namespace std;


vector<vector<int>> neighbor_trace(int);

void next_border(int);
bool check_edges();
bool check_door(int);