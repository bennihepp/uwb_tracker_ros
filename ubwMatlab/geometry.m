

phi=[-60,60,180,0];
theta=[-120,-120,-120,0]
for i=1:4
x(i)=sind(theta(i))*cosd(phi(i));
y(i)=sind(theta(i))*sind(phi(i));
z(i)=cosd(theta(i));
end


plot3(x(1),y(1),z(1),'or')
hold on
plot3(x(2),y(2),z(2),'or')
plot3(x(3),y(3),z(3),'or')
plot3(x(4),y(4),z(4),'or')
