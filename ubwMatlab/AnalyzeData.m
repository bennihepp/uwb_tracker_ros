close all
clear all
clear all
load('log_data_rotate_target')

addpath(genpath(fullfile(fileparts(fileparts(pwd)), 'tools'))) % standard matlab calibration
phi=[-60,60,180,0];
theta=[-120,-120,-120,0];
radius=0.13;
o_init=zeros(3*4,1);
        o_init(1)=radius*sind(theta(1))*cosd(phi(1));
        o_init(2)=radius*sind(theta(1))*sind(phi(1));
        o_init(3)=radius*cosd(theta(1));

        o_init(4)=radius*sind(theta(2))*cosd(phi(2));
        o_init(5)=radius*sind(theta(2))*sind(phi(2));
        o_init(6)=radius*cosd(theta(2));

        o_init(7)=radius*sind(theta(3))*cosd(phi(3));
        o_init(8)=radius*sind(theta(3))*sind(phi(3));
        o_init(9)=radius*cosd(theta(3));

        o_init(10)=radius*sind(theta(4))*cosd(phi(4));
        o_init(11)=radius*sind(theta(4))*sind(phi(4));
        o_init(12)=radius*cosd(theta(4));

o_offset=3*4;
x=[ones(o_offset,1);o_init];

R_WB=eye(3);


for iterations=1:1
    
H=[];
h_v=[];
h_vd=[];
Range_v=[];
    %for i=1:100
    for i=1:length(log_data_seq.Range)
        Adress=log_data_seq.Adress(i);
        Range=log_data_seq.Range(i);
        
        rel_pos=(log_data_seq.TrackerPos(:,i)-log_data_seq.TargetPos(:,i));
        pos=rel_pos;

    
        if ((~isempty(Range))&(Range~=-1)&(Range<1e2))
            %R_WB=RotFromQuatJ(log_data_seq.Tracker_q(:,i));

                
            h(1,i)=norm(log_data_seq.UWB1(:,i)- log_data_seq.UWB_Target1(:,i));
            h(2,i)=norm(log_data_seq.UWB2(:,i)- log_data_seq.UWB_Target1(:,i));
            h(3,i)=norm(log_data_seq.UWB3(:,i)- log_data_seq.UWB_Target1(:,i));
            h(4,i)=norm(log_data_seq.UWB4(:,i)- log_data_seq.UWB_Target1(:,i));
            zv=[nan;nan;nan;nan];
            zv(Adress+1)=Range;
            z_vector(i)=Range;
            z(:,i)=zv;
            
            stdv=[nan;nan;nan;nan];
            stdv(Adress+1)=log_data_seq.std(i);
            zv=[nan;nan;nan;nan];
            std(:,i)=stdv;
            
            pwd1v=[nan;nan;nan;nan];
            pwd2v=[nan;nan;nan;nan];
            pwd3v=[nan;nan;nan;nan];
            pwd1v(Adress+1)=log_data_seq.pwr2(1,i);
            pwd2v(Adress+1)=log_data_seq.pwr2(2,i);
            pwd3v(Adress+1)=log_data_seq.pwr2(3,i);
        
            pwd1(:,i)=pwd1v;
             pwd2(:,i)=pwd2v;
              pwd3(:,i)=pwd3v;
                    
  
            
            
        end
        
    end
    

    
end

%% save params:


figure(1)
ax1 = subplot(3,1,1)
plot(h(1,:),'r')
hold on
plot(h(2,:),'g')
plot(h(3,:),'b')
plot(h(4,:),'c')

% uwb plot
plot(z(1,:),'r+')
plot(z(2,:),'g+')
plot(z(3,:),'b+')
plot(z(4,:),'c+')
%plot(z_vector,'gx')
ax2 = subplot(3,1,2)
plot(std(1,:),'r+')
hold on
plot(std(2,:),'g+')
plot(std(3,:),'b+')
plot(std(4,:),'c+')


ax3 = subplot(3,1,3);
%plot(pwd1(1,:),'r+')
hold on


%plot(pwd1(2,:),'g+')
plot(pwd1(3,:),'r+')
%plot(pwd1(4,:),'c+')

%plot(pwd2(1,:),'r+')
%plot(pwd2(2,:),'g+')
plot(pwd2(3,:),'g+')
%plot(pwd2(4,:),'c+')

%plot(pwd3(1,:),'r+')
%plot(pwd3(2,:),'g+')
plot(pwd3(3,:),'b+')
%plot(pwd3(4,:),'c+')
box on
grid on
grid minor

%%validation

linkaxes([ax1, ax2, ax3], 'x');
