close all
clear all

dt=0.01;
t=0:dt:10;
trajectory=[sin(t);cos(t);0*t];
posTarget=[0;0;0];
posQuad=[0;0;0]
relUwbPos=0.1*[1,1,0;-1,-1,0;1,-1,0;0,0,1]';


tarPlot=plot3(rand(1),rand(1),rand(1),'ro');
grid on
axis(2*[-1,1,-1,1,-1,1])
qv=0.1;

% model
F_r=[1,0,0,dt,0,0;
    0,1,0,0,dt,0;
    0,0,1,0,0,dt;
    0,0,0,1,0,0;
    0,0,0,0,1,0;
    0,0,0,0,0,1];

F_b=eye(4*2+1);
P=eye(9);

x_r=[0;0;0;0;0;0];
x_b=zeros(4*3,1);

Q=diag([0,0,0,qv,qv,qv,0,0,0]);

for i=1:length(t)
    %% produce measurements
    posTarget=trajectory(:,i);
    % generate UWB messenges
    for indUWB=1:4
       zUWB(indUWB)=norm(posQuad+relUwbPos(:,indUWB)-posTarget);
    end
    zUWB_log(:,i)=zUWB';
    set(tarPlot,'XData',posTarget(1),'YData',posTarget(2),'ZData',posTarget(3))
    %drawnow ;%limitrate;
    
    %% estimator
%     x=F*x;
%     P=F*P*F'+Q;
%     
%     h_= x(1:3)+(x(7:9))
end

figure(2)
plot(zUWB_log(1,:),'r')
hold on
plot(zUWB_log(2,:),'g')
plot(zUWB_log(3,:),'b')
plot(zUWB_log(4,:),'c')
