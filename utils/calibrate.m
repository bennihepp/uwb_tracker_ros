% Calibration script for UWB units. Fits a linear model to the data and
% computes the spatial configuration of the UWB units.
%
% A Gauss-Newton fitting is run multiple times to update the estimated
% variance and the corresponding outlier-threshold.

%% Input and output
% Input:
% uwb_ranges: NxM array containing N range-measurements for the M UWB units
% gt_ranges: NxM array containing N ground-truth ranges for the M UWB units
% gt_positions: NxMx3 array containing N ground-truth positions for the M
%               UWB units
%
% Output: Writes a .mat and .yaml file containing the average positions of
%         the UWB units and the coefficients for a linear model.

%% Visualization
% Inliers are shown in colors.
% Outliers are shown in black.
% Ground truth is shown as crosses (x).
% UWB measurements are shown as pluses (+).

%% If UWB ranges and true positions are not already defined load them from a file
if ~exist('uwb_ranges', 'var') || ~exist('gt_ranges', 'var') || ~exist('gt_positions', 'var')
    [FileName, PathName, ~] = uigetfile('*.mat', 'Select MAT file to open');
    load(fullfile(PathName, FileName));
end

%% Parameters
num_of_measurements = size(uwb_ranges, 1);
num_of_units = size(uwb_ranges, 2);
num_of_iterations = 5;

%% Plots for visualization
figure();
cmap = colormap('lines');
uwb_lines = {};
gt_ranges_lines = {};
for j = 1:num_of_units
    color = cmap(j, :);
    uwb_lines{j} = animatedline('Marker', '+', 'LineStyle', 'none', 'Color', color);
    gt_ranges_lines{j} = animatedline('Marker', 'x', 'LineStyle', 'none', 'Color', color);
end
uwb_outliers = animatedline('Marker', '+', 'LineStyle', 'none', 'Color', 'k');
gt_outliers = animatedline('Marker', 'x', 'LineStyle', 'none', 'Color', 'k');
xlabel('measurement #');
ylabel('range');
drawnow;

%% Initialize coefficients and variances
% x contains the constant and linear coefficient for each unit
num_of_units = size(uwb_ranges, 2);
x = ones(2 * num_of_units, 1);
x(1:2:end) = 0;
uwb_variance = ones(size(uwb_ranges, 2), 1);
inlier_mask = logical(ones(size(uwb_ranges, 2), 1));

% Estimate a first-order model: groundtruth = p0 + p1 * UWB
for iterations = 1:num_of_iterations
    z_v_hat = [];
    z_v_true = [];
    H = [];

    gt_range_cell = cell(num_of_units, 1);
    measurement = cell(num_of_units, 1);

    % Visualization
    for j = 1:num_of_units
        clearpoints(uwb_lines{j});
        clearpoints(gt_ranges_lines{j});
    end
    clearpoints(uwb_outliers);
    clearpoints(gt_outliers);

    % Build Jacobian and model predictions
    for i=1:num_of_measurements
        for j = 1:num_of_units
            uwb_range = uwb_ranges(i, j);
            gt_range = gt_ranges(i, j);

            % Exclude invalid measurements
            if isempty(uwb_range) || (uwb_range < 0) || isnan(gt_range)
                continue;
            end
            % Coefficients for unit j
            p0 = x(j*2 - 1);
            p1 = x(j*2);

            % Model prediction
            z_hat = p0 + p1 * uwb_range;

            % Detect UWB outliers
            if abs(gt_range - z_hat) > 3*uwb_variance(j)
                % Visualize outlier
                addpoints(uwb_outliers, i, z_hat);
                addpoints(gt_outliers, i, gt_range);
                inlier_mask(i) = false;
            else
                % Visualize inlier
                addpoints(uwb_lines{j}, i, z_hat);
                addpoints(gt_ranges_lines{j}, i, gt_range);
                % Append prediction and Jacobian to full matrix
                z_v_hat = [z_v_hat; z_hat];
                z_v_true = [z_v_true; gt_range];
                % Jacobian
                H_p0 = 1;
                H_p1 = uwb_range;
                H_p = zeros(1, 4*2);
                H_p(2*j - 1:2*j) = [H_p0, H_p1];
                H = [H; H_p];
                % Save inlier ranges for adaptive variance computation
                gt_range_cell{j} = [gt_range_cell{j}, gt_range];
                measurement{j} = [measurement{j}, uwb_range];
            end
            drawnow limitrate;
        end
    end

    % Perform Gauss-Newton step
    x = x + (H'*H) \ H'*(z_v_true - z_v_hat);

    % Update estimate of UWB variance
    for j = 1:numel(gt_range_cell)
        p0 = x(j*2 - 1);
        p1 = x(j*2);
        new_estimate = p0 + p1 * measurement{j};
        uwb_variance(j) = std((new_estimate - gt_range_cell{j}));
    end

    uwb_variance
end


%% Create configuration file

params = struct();

% Write UWB positions into parameter structure
uwb_tracker_center = zeros(3, 1);
for i = 1:3
    uwb_tracker_center(i) = mean(mean(gt_positions(inlier_mask, :, i)));
end
for j = 1:num_of_units
    rel_uwb_pos = squeeze(mean(gt_positions(inlier_mask, j, :)));
    params = setfield(params, ['m_uwb', int2str(j)], rel_uwb_pos);
    a = 0;
    b = 1;
    params = setfield(params, ['UWB', int2str(j), 'p0'], a);
    params = setfield(params, ['UWB', int2str(j), 'p1'], b);
end

% Visualize UWB positions
figure(2);
hold on;
plot3(params.m_uwb1(1), params.m_uwb1(2), params.m_uwb1(3),'r+');
plot3(params.m_uwb2(1), params.m_uwb2(2), params.m_uwb2(3),'g+');
plot3(params.m_uwb3(1), params.m_uwb3(2), params.m_uwb3(3),'b+');
plot3(params.m_uwb4(1), params.m_uwb4(2), params.m_uwb4(3),'c+');
xlabel('x');
xlabel('y');
xlabel('z');
box on;
grid on;
grid minor;
daspect([1, 1, 1]);

% Write coefficients into parameter structure
for j = 0:num_of_units-1
    p0 = x(j*2 + 1);
    p1 = x(j*2 + 2);
    params = setfield(params, ['UWB', int2str(j), 'p0'], p0);
    params = setfield(params, ['UWB', int2str(j), 'p1'], p1);
end

% Save as .mat file
save('params', 'params')

% Generate YAML configuration
display('Generating yaml config');
f = fopen('config.yaml', 'w');
fprintf(f, 'num_of_units: %d\n', num_of_units);
for i = 1:num_of_units
    unit_id = i - 1;
    uwb_pos = getfield(params, ['m_uwb', int2str(i)]);
    uwb_p0 = getfield(params, ['UWB', int2str(i), 'p0']);
    uwb_p1 = getfield(params, ['UWB', int2str(i), 'p1']);
    fprintf(f, 'unit_%d:\n', unit_id);
    fprintf(f, '  x: %f\n', uwb_pos(1));
    fprintf(f, '  y: %f\n', uwb_pos(2));
    fprintf(f, '  z: %f\n', uwb_pos(3));
    fprintf(f, '  p0: %f\n', uwb_p0);
    fprintf(f, '  p1: %f\n', uwb_p1);
end
fclose(f);
