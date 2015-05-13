function pedScaleTable = helperTableOfScales(bbox, imgHeight)
% helperTableOfScales Estimates the height of a bounding box in image
%   This function helperTableOfScales is only in support of the
%   PedestrianTrackingFromMovingCameraExample. It may change in a future
%   release. It uses regress function from the Statistics Toolbox.
%
%   pedScaleTable = helperTableOfScales(bbox, imgHeight) returns an
%   imgHeight-by-1 table, pedScaleTable, with height estimates of a bounding
%   box at every y-pixel location in an image, given the training bounding
%   boxes in bbox. bbox is an N-by-4 matrix in the form of [X, Y, W, H].
%   imgHeight is the height of the image.
%
%   Example
%   -------
%   % The bounding boxes were collected using trainingImageLabeler by
%   % manually marking bounding boxes that enclose pedestrians.
%   boundingBoxes = [200, 30, 30, 50; 180, 29, 29, 48; 160, 25, 25, 45];
%   imgHeight = 480; % image height in pixels
%   table = helperTableOfScales(boundingBoxes, imgHeight);

%   Copyright 2014 The MathWorks, Inc.

% Set the bottom of the bounding box as predictor variable.
yBottom = bbox(:, 2) + bbox(:, 4);
% Set the height of the bounding box as response to the observation.
yHeight = bbox(:, 4);

% Set predictor variables up to 2nd degree.
X = [ones(size(yBottom)) yBottom yBottom.*yBottom];

% Fit a linear regression model using least squares.
b = regress(yHeight, X);

% Predict the height at every pixel location in an image.
y = 1:imgHeight;
pedScaleTable = b' * [ones(size(y)); y; y.*y];

