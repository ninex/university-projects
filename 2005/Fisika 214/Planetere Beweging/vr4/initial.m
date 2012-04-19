function [x,v_x,y,v_y,dt] = initial()

rmin = 0.59;
GM = 4*pi^2;
P=76;
a = ((P*sqrt(GM)/(2*pi)))^(2/3);
x=rmin;
y=0;
v_x=0;
v_y=sqrt(GM*(2/rmin - 1/a));
dt=0.001;


r = 1.4*sqrt(x^2+y^2);

figure('doublebuffer','on');
subplot(2,2,1);
axis([-30, 10, -30 30])
hold all;
text(0,0,'*');