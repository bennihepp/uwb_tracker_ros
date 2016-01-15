close all
clear all
clear all
ROS_MASTER_IP = '192.168.1.100';
ROS_IP = '192.168.1.2';
setenv('ROS_MASTER_URI', ['http://',ROS_MASTER_IP,':11311']);
setenv('ROS_IP', ROS_IP);

addpath(genpath(fullfile(fileparts(fileparts(pwd)), 'tools'))) % standard matlab calibration
scp1=Scope(4);
figure(2)
q1=quiver3(0,0,0,1,1,1,'r');
axis(15*[-1,1,-1,1,-1,1])
[xs,ys,zs] = sphere(20);
ap = [xs(:) ys(:) zs(:)]';
[v,d]=eig(eye*(3));
d = 1 * sqrt(d); % convert variance to sdwidth*sd
bp = (v*d*ap) + repmat([0;0;0], 1, size(ap,2));
xp = reshape(bp(1,:), size(xs));
yp = reshape(bp(2,:), size(ys));
zp = reshape(bp(3,:), size(zs));
hold on
h = surf( xp,yp,zp);
hold off
set(h,'facealpha',0.2);
try
    rosinit
    %rosinit(['http://',ROS_MASTER_IP, ':11311']);
catch
    display('Ros is already started');
    rosshutdown
    rosinit(['http://',ROS_MASTER_IP, ':11311']);
end

% uwb_pub=rospublisher('/uwb1','uwb/UWBRangeStats','IsLatching', false);
% uwb_msg=rosmessage(uwb_pub);

% fprintf('Ros master at %s, host IP: %s\n', ROS_MASTER_IP, ROS_IP)
% %rostopic echo /uwb/range
% %sub = rossubscriber('/uwb/range',@callback, 'BufferSize', 1);
subUWBRange = rossubscriber('/uwb/range');
subUWBStats = rossubscriber('/uwb/range_stats');
pause(1)
i = 1;
UWBRange1=0; UWBRange2=0; UWBRange3=0; UWBRange4=0;
startFlag=0;
completeFlag=0;
rangeVect=[0;0;0;0];
log_data=[];
log_data_seq=[];
while true
    %for i=1:1000000
    Adress=subUWBRange.LatestMessage.Address;
    Range=double(subUWBRange.LatestMessage.Range);
    
     if ((~isempty(Range))&(Range~=-1)&(Range<1e2))
        valid=1;   
        [x,P,z_est]=TargetEstimation(Adress,Range);
        

            switch Adress
                
                case 0
                    UWBRange1= Range;
                    startFlag=1;
                    completeFlag=0;
                    rangeVect(1)=Range;
                case 1
                    UWBRange2= Range;
                    rangeVect(2)=Range;
                case 2
                    UWBRange3= Range;
                    rangeVect(3)=Range;
                case 3
                    UWBRange4= Range;
                    startFlag=0;
                    completeFlag=1;
                    rangeVect(4)=Range;
                otherwise
            end
            UWBRange=[UWBRange1;UWBRange2;UWBRange3;UWBRange4];
            scp1.plot(i,double(UWBRange));
        end

    %UWBRange= subUWBRange.LatestMessage.Range;
    %    subUWBStats.LatestMessage.ChannelImpulseResponsePower;
    %    std=subUWBStats.LatestMessage.StdNoise;
    %    pwr2=subUWBStats.LatestMessage.FirstPathAmp2;
    set(q1,'UData',x(1),'VData',x(2),'WData',x(3))
    [v,d]=eig(P);
    d = 1 * sqrt(d); % convert variance to sdwidth*sd
    bp = (v*d*ap) + repmat(x(1:3), 1, size(ap,2));
    xp = reshape(bp(1,:), size(xs));
    yp = reshape(bp(2,:), size(ys));
    zp = reshape(bp(3,:), size(zs));
    %set(h,'XData',xp,'YData',yp,'ZData',zp)
    
    
    
    drawnow limitrate
    % storing the data
    if completeFlag==1
        log_data=[log_data,rangeVect];
    end
    log_data_seq(:,i)=[double(Adress);double(Range);double(h)];
    i = i + 1;
end
figure(4)
plot(log_data(1,:),'r')
hold on
plot(log_data(2,:),'g')
plot(log_data(3,:),'b')
plot(log_data(4,:),'c')
grid on
grid minor

figure(5)
ind1=find((log_data_seq(1,:)==1)&(log_data_seq(2,:)<1e2));
plot(log_data_seq(2,ind1),'r')
hold on
plot(log_data_seq(3,ind1),'g')


figure(6)
ind1=find((log_data_seq(1,:)==2)&(log_data_seq(2,:)<1e2));
plot(log_data_seq(2,ind1),'r')
hold on
plot(log_data_seq(3,ind1),'g')

save('log_data_walking','log_data_seq')