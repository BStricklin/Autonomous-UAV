%% Tracking Pedestrians from a Moving Car
%
% This example shows how to track pedestrians using a camera mounted in a
% moving car.
%
% Copyright 2014 The MathWorks, Inc.

%% Overview
% This example shows how to perform automatic detection and tracking of
% people in a video from a moving camera. It demonstrates the flexibility
% of a tracking system adapted to a moving camera, which is ideal for
% automotive safety applications. Unlike the stationary camera
% example, <motion-based-multiple-object-tracking.html The Motion-Based
% Multiple Object Tracking>, it contains several additional algorithmic steps.
% These steps include people detection, customized non-maximum suppression,
% and heuristics to identify and eliminate false alarm tracks.
%
% This example is a function with the main body at the top and helper 
% routines in the form of 
% <matlab:helpview(fullfile(docroot,'toolbox','matlab','matlab_prog','matlab_prog.map'),'nested_functions') nested functions> 
% below.
% 


function PedestrianTrackingFromMovingCameraExample()
 global x
 x=0;
 global previous
 previous=0;
 global tracks
% Create system objects used for reading video, loading prerequisite data file, detecting pedestrians, and displaying the results.

videoFile = 'Web Video 1.wmv';

% vippedtracking.avi
% RingDunk.mov
scaleDataFile   = 'pedScaleTable.mat'; % An auxiliary file that helps to determine the size of a pedestrian at different pixel locations.

obj = setupSystemObjects(videoFile, scaleDataFile);

% Create an empty array of tracks.
tracks = initializeTracks(); 

% ID of the next track.
nextId = 1; 

% Set the global parameters.
% option.ROI                  = [1,60,319,120];   % A rectangle [x, y, w, h] that limits the processing area to ground locations.
option.ROI                  = [1,200,1270,520];   % A rectangle [x, y, w, h] that limits the processing area to ground locations.
option.scThresh             = 2;              % A threshold to control the tolerance of error in estimating the scale of a detected pedestrian. 
option.gatingThresh         = 0.9;              % A threshold to reject a candidate match between a detection and a track.
option.gatingCost           = 100;              % A large value for the assignment cost matrix that enforces the rejection of a candidate match.
option.costOfNonAssignment  = 10;               % A tuning parameter to control the likelihood of creation of a new track.
option.timeWindowSize       = 16;               % A tuning parameter to specify the number of frames required to stabilize the confidence score of a track.
option.confidenceThresh     = 2;                % A threshold to determine if a track is true positive or false alarm.
option.ageThresh            = 8;                % A threshold to determine the minimum length required for a track being true positive.
option.visThresh            = 0.6;              % A threshold to determine the minimum visibility value for a track being true positive.

frameNo = 0;
% Detect people and track them across video frames.
cont = ~isDone(obj.reader);


while cont, 
    frame   = readFrame();
% % % % % % % % % %     imresize(frame,0.5);
    frameNo = frameNo+1;

    [centroids, bboxes, scores] = detectPeople();
    predictNewLocationsOfTracks();    
    [assignments, unassignedTracks, unassignedDetections] = ...
        detectionToTrackAssignment();
   
    
    updateAssignedTracks();    
    updateUnassignedTracks();
    deleteLostTracks();   
    createNewTracks();
    
    displayTrackingResults();
    
    % Exit the loop if the video player figure is closed by user.     
    cont = ~isDone(obj.reader) && isOpen(obj.videoPlayer);
end


