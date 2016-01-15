close all
clear all
clear all
load('log_data_all3')

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


for iterations=1:10
    
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
            switch Adress
                
                case 0
                    
                    o=R_WB*x(o_offset+1:o_offset+3);
                    a=x(1);
                    b=x(2);
                    c=x(3);
                case 1
                    o=R_WB*x(o_offset+4:o_offset+6);
                    a=x(4);
                    b=x(5);
                    c=x(6);
                case 2
                    o=R_WB*x(o_offset+7:o_offset+9);
                    a=x(7);
                    b=x(8);
                    c=x(9);
                case 3
                    o=R_WB*x(o_offset+10:o_offset+12);
                    a=x(10);
                    b=x(11);
                    c=x(12);
                   
            end

            h_n=norm(pos-o);
            h=c+a*h_n+b*h_n^2;
          
            if Adress==1
              h_vd=[h_vd,[h;Range]];
            end
            h_to_a=h_n;
            h_to_b=h_n^2;
            h_to_c=1;
            %derivatives
            h_to_hn=a+2*b*h_n;
            hs_to_oi=2*(-pos'*R_WB'+o');
            
            hn_to_hs=1/(2*sqrt(h_n));
            

            
            hn_to_o=hn_to_hs*hs_to_oi;
            h_to_o=h_to_hn*hn_to_o;

            
            H_ab=zeros(1,4*3);
            H_ab(3*Adress+1:3*Adress+3)=[h_to_a,h_to_b,h_to_c];

            H_o=zeros(1,4*3);
            
            H_o(3*Adress+1:3*Adress+3)=h_to_o;
           
            
            H=[H;[H_ab,H_o]];
            h_v=[h_v;h];
            Range_v=[Range_v;Range];
            
            
        end
        
    end
    
    x=x+(H'*H)\H'*(Range_v-h_v)
    
end

%% save params:
save('params','x')

figure(1)
plot(h_vd(1,:),'r')
hold on
plot(h_vd(2,:),'g')

figure(2)
plot3(x(9),x(10),x(11),'ro')
hold on
plot3(x(12),x(13),x(14),'go')
plot3(x(15),x(16),x(17),'bo')
plot3(x(18),x(19),x(20),'co')

plot3(o_init(1),o_init(2),o_init(3),'rx')
plot3(o_init(4),o_init(5),o_init(6),'gx')
plot3(o_init(7),o_init(8),o_init(9),'bx')
plot3(o_init(10),o_init(11),o_init(12),'cx')


box on
grid on
grid minor
axis(2*[-1,1,-1,1,-1,1])
%%validation
