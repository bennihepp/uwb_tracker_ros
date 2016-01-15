function [ x_out,P_pos_out,h ] = TargetEstimation( Adress,z )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
% model

persistent initialized x P

if isempty(initialized)
   initialized=1;
   P=20*eye(6);
   x=[1;1;1;0;0;0];
end
radius=0.13;
qv=0.009;
qv=0.00009;
dt=1/50;
F_r=[1,0,0,dt,0,0;
    0,1,0,0,dt,0;
    0,0,1,0,0,dt;
    0,0,0,1,0,0;
    0,0,0,0,1,0;
    0,0,0,0,0,1];

F_b=eye(1);






F=blkdiag(F_r);
Q_r=diag([0,0,0,qv,qv,qv]);
Q=blkdiag(Q_r);
R=eye(1)*1;



phi=[-60,60,180,0];
theta=[-120,-120,-120,0];
o=[0;0;0];
pos=x(1:3);
switch Adress
    
    case 0
        o(1)=radius*sind(theta(1))*cosd(phi(1));
        o(2)=radius*sind(theta(1))*sind(phi(1));
        o(3)=radius*cosd(theta(1));
        h=norm(pos-o);

    case 1
        o(1)=radius*sind(theta(2))*cosd(phi(2));
        o(2)=radius*sind(theta(2))*sind(phi(2));
        o(3)=radius*cosd(theta(2));
        h=norm(pos-o);


    case 2
        
        o(1)=radius*sind(theta(3))*cosd(phi(3));
        o(2)=radius*sind(theta(3))*sind(phi(3));
        o(3)=radius*cosd(theta(3));
        h=norm(pos-o);


    case 3
        o(1)=radius*sind(theta(4))*cosd(phi(4));
        o(2)=radius*sind(theta(4))*sind(phi(4));
        o(3)=radius*cosd(theta(4));
        h=norm(pos-o);

    otherwise
        h=[-1;-1;-1];
end

        H_norm_1=1/(2*h);
        H_norm_2=[2*x(1)-2*o(1),2*x(2)-2*o(2),2*x(3)-2*o(3),0,0,0];
        
        H=H_norm_1*H_norm_2;
% prediction
    x=F*x;
    P=F*P*F'+Q;
% update
    res=z-h;
    S=H*P*H'+R;
    
    mahal=sqrt(res/S*res);
    if mahal<0.8
    K=P*H'/S;
    x=x+K*res;
    P=(eye(6)-K*H)*P;

    end
x_out=x;
P_pos_out=P(1:3,1:3);
end