%% Auxiliary Input and Global Parameters of the Tracking System
% This tracking system requires a data file that contains information that
% relates the pixel location in the image to the size of the bounding box
% marking the pedestrian's location. This prior knowledge is stored in a
% vector |pedScaleTable|. The n-th entry in |pedScaleTable| represents the
% estimated height of an adult person in pixels. The index |n| references
% the approximate Y-coordinate of the pedestrian's feet.
%
% To obtain such a vector, a collection of training images were taken from
% the same viewpoint and in a similar scene to the testing environment. The
% training images contained images of pedestrians at varying distances
% from the camera. Using the 
% <matlab:helpview(fullfile(docroot,'toolbox','vision','vision.map'),'visionTrainingImageLabeler'); trainingImageLabeler>
% app, bounding boxes of the pedestrians in the images were manually
% annotated. The height of the bounding boxes together with the location of
% the pedestrians in the image were used to generate the scale data file
% through regression. Here is a helper function to show the algorithmic steps to 
% fit the linear regression model:
% <matlab:edit(fullfile(matlabroot,'toolbox','vision','visiondemos','helperTableOfScales.m')) |helperTableOfScales.m|>   
%
%
% There is also a set of global parameters that can be tuned to optimize
% the tracking performance. You can use the descriptions below to learn out
% how these parameters affect the tracking performance.
%
% * |ROI| :                 Region-Of-Interest in the form of
%                           [x, y, w, h]. It limits the processing area to 
%                           ground locations.
% * |scThresh| :            Tolerance threshold for scale estimation.
%                           When the difference between the detected scale
%                           and the expected scale exceeds the tolerance,
%                           the candidate detection is considered to be
%                           unrealistic and is removed from the output.
% * |gatingThresh| :        Gating parameter for the distance measure. When
%                           the cost of matching the detected bounding box
%                           and the predicted bounding box exceeds the
%                           threshold, the system removes the association
%                           of the two bounding boxes from tracking
%                           consideration.
% * |gatingCost| :          Value for the assignment cost matrix to
%                           discourage the possible tracking to detection 
%                           assignment.
% * |costOfNonAssignment| : Value for the assignment cost matrix for
%                           not assigning a detection or a track. Setting 
%                           it too low increases the likelihood of
%                           creating a new track, and may result in track 
%                           fragmentation. Setting it too high may result 
%                           in a single track corresponding to a series of
%                           separate moving objects.
% * |timeWindowSize| :      Number of frames required to estimate the
%                           confidence of the track.
% * |confidenceThresh| :    Confidence threshold to determine if the
%                           track is a true positive.
% * |ageThresh| :           Minimum length of a track being a true positive.
% * |visThresh| :           Minimum visibility threshold to determine if
%                           the track is a true positive.


%% Create System Objects for the Tracking System Initialization 
% The |setupSystemObjects| function creates system objects used for reading
% the video frames and the scale data file, to detect people, and display
% results. 
%
% For the people detector, we set the 'ClassificationThreshold' to be higher
% than the default value in order to remove false detections. We also
% disable the 'MergeDetections' option in order to perform customized, more
% robust non-maximum suppression that uses the scale data file described above.
%
% The |pedScaleTable| vector, which is stored in the scale data file,
% encodes our prior knowledge of the target and the scene. Once you have the
% regressor trained from your samples, you can compute the expected height
% at every possible Y-position in the image. These values are stored in the
% vector. The n-th entry in |pedScaleTable| represents our estimated height 
% of an adult person in pixels. The index |n| references the approximate
% Y-coordinate of the pedestrian's feet.

    function obj = setupSystemObjects(videoFile,scaleDataFile)
        % Initialize Video I/O
        % Create objects for reading a video from a file, drawing the 
        % detected and tracked people in each frame, and playing the video.
        
        % Create a video file reader.
        obj.reader = vision.VideoFileReader(videoFile, 'VideoOutputDataType', 'uint8');
        
        % Create a video player.
        obj.videoPlayer = vision.VideoPlayer('Position', [200, 200, 400, 250]);
        
        % Create a people detector and initialize the parameters. 
        % The height of a person in the video is expected to be between [96, 384] pixels. 
        obj.peopleDetector = vision.PeopleDetector('UprightPeople_96x48',...
                                                    'MinSize',[96,48],'MaxSize',[784,492], ...
                                                    'ClassificationThreshold',1.2, ...
                                                    'MergeDetections',false);

        % Load the scale data file                                        
        ld = load(scaleDataFile, 'pedScaleTable');
        obj.pedScaleTable = ld.pedScaleTable;
    end


%% Initialize Tracks
% The |initializeTracks| function creates an array of tracks, where each
% track is a structure representing a moving object in the video. The
% purpose of the structure is to maintain the state of a tracked object.
% The state consists of information used for detection-to-track assignment,
% track termination, and display. 
%
% The structure contains the following fields:
%
% * |id| :                  An integer ID of the track.
% * |color| :               The color of the track for display purpose.
% * |bboxes| :              A N-by-4 matrix to represent the bounding boxes 
%                           of the object with the current box at the last
%                           row. Each row has a form of [x, y, width,
%                           height].
% * |scores| :              An N-by-1 vector to record the classification
%                           score from the person detector with the current
%                           detection score at the last row.
% * |kalmanFilter| :        A Kalman filter object used for motion-based
%                           tracking. We track the center point of the
%                           object in image;
% * |age| :                 The number of frames since the track was
%                           initialized.
% * |totalVisibleCount| :   The total number of frames in which the object
%                           was detected (visible).
% * |confidence| :          A pair of two numbers to represent how
%                           confident we trust the track. It stores the 
%                           maximum and the average detection scores in the
%                           past within a predefined time window.
% * |predPosition| :        The predicted bounding box in the next frame.

    function tracks = initializeTracks()
        % Create an empty array of tracks
        tracks = struct(...
            'id', {}, ...
            'color', {}, ...
            'bboxes', {}, ...
            'scores', {}, ...
            'kalmanFilter', {}, ...
            'age', {}, ...
            'totalVisibleCount', {}, ...
            'confidence', {}, ...            
            'predPosition', {});
      
    end

%% Read a Video Frame
% Read the next video frame from the video file.
    function frame = readFrame()
        frame = step(obj.reader);
    end

%% Detect People
% The |detectPeople| function returns the centroids, the bounding boxes,
% and the classification scores of the detected people. It performs
% filtering and non-maximum suppression on the raw output of vision.peopleDetector.
%
% * |centroids| :         A N-by-2 matrix with each row in the form of [x,y].
% * |bboxes| :            A N-by-4 matrix with each row in the form of
%                         [x, y, width, height].
% * |scores| :            A N-by-1 vector with each element is the
%                         classification score at the corresponding frame.

    function [centroids, bboxes, scores] = detectPeople()
        % Only extract the region of interest to speed up processing.
        frameToDet = frame(option.ROI(2):option.ROI(2)+option.ROI(4), ...
                           option.ROI(1):option.ROI(1)+option.ROI(3), :);

        resizeRatio = 1/2;
        
        % Resize the image to increase the resolution of the pedestrian.
        % vision.PeopleDetector requires minimum size to operate.
        frameToDet = imresize(frameToDet, resizeRatio);
        
        % Run the people detector to produce detection candidates.
        [bboxes, scores] = step(obj.peopleDetector, frameToDet);
                
        % Resize back the output bounding boxes
        bboxes = bboxes / resizeRatio;
        
        % Shift the position of the bounding box back to the original frame.
        bboxes(:,1) = bboxes(:,1) + option.ROI(1);
        bboxes(:,2) = bboxes(:,2) + option.ROI(2);
        
        % Look up the estimated height of a pedestrian based on location of their feet.
        height = bboxes(:, 4);
        yfoot = min(length(obj.pedScaleTable), round(bboxes(:,2)+height));
        estHeight = obj.pedScaleTable(yfoot); 
        
        % Remove detections whose size deviates from the expected size, 
        % provided by the calibrated scale estimation. 
        invalid = abs(estHeight-height)>estHeight*option.scThresh;        
        bboxes(invalid, :) = [];
        scores(invalid, :) = [];

        % Apply non-maximum suppression to select the strongest bounding boxes.
        [bboxes, scores] = selectStrongestBbox(bboxes, scores, ...
                            'RatioType', 'Min', 'OverlapThreshold', 0.7);
        
        % Compute the centroids
        if isempty(bboxes)
            centroids = [];
        else
            centroids = [(bboxes(:, 1) + bboxes(:, 3) / 2), ...
                (bboxes(:, 2) + bboxes(:, 4) / 2)];
        end
    end

%% Predict New Locations of Existing Tracks
% Use the Kalman filter to predict the centroid of each track in the
% current frame, and update its bounding box accordingly. We take the width
% and height of the bounding box in previous frame as our current
% prediction of the size.

    function predictNewLocationsOfTracks()
        for i = 1:length(tracks)
            % Get the last bounding box on this track.
            bbox = tracks(i).bboxes(end, :);
            
            % Predict the current location of the track.
            predictedCentroid = predict(tracks(i).kalmanFilter);
            
            % Shift the bounding box so that its center is at the predicted location.
            tracks(i).predPosition = [predictedCentroid - bbox(3:4)/2, bbox(3:4)];
        end
    end

