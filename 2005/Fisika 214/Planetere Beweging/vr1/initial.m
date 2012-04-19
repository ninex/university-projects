function [x,v_x,y,v_y,dt] = initial()

%x = input('Initial x position = ');
%y = input('Initial y position = ');
%v_x = input('Initial x velocity = ');
%v_y = input('Initial y velocity = ');
%dt = input('Time step = ');
x=1;
y=0;
v_x=0;
v_y=2*pi;
dt=0.1;

aspect = 1.33;
r = 1.3*sqrt(x^2+y^2);

figure;
subplot(2,2,1);
%axes = [-r,r,-r/aspect,r/aspect];
axis equal
hold all;
text(0,0,'*');
%plot(-r:0.1:r,0,'k');
%plot(0,-r:0.1:r,'k');
%line([-r r],[0 0],'k');
%line([0 0],[-r r]);


%subplot(2,2,2);
%subplot(2,2,3);
%subplot(2,2,4);