function calculate(x0,v_x0,y0,v_y0,dt)
jr=10;
t=0:dt:10;
[x, y, v_x, v_y, r,KE, PE, E] = RK4(x0, y0, v_x0, v_y0, t, dt);


subplot(2,2,1);
plot(t,E,'r');
title(['Totale energie vir',vals(x0, v_y0, jr, dt)]);
xlabel('Tyd');
ylabel('Totale Energie');

subplot(2,2,2);
plot(t,x.*v_y - y.*v_x,'r');
title(['Hoekmomentum vir',vals(x0, v_y0, jr, dt)]);
xlabel('Tyd');
ylabel('Hoekmomentum');

subplot(2,2,3);
plot(t,PE,'b');
title(['Potensiële energie vir',vals(x0, v_y0, jr, dt)]);
xlabel('Tyd');
ylabel('Potensiële energie');

subplot(2,2,4);
plot(t,KE,'r');
title(['Kinetiese energie vir',vals(x0, v_y0, jr, dt)]);
xlabel('Tyd');
ylabel('Kinetiese energie');