%% Assign Detections to Tracks
% Assigning object detections in the current frame to existing tracks is
% done by minimizing cost. The cost is computed using the |bboxOverlapRatio| 
% function, and is the overlap ratio between the predicted bounding box and 
% the detected bounding box. In this example, we assume the person will move 
% gradually in consecutive frames due to the high frame rate of the video 
% and the low motion speed of a person.
%
% The algorithm involves two steps: 
%
% Step 1: Compute the cost of assigning every detection to each track using
% the |bboxOverlapRatio| measure. As people move towards or away from the
% camera, their motion will not be accurately described by the centroid
% point alone. The cost takes into account the distance on the image plane as
% well as the scale of the bounding boxes. This prevents assigning
% detections far away from the camera to tracks closer to the
% camera, even if their centroids coincide. The choice of this cost function
% will ease the computation without resorting to a more sophisticated
% dynamic model. The results
% are stored in an MxN matrix, where M is the number of tracks, and N is
% the number of detections.
%
% Step 2: Solve the assignment problem represented by the cost matrix using
% the |assignDetectionsToTracks| function. The function takes the cost
% matrix and the cost of not assigning any detections to a track.
%
% The value for the cost of not assigning a detection to a track depends on
% the range of values returned by the cost function. This value must be
% tuned experimentally. Setting it too low increases the likelihood of
% creating a new track, and may result in track fragmentation. Setting it
% too high may result in a single track corresponding to a series of
% separate moving objects.
%
% The |assignDetectionsToTracks| function uses the Munkres' version of the
% Hungarian algorithm to compute an assignment which minimizes the total
% cost. It returns an M x 2 matrix containing the corresponding indices of
% assigned tracks and detections in its two columns. It also returns the
% indices of tracks and detections that remained unassigned.

    function [assignments, unassignedTracks, unassignedDetections] = ...
            detectionToTrackAssignment()
        
        % Compute the overlap ratio between the predicted boxes and the
        % detected boxes, and compute the cost of assigning each detection
        % to each track. The cost is minimum when the predicted bbox is
        % perfectly aligned with the detected bbox (overlap ratio is one)
        predBboxes = reshape([tracks(:).predPosition], 4, [])';
        cost = 1 - bboxOverlapRatio(predBboxes, bboxes);

        % Force the optimization step to ignore some matches by
        % setting the associated cost to be a large number. Note that this
        % number is different from the 'costOfNonAssignment' below.
        % This is useful when gating (removing unrealistic matches)
        % technique is applied.
        cost(cost > option.gatingThresh) = 1 + option.gatingCost;

        % Solve the assignment problem.
        [assignments, unassignedTracks, unassignedDetections] = ...
            assignDetectionsToTracks(cost, option.costOfNonAssignment);
    end

%% Update Assigned Tracks
% The |updateAssignedTracks| function updates each assigned track with the
% corresponding detection. It calls the |correct| method of
% |vision.KalmanFilter| to correct the location estimate. Next, it stores
% the new bounding box by taking the average of the size of recent (up to) 
% 4 boxes, and increases the age of the track and the total visible count 
% by 1. Finally, the function adjusts our confidence score for the track 
% based on the previous detection scores. 

    function updateAssignedTracks()
        numAssignedTracks = size(assignments, 1);
        for i = 1:numAssignedTracks
            trackIdx = assignments(i, 1);
            detectionIdx = assignments(i, 2);

            centroid = centroids(detectionIdx, :);
            bbox = bboxes(detectionIdx, :);
            
            % Correct the estimate of the object's location
            % using the new detection.
            correct(tracks(trackIdx).kalmanFilter, centroid);
            
            % Stabilize the bounding box by taking the average of the size 
            % of recent (up to) 4 boxes on the track. 
            T = min(size(tracks(trackIdx).bboxes,1), 4);
            w = mean([tracks(trackIdx).bboxes(end-T+1:end, 3); bbox(3)]);
            h = mean([tracks(trackIdx).bboxes(end-T+1:end, 4); bbox(4)]);
            tracks(trackIdx).bboxes(end+1, :) = [centroid - [w, h]/2, w, h];
            
            % Update track's age.
            tracks(trackIdx).age = tracks(trackIdx).age + 1;
            
            % Update visibility.
            tracks(trackIdx).totalVisibleCount = ...
                tracks(trackIdx).totalVisibleCount + 1;
            
            % Adjust track confidence score based on the maximum detection
            % score in the past 'timeWindowSize' frames.
            T = min(option.timeWindowSize, length(tracks(trackIdx).scores));
            score = tracks(trackIdx).scores(end-T+1:end);
            tracks(trackIdx).confidence = [max(score), mean(score)];
        end
    end

%% Update Unassigned Tracks
% The |updateUnassignedTracks| function marks each unassigned track as 
% invisible, increases its age by 1, and appends the predicted bounding box 
% to the track. The confidence is set to zero since we are not sure why it
% was not assigned to a track.

    function updateUnassignedTracks()
        for i = 1:length(unassignedTracks)
            idx = unassignedTracks(i);
            tracks(idx).age = tracks(idx).age + 1;
            tracks(idx).bboxes = [tracks(idx).bboxes; tracks(idx).predPosition];
            tracks(idx).scores = [tracks(idx).scores; 0];
            
            % Adjust track confidence score based on the maximum detection
            % score in the past 'timeWindowSize' frames
            T = min(option.timeWindowSize, length(tracks(idx).scores));
            score = tracks(idx).scores(end-T+1:end);
            tracks(idx).confidence = [max(score), mean(score)];
        end
    end

