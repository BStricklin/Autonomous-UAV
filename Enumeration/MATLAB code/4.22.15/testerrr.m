clear all; close all; clc;

videoFile = 'test_vid0.avi'

% vision.VideoFileReader(videoFile, 'VideoOutputDataType', 'uint8')
% vision.VideoPlayer('Position', [200, 200, 400, 250])

implay(videoFile)