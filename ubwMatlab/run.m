close all
ROS_MASTER_IP = '192.168.1.100';
ROS_IP = '192.168.1.2';
setenv(['ROS_MASTER_URI', 'http://',ROS_MASTER_IP,':11311']);
setenv('ROS_IP', ROS_IP);

addpath(genpath(fullfile(fileparts(pwd), 'tools'))) % standard matlab calibration
scp1=Scope(1);
try
    rosinit
    %rosinit(['http://',ROS_MASTER_IP, ':11311']);
catch
    display('Ros is already started');
    rosshutdown
    rosinit(['http://',ROS_MASTER_IP, ':11311']);
end
fprintf('Ros master at %s, host IP: %s\n', ROS_MASTER_IP, ROS_IP)
%rostopic echo /uwb/range
%sub = rossubscriber('/uwb/range',@callback, 'BufferSize', 1);
subUWB = rossubscriber('/uwb/range');
pause(0.1)
i = 1;
while true
%for i=1:1000000
   UWBRange= subUWB.LatestMessage.Range;
   if ~isempty(UWBRange)
       scp1.plot(i,double(UWBRange))
       drawnow limitrate
   end
   i = i + 1;
end