%% Delete Lost Tracks
% The |deleteLostTracks| function deletes tracks that have been invisible
% for too many consecutive frames. It also deletes recently created tracks
% that have been invisible for many frames overall.
% 
% Noisy detections tend to result in creation of false tracks. For this
% example, we remove a track under following conditions:
%
% * The object was tracked for a short time. This typically happens when a 
%   false detection shows up for a few frames and a track was initiated for it. 
% * The track was marked invisible for most of the frames. 
% * It failed to receive a strong detection within the past few frames, 
%   which is expressed as the maximum detection confidence score.

    function deleteLostTracks()
       
        if isempty(tracks)
            return;
        end        
        
        % Compute the fraction of the track's age for which it was visible.
        ages = [tracks(:).age]';
        totalVisibleCounts = [tracks(:).totalVisibleCount]';
        visibility = totalVisibleCounts ./ ages;
        
        % Check the maximum detection confidence score.
        confidence = reshape([tracks(:).confidence], 2, [])';
        maxConfidence = confidence(:, 1);

        % Find the indices of 'lost' tracks.
        lostInds = (ages <= option.ageThresh & visibility <= option.visThresh) | ...
             (maxConfidence <= option.confidenceThresh);

        % Delete lost tracks.
       tracks = tracks(~lostInds);
       
    end

%% Create New Tracks
% Create new tracks from unassigned detections. Assume that any unassigned
% detection is a start of a new track. In practice, you can use other cues
% to eliminate noisy detections, such as size, location, or appearance.

    function createNewTracks()
        unassignedCentroids = centroids(unassignedDetections, :);
        unassignedBboxes = bboxes(unassignedDetections, :);
        unassignedScores = scores(unassignedDetections);
        
        for i = 1:size(unassignedBboxes, 1)            
            centroid = unassignedCentroids(i,:);
            bbox = unassignedBboxes(i, :);
            score = unassignedScores(i);
            
            % Create a Kalman filter object.
            kalmanFilter = configureKalmanFilter('ConstantVelocity', ...
                centroid, [2, 1], [5, 5], 100);
            
            % Create a new track.
            newTrack = struct(...
                'id', nextId, ...
                'color', 255*rand(1,3), ...
                'bboxes', bbox, ...
                'scores', score, ...
                'kalmanFilter', kalmanFilter, ...
                'age', 1, ...
                'totalVisibleCount', 1, ...
                'confidence', [score, score], ...
                'predPosition', bbox);
            
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            %It appears that the tracks are not similar to what we thought 
%             if newTrack.color
%                x=x+1 
%             end
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            
            % Add it to the array of tracks.
            tracks(end + 1) = newTrack; %#ok<AGROW>
            
            % Increment the next id.
            nextId = nextId + 1;
        end
    end

%% Display Tracking Results
% The |displayTrackingResults| function draws a colored bounding box for
% each track on the video frame. The level of transparency of the box
% together with the displayed score indicate the confidence of the
% detections and tracks.
    
    function displayTrackingResults()
    current=0;
        if ~isempty(tracks),
            ages = [tracks(:).age]';        
            confidence = reshape([tracks(:).confidence], 2, [])';
            maxConfidence = confidence(:, 1);
            avgConfidence = confidence(:, 2);
            opacity = min(0.5,max(0.1,avgConfidence/3));
            noDispInds = (ages < option.ageThresh & maxConfidence < option.confidenceThresh) | ...
                       (ages < option.ageThresh / 2);
                   
            for i = 1:length(tracks)
                if ~noDispInds(i)
                    frame = insertShape(frame, ...
                                            'FilledRectangle', tracks(i).bboxes(end, :), ...
                                            'Color', tracks(i).color, ...
                                            'Opacity', opacity(i));
                    frame = insertObjectAnnotation(frame, ...
                                            'rectangle', tracks(i).bboxes(end, :), ...
                                            num2str(avgConfidence(i)), ...
                                            'Color', tracks(i).color);
                current=current+1
                end
            end
        end
        
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        %Rough approximation of people counted
        if previous == current
            x
        elseif previous<current
            x=x+(current-previous)
        
        end
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        previous=current;
        current=0;
        
        
        frame = insertText(frame,[10, 10],num2str(frameNo));
        frame = insertShape(frame, 'Rectangle', [option.ROI(1:2), option.ROI(3:4)], ...
                                'Color', [255, 0, 0], 'LineWidth', 3);
        step(obj.videoPlayer, frame);
        
    end

%%
displayEndOfDemoMessage(mfilename)
end