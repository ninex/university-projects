function calculate(x0,v_x0,y0,v_y0,dt)

t=0:dt:10;
[x, y, v_x, v_y, r,KE, PE, E] = Euler(x0, y0, v_x0, v_y0, t, dt);

subplot(2,2,1);
plot(x,y,'r');

subplot(2,2,2);
plot(t,E,'r');

subplot(2,2,3);
plot(t,x.*v_y - y.*v_x,'r');

subplot(2,2,4);
plot(t,(x.*v_y - y.*v_x)./2,'r');

figure;

subplot(2,2,1);
plot(t,r,'b');

subplot(2,2,2);
plot(t,PE,'b');

v = sqrt(v_x.^2 + v_y.^2);
subplot(2,2,3);
plot(t,v,'r');

subplot(2,2,4);
plot(t,KE,'r');
