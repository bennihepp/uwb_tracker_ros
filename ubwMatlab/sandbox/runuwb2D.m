close all
clear all
%clear all

dt=0.01;
t=0:dt:50;
trajectory=[sin(t);cos(t)];
posTarget=[0;0];
posQuad=[0;0]
relUwbPos=0.1*[1,1;-1,10;0,-1]';

a1=45;
a2=3*45;
a3=6*45;
r=0.3
relUwbPos(:,1)= (posQuad+[cosd(a1)*r;sind(a1)*r]);
relUwbPos(:,2)= (posQuad+[cosd(a2)*r;sind(a2)*r]);
relUwbPos(:,3)= (posQuad+[cosd(a3)*r;sind(a3)*r]);

tarPlot=plot(rand(1),rand(1),'ro');
hold on
tarPlotest=plot(rand(1),rand(1),'g+');

for i=1:3
    plot(relUwbPos(1,i),relUwbPos(2,i),'bx')
end
grid on
axis(2*[-1,1,-1,1])
qv=1;

% model
F_r=[1,0,dt,0;
    0,1,0,dt;
    0,0,1,0;
    0,0,0,1];

F_b=eye(1);
P=ones(5);

x_r=[1;1;1;1];
x_b=ones(1);

x=[x_r;x_b]
F=blkdiag(F_r,F_b);
Q_r=diag([0,0,qv,qv]);
Q_b=diag([0]);
Q=blkdiag(Q_r,Q_b);
R=eye(3)*0.001;
for i=1:length(t)
    %% produce measurements
    posTarget=trajectory(:,i);
    % generate UWB messenges
    for indUWB=1:3
        zUWB(indUWB,1)=norm(posTarget-posQuad-relUwbPos(:,indUWB));
    end
    zUWB_log(:,i)=zUWB;
    set(tarPlot,'XData',posTarget(1),'YData',posTarget(2))
    set(tarPlotest,'XData',x(1),'YData',x(2))
    drawnow ;%limitrate;
    
    %% estimator
    x=F*x;
    P=F*P*F'+Q;
    %
    r=x(5);
    d=x(1:2);
    dx=d(1);dy=d(2);
%     a1=atan2(relUwbPos(1,1),relUwbPos(2,1));
%     a2=atan2(relUwbPos(1,2),relUwbPos(2,2));
%     a3=atan2(relUwbPos(1,3),relUwbPos(2,3));
    
    h_uwb(1,1)= norm(d+[cosd(a1)*r;sind(a1)*r]);
    h_uwb(2,1)= norm(d+[cosd(a2)*r;sind(a2)*r]);
    h_uwb(3,1)= norm(d+[cosd(a3)*r;sind(a3)*r]);
    
    H_1=[1/(2*sqrt(h_uwb(1))),0,0;
        0,1/(2*sqrt(h_uwb(2))),0;
        0,0,1/(2*sqrt(h_uwb(3)))];
    H_2=[2*dx+2*cos(a1)*r,2*dy+2*sin(a1)*r,0,0,2*dx*cos(a1)+2*cos(a1)^2*r+2*dy*sin(a1)+2*sin(a1)^2*r;
        2*dx+2*cos(a2)*r,2*dy+2*sin(a2)*r,0,0,2*dx*cos(a2)+2*cos(a2)^2*r+2*dy*sin(a2)+2*sin(a2)^2*r;
        2*dx+2*cos(a3)*r,2*dy+2*sin(a3)*r,0,0,2*dx*cos(a3)+2*cos(a3)^2*r+2*dy*sin(a3)+2*sin(a3)^2*r];
    H=H_1*H_2;
    zUWB
    res=zUWB-h_uwb;
    S=H*P*H'+R;
    K=P*H'/S;
    x=x+K*res
    P=(eye(5)-K*H)*P;
end

figure(2)
plot(zUWB_log(1,:),'r')
hold on
plot(zUWB_log(2,:),'g')
plot(zUWB_log(3,:),'b')

