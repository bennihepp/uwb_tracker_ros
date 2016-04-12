%% Load file with data
%
% The file should contain the following fields:
% tracker_pos: Ground-truth positions of individual UWB units
% tracker_transform.pos: Ground-truth position of tracker coordinate frame
% tracker_transform.rot: Ground-truth quaternion of tracker coordinate frame
% uwb_ranges: UWB ranges from tracker UWB units to target unit
% vicon_ranges: Ground-truth ranges from tracker UWB units to target unit
%
[FileName, PathName, ~] = uigetfile('*.mat', 'Select MAT file to open');
data = load(fullfile(PathName, FileName));

%% Parameters
VICON_OUTLIER_DISTANCE = 0.5;
num_of_units = data.num_of_units;

uwb_ranges = [];
gt_ranges = [];
gt_positions = zeros(0, num_of_units, 3);
for i=1:numel(data.time)
    %% Detect vicon outliers
    outlier = false;
    for j = 1:num_of_units
        d = norm(data.tracker_transform.pos(:, i) - squeeze(data.tracker_pos(i, j, :)));
        if d > VICON_OUTLIER_DISTANCE
            outlier = true;
            break;
        end
    end
    if ~outlier
        % Ground truth range
        gt_ranges = [gt_ranges; data.vicon_ranges(i, :)];
        % UWB range
        uwb_ranges = [uwb_ranges; data.uwb_ranges(i, :)];
        % UWB position in tracker coordinate frame
        rel_position = squeeze(data.tracker_pos(i, :, :)) - repmat(data.tracker_transform.pos(:, i)', [num_of_units, 1]);
        tracker_tf_quaternion = data.tracker_transform.rot(:, i)';
        rel_position = quatrotate(tracker_tf_quaternion, rel_position);
        gt_positions(end+1, :, :) = rel_position;
    end
end

calibrate();
