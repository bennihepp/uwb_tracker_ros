close all;
clear all;
clear all;
% ROS_MASTER_IP = '192.168.1.100';
% ROS_IP = '192.168.1.2';
% setenv('ROS_MASTER_URI', ['http://', ROS_MASTER_IP, ':11311']);

addpath(genpath(fullfile(fileparts(fileparts(pwd)), 'tools'))) % standard matlab calibration

try
    rosinit;
    %rosinit(['http://',ROS_MASTER_IP, ':11311']);
catch
    display('Ros is already started');
    rosshutdown;
    rosinit();
end


subUWBRange = rossubscriber('/uwb/range');
subUWBStats = rossubscriber('/uwb/range_stats');

subVicon_Target=rossubscriber('/vicon/UWB_Target/UWB_Target');
subVicon_Tracker=rossubscriber('/vicon/UWB_Tracker/UWB_Tracker');

subVicon_marker=rossubscriber('/vicon/markers/');
pause(1)
i = 1;

scope = Scope(3);
ylim(scope.axis(1), [2, 6]);

t=rostime('now');
t1 = double(t.Sec) + double(t.Nsec) * 10^-9;

while true
    t=rostime('now');
    t2 = double(t.Sec) + double(t.Nsec) * 10^-9;
    e=(t2 - t1);
    if e>1/60
        1/e
        Markers=subVicon_marker.LatestMessage.Markers_;
        Adress=subUWBRange.LatestMessage.Address;
        %Range=double(subUWBRange.LatestMessage.Range);
        %stats=double(subUWBStats.LatestMessage.Range);
        tmp_lm=subVicon_Target.LatestMessage.Transform;
        Target_pos=[tmp_lm.Translation.X;
            tmp_lm.Translation.Y;
            tmp_lm.Translation.Z];
        
        Target_q=[tmp_lm.Rotation.W;
            tmp_lm.Rotation.X;
            tmp_lm.Rotation.Y;
            tmp_lm.Rotation.Z];
        
        
        tmp_lm=subVicon_Tracker.LatestMessage.Transform;
        
        Tracker_pos=[tmp_lm.Translation.X;
            tmp_lm.Translation.Y;
            tmp_lm.Translation.Z];
        
        Tracker_q=[tmp_lm.Rotation.W;
            tmp_lm.Rotation.X;
            tmp_lm.Rotation.Y;
            tmp_lm.Rotation.Z];
        
        Range=double(subUWBRange.LatestMessage.Range);
        
        subUWBStats.LatestMessage.ChannelImpulseResponsePower;
        std=subUWBStats.LatestMessage.StdNoise;
        pwr2=subUWBStats.LatestMessage.FirstPathAmp2;
        
        if ((~isempty(Range))&(Range~=-1)&(Range<1e2))
            valid=1;

            dt(i)=e;
            Adress(i)=double(Adress);
            Range(i)=double(Range);
            
            TargetPos(:,i)=double(Target_pos);
            TrackerPos(:,i)=double(Tracker_pos);

            for cnt_marker=1:size(Markers,1)
                if strcmp(Markers(cnt_marker).MarkerName,'UWB1')
                    UWB1=0.001*[Markers(cnt_marker).Translation.X;Markers(cnt_marker).Translation.Y;Markers(cnt_marker).Translation.Z];
                end
                if strcmp(Markers(cnt_marker).MarkerName,'UWB2')
                    UWB2=0.001*[Markers(cnt_marker).Translation.X;Markers(cnt_marker).Translation.Y;Markers(cnt_marker).Translation.Z];
                end
                if strcmp(Markers(cnt_marker).MarkerName,'UWB3')
                    UWB3=0.001*[Markers(cnt_marker).Translation.X;Markers(cnt_marker).Translation.Y;Markers(cnt_marker).Translation.Z];
                end
                if strcmp(Markers(cnt_marker).MarkerName,'UWB4')
                    UWB4=0.001*[Markers(cnt_marker).Translation.X;Markers(cnt_marker).Translation.Y;Markers(cnt_marker).Translation.Z];
                end
                
                if strcmp(Markers(cnt_marker).MarkerName,'UWB_Target1')
                    UWB_Target1=0.001*[Markers(cnt_marker).Translation.X;Markers(cnt_marker).Translation.Y;Markers(cnt_marker).Translation.Z];
                end
            end

            Target_q(:,i)=double(Target_q);
            Tracker_q(:,i)=double(Tracker_q);
            
            std(:,i)=double(std);
            pwr2(:,i)=double(pwr2);

            dist1 = norm(UWB1- UWB_Target1);
            dist2 = norm(UWB2- UWB_Target1);
            dist3 = norm(UWB3- UWB_Target1);
            dist4 = norm(UWB4- UWB_Target1);
            scope.subplot(1, i, [dist1, dist2, dist3, dist4]);

            t1=t2;
            i = i + 1;
            drawnow('limitrate');
        end
    end
end
