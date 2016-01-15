close all
clear all
clear all
load('log_data_all3')
load('params');
params=x;
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

UWBRange1=0; UWBRange2=0; UWBRange3=0; UWBRange4=0;
startFlag=0;
completeFlag=0;
rangeVect=[0;0;0;0];
z_estV=[0;0;0;0];
log_data=[];
valid=0;

for i=1:length(log_data_seq.Range)
    %for i=1:1000000
        Adress=log_data_seq.Adress(i);
        Range=log_data_seq.Range(i);
      R_WB=RotFromQuatJ(log_data_seq.Tracker_q(:,i));
      
  rel_pos=(log_data_seq.TrackerPos(:,i)-log_data_seq.TargetPos(:,i));
  rel_pos_v(:,i)=rel_pos;  
    if ((~isempty(Range))&(Range~=-1)&(Range<1e2))
        valid=1;   
        [x,P,z_est]=TargetEstimation2(Adress,Range,params,R_WB);
        x_log(:,i)=x;
        switch Adress
            
            case 0
                UWBRange1= Range;
                startFlag=1;
                completeFlag=0;
                rangeVect(1)=Range;
                z_estV(1)=z_est;
            case 1
                UWBRange2= Range;
                rangeVect(2)=Range;
                z_estV(2)=z_est;
            case 2
                UWBRange3= Range;
                rangeVect(3)=Range;
                z_estV(3)=z_est;
            case 3
                UWBRange4= Range;
                startFlag=0;
                completeFlag=1;
                rangeVect(4)=Range;
                z_estV(4)=z_est;
            otherwise
        end
        UWBRange=[UWBRange1;UWBRange2;UWBRange3;UWBRange4];
        
        
    end
    %set(q1,'UData',x(1),'VData',x(2),'WData',x(3))
   
    %set(h,'XData',xp,'YData',yp,'ZData',zp)
    
    
    

    % storing the data
    if completeFlag==1
        log_data=[log_data,[rangeVect;z_estV]];
    end
    log_data_seq_run(:,i)=[double(Adress);double(Range);double(z_est)];
    
end

plotting