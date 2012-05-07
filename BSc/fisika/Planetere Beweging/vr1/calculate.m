function calculate(x0,v_x0,y0,v_y0,dt)
jr=15;
t=0:dt:jr;
[x, y, v_x, v_y, r,KE, PE, E] = Euler_Cromer(x0, y0, v_x0, v_y0, t, dt);

subplot(2,2,1);
plot(x,y,'r');
title(['Euler-Cromer trajek vir',vals(x0, v_y0, jr, dt)]);
xlabel('x-dimensie');
ylabel('y-dimensie');

subplot(2,2,2);
plot(t,E,'r');
title(['Totale energie vir',vals(x0, v_y0, jr, dt)]);
xlabel('Tyd');
ylabel('Totale Energie');

subplot(2,2,3);
plot(t,x.*v_y - y.*v_x,'r');
title(['Hoekmomentum vir',vals(x0, v_y0, jr, dt)]);
xlabel('Tyd');
ylabel('Hoekmomentum');

subplot(2,2,4);
plot(t,(x.*v_y - y.*v_x)./2,'r');
title(['Oppervlakspoed vir',vals(x0, v_y0, jr, dt)]);
xlabel('Tyd');
ylabel('Area spoed');

figure;

subplot(2,2,1);
plot(t,r,'b');
title(['Radiale afstand vir',vals(x0, v_y0, jr, dt)]);
xlabel('Tyd');
ylabel('Radiale afstand');

subplot(2,2,2);
plot(t,PE,'b');
title(['Potensiële energie vir',vals(x0, v_y0, jr, dt)]);
xlabel('Tyd');
ylabel('Potensiële energie');

v = sqrt(v_x.^2 + v_y.^2);
subplot(2,2,3);
plot(t,v,'r');
title(['Baanspoed vir',vals(x0, v_y0, jr, dt)]);
xlabel('Tyd');
ylabel('Baanspoed');

subplot(2,2,4);
plot(t,KE,'r');
title(['Kinetiese energie vir',vals(x0, v_y0, jr, dt)]);
xlabel('Tyd');
ylabel('Kinetiese energie');