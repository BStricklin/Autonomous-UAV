close all; clear all; clc;
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

%   boundingBoxes = [350, 30, 30, 400];
% boundingBoxes = [1, 500, 30, 500];
boundingBoxes = [200, 30, 30, 500; 180, 29, 29, 480; 160, 25, 25, 450];
  imgHeight = 500; % image height in pixels
  table = helperTableOfScales(boundingBoxes, imgHeight);