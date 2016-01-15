figure(4)

subplot(4,1,1)
plot(log_data(1,:),'r')
hold on
plot(log_data(5,:),'g')
grid on
grid minor

subplot(4,1,2)
plot(log_data(2,:),'r')
hold on
plot(log_data(6,:),'g')
grid on
grid minor

subplot(4,1,3)
plot(log_data(3,:),'r')
hold on
plot(log_data(7,:),'g')
grid on
grid minor

subplot(4,1,4)
plot(log_data(4,:),'r')
hold on
plot(log_data(8,:),'g')

grid on
grid minor

%%
figure(5)

subplot(3,1,1)
plot(x_log(1,:),'r')
hold on
%plot(log_data(5,:),'g')
grid on
grid minor

subplot(3,1,2)
plot(x_log(2,:),'r')
hold on
%plot(log_data(6,:),'g')
grid on
grid minor

subplot(3,1,3)
plot(x_log(3,:),'r')
hold on
%plot(log_data(7,:),'g')
grid on
grid minor



grid on
grid minor

figure(6)
plot3(x_log(1,:),x_log(2,:),x_log(3,:),'r')
hold on
plot3(rel_pos_v(1,:),rel_pos_v(2,:),rel_pos_v(3,:),'g')

