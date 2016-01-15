close all
clear all
clear all
ROS_MASTER_IP = '192.168.1.100';
ROS_IP = '192.168.1.2';
setenv('ROS_MASTER_URI', ['http://',ROS_MASTER_IP,':11311']);
setenv('ROS_IP', ROS_IP);

addpath(genpath(fullfile(fileparts(fileparts(pwd)), 'tools'))) % standard matlab calibration

try
    rosinit
    %rosinit(['http://',ROS_MASTER_IP, ':11311']);
catch
    display('Ros is already started');
    rosshutdown
    rosinit(['http://',ROS_MASTER_IP, ':11311']);
end


subUWBRange = rossubscriber('/uwb/range');
subUWBStats = rossubscriber('/uwb/range_stats');

subVicon_Target=rossubscriber('/vicon/UWB_Target/UWB_Target');
subVicon_Tracker=rossubscriber('/vicon/UWB_Tracker/UWB_Tracker');

subVicon_marker=rossubscriber('/vicon/markers/');
pause(1)
i = 1;

t=rostime('now');
t1 = double(t.Sec)+double(t.Nsec)*10^-9;
while true


    t=rostime('now');
    t2 = double(t.Sec)+double(t.Nsec)*10^-9;
    e=(t2-t1);
    if e>1/60
        %1/e
        
        t1=t2;
        %for i=1:1000000
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
            
            
            
            log_data_seq.dt(i)=e;
            log_data_seq.Adress(i)=double(Adress);
            log_data_seq.Range(i)=double(Range);
            
            log_data_seq.TargetPos(:,i)=double(Target_pos);
            log_data_seq.TrackerPos(:,i)=double(Tracker_pos);
            for cnt_marker=1:size(Markers,1)
                if strcmp(Markers(cnt_marker).MarkerName,'UWB1')
                    log_data_seq.UWB1(:,i)=0.001*[Markers(cnt_marker).Translation.X;Markers(cnt_marker).Translation.Y;Markers(cnt_marker).Translation.Z];
                end
                if strcmp(Markers(cnt_marker).MarkerName,'UWB2')
                    log_data_seq.UWB2(:,i)=0.001*[Markers(cnt_marker).Translation.X;Markers(cnt_marker).Translation.Y;Markers(cnt_marker).Translation.Z];
                end
                if strcmp(Markers(cnt_marker).MarkerName,'UWB3')
                    log_data_seq.UWB3(:,i)=0.001*[Markers(cnt_marker).Translation.X;Markers(cnt_marker).Translation.Y;Markers(cnt_marker).Translation.Z];
                end
                if strcmp(Markers(cnt_marker).MarkerName,'UWB4')
                    tmp=[Markers(cnt_marker).Translation.X;Markers(cnt_marker).Translation.Y;Markers(cnt_marker).Translation.Z];
                    log_data_seq.UWB4(:,i)=0.001*tmp;
                end
                
                if strcmp(Markers(cnt_marker).MarkerName,'UWB_Target1')
                    log_data_seq.UWB_Target1(:,i)=0.001*[Markers(cnt_marker).Translation.X;Markers(cnt_marker).Translation.Y;Markers(cnt_marker).Translation.Z];
                end
            end
            
            log_data_seq.Target_q(:,i)=double(Target_q);
            log_data_seq.Tracker_q(:,i)=double(Tracker_q);
            
            log_data_seq.std(:,i)=double(std);
            log_data_seq.pwr2(:,i)=double(pwr2);
            pwr2
            
            i = i + 1;
        end
        
        %UWBRange= subUWBRange.LatestMessage.Range;
        
        
    end

end


plot3(log_data_seq.TargetPos(1,:),log_data_seq.TargetPos(2,:),log_data_seq.TargetPos(3,:))

plot3(log_data_seq.UWB1(1,:),log_data_seq.UWB1(2,:),log_data_seq.UWB1(3,:),'rx')
hold on
plot3(log_data_seq.UWB2(1,:),log_data_seq.UWB2(2,:),log_data_seq.UWB2(3,:),'gx')
plot3(log_data_seq.UWB3(1,:),log_data_seq.UWB3(2,:),log_data_seq.UWB3(3,:),'bx')
plot3(log_data_seq.UWB4(1,:),log_data_seq.UWB4(2,:),log_data_seq.UWB4(3,:),'cx')

plot3(log_data_seq.UWB_Target1(1,:),log_data_seq.UWB_Target1(2,:),log_data_seq.UWB_Target1(3,:),'mx')
%axis(3*[-1,1,-1,1,-1,1]);
daspect([1,1,1])
%axis equal
grid on
grid minor
box on
save('log_data_rotate_target','log_data_seq')