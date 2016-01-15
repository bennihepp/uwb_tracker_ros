close all
clear all
clear all
load('log_data_all2')
addpath(genpath(fullfile(fileparts(fileparts(pwd)), 'tools'))) % standard matlab calibration
phi=[-60,60,180,0];
theta=[-120,-120,-120,0];
o=zeros(3*4,1);
        o(1)=sind(theta(1))*cosd(phi(1));
        o(2)=sind(theta(1))*sind(phi(1));
        o(3)=cosd(theta(1));

        o(4)=sind(theta(2))*cosd(phi(2));
        o(5)=sind(theta(2))*sind(phi(2));
        o(6)=cosd(theta(2));

        o(7)=sind(theta(3))*cosd(phi(3));
        o(8)=sind(theta(3))*sind(phi(3));
        o(9)=cosd(theta(3));

        o(10)=sind(theta(4))*cosd(phi(4));
        o(11)=sind(theta(4))*sind(phi(4));
        o(12)=cosd(theta(4));


x=[1;1;o];

R_WB=eye(3);


for iterations=1:20
    
H=[];
h_v=[];
h_vd=[];
Range_v=[];
    %for i=1:100
    for i=1:length(log_data_seq.Range)
        %for i=1:1000000
        Adress=log_data_seq.Adress(i);
        Range=log_data_seq.Range(i);
        
        rel_pos=(log_data_seq.TrackerPos(:,i)-log_data_seq.TargetPos(:,i));
        pos=rel_pos;
        a=x(1);
        b=x(2);
    
        if ((~isempty(Range))&(Range~=-1)&(Range<1e2))
           % R_WB=RotFromQuatJ(log_data_seq.Tracker_q(:,i))';
            switch Adress
                
                case 0
                    o=R_WB*x(3:5);
                case 1
                    o=R_WB*x(6:8);
                case 2
                    o=R_WB*x(9:11);
                case 3
                    o=R_WB*x(12:14);
                   
            end
            rx=pos(1); ry=pos(2); rz=pos(3);
            oxi=o(1); oyi=o(2); ozi=o(3);
            h_n=norm(pos-o);
            h=a*h_n+b*h_n^2;
          
            if Adress==1
              h_vd=[h_vd,[h;Range]];
            end
            h_to_a=h_n;
            h_to_b=h_n^2;
            %derivatives
            h_to_hn=a+2*b*h_n;
            
            hs_to_x=-2*rx+2*oxi;
            hs_to_y=-2*ry+2*oxi;
            hs_to_z=-2*rz+2*oxi;
            
            hs_to_oxi=-2*rx+2*oxi;
            hs_to_oyi=-2*ry+2*oyi;
            hs_to_ozi=-2*rz+2*ozi;
            
            hn_to_hs=1/(2*sqrt(h_n));
            
            hn_to_r=hn_to_hs*[hs_to_x,hs_to_y,hs_to_z];
            h_to_r=h_to_hn*hn_to_r;
            h_to_x=h_to_r(1);h_to_y=h_to_r(2);h_to_z=h_to_r(3);
            
            hn_to_o=hn_to_hs*[hs_to_oxi,hs_to_oyi,hs_to_ozi];
            h_to_o=h_to_hn*hn_to_o;
            h_to_hoxi=h_to_o(1);h_to_hoyi=h_to_o(2);h_to_hozi=h_to_o(3);
            
            
            H_x=[h_to_a,h_to_b];
            H_o=zeros(1,4*3);
            
            H_o(3*Adress+1:3*Adress+3)=[hs_to_oxi,hs_to_oyi,hs_to_ozi];
           
            
            H=[H;[H_x,H_o]];
            h_v=[h_v;h];
            Range_v=[Range_v;Range];
            
            
        end
        
    end
    
    x=x+(H'*H)\H'*(Range_v-h_v)
    
end
plot(h_vd(1,:),'r')
hold on
plot(h_vd(2,:),'g')
%%validation
