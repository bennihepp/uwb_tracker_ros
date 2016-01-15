function [ x_out,P_pos_out,z_est ] = TargetEstimation2( Adress,z,params,R_WB )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
% model

persistent initialized x P
rad=0.13;
if isempty(initialized)
    initialized=1;
    P=20*eye(6);
    P(6,6)=0.01;
    x=[1;1;1;0;0;0];
end

qv=0.005;
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
Q_b=0;
Q=blkdiag(Q_r);
R=eye(1)*1;

pos=x(1:3);
o_offset=2*4;
switch Adress
    
    case 0
        
        o=R_WB*params(o_offset+1:o_offset+3);
        a=params(1);
        b=params(2);
    case 1
        o=R_WB*params(o_offset+4:o_offset+6);
        a=params(3);
        b=params(4);
    case 2
        o=R_WB*params(o_offset+7:o_offset+9);
        a=params(5);
        b=params(6);
    case 3
        o=R_WB*params(o_offset+10:o_offset+12);
        a=params(7);
        b=params(8);
        
end
h_n=norm(pos-o);
h=a*h_n+b*h_n^2;
z_est=h;
%derivatives
h_to_hn=a+2*b*h_n;
hs_to_x=2*(pos'-o'*R_WB');

hn_to_hs=1/(2*sqrt(h_n));



hn_to_x=hn_to_hs*hs_to_x;
h_to_x=h_to_hn*hn_to_x;


% H_ab=zeros(1,4*2);
% H_ab(2*Adress+1:2*Adress+2)=[h_to_a,h_to_b];
% 
% H_o=zeros(1,4*3);
% 
% H_o(3*Adress+1:3*Adress+3)=h_to_o;
H=[h_to_x,0,0,0];

% prediction
x=F*x;
P=F*P*F'+Q;
% update
res=z-h;
S=H*P*H'+R;

mahal=sqrt(res/S*res);
if mahal<2
    K=P*H'/S;
    x=x+K*res;
    P=(eye(6)-K*H)*P;
else
    display('problem')
end
x_out=x;
P_pos_out=P(1:3,1:3);
end

