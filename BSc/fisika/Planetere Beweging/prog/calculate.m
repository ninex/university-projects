function calculate(x0,v_x0,y0,v_y0,dt)

%t=0:dt:10;
[x, y, v_x, v_y, r,KE, PE, E, i] = RK4(x0, y0, v_x0, v_y0, dt);
t=0:i-2;
subplot(2,2,1);
plot(x,y,'r');
axis equal;
title('RK4 trajek');

subplot(2,2,2);
plot(t,E,'r');
title('Totale energie');

subplot(2,2,3);
plot(t,x.*v_y - y.*v_x,'r');
title('Hoekmomentum');

subplot(2,2,4);
plot(t,(x.*v_y - y.*v_x)./2,'r');
title('Oppervlakspoed');

