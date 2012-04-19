function harmonic
[m, k, x0, v0, dt, aantal_siklusse] = initial();

% die hoekfrekwensie en periode van die EHB
omega = sqrt(k/m); tau = 2 * pi / omega;
t=0:dt:tau*aantal_siklusse; % tyd vektor

[x_a, v_a, KE_a, PE_a, E_a] = analities(m, k, x0, v0, t);

[x_e, v_e, KE_e, PE_e, E_e] = Euler(m, k, x0, v0, t, dt);
[x_ec, v_ec, KE_ec, PE_ec, E_ec] = Euler_Cromer(m, k, x0, v0, t, dt);
[x_rk2, v_rk2, KE_rk2, PE_rk2, E_rk2] = RK2(m, k, x0, v0, t, dt);
[x_rk4, v_rk4, KE_rk4, PE_rk4, E_rk4] = RK4(m, k, x0, v0, t, dt);

f_e = v_a - v_e;
f_ec = v_a - v_ec;
f_rk2 = v_a - v_rk2;
f_rk4 = v_a - v_rk4;

fx_e = x_a - x_e;
fx_ec = x_a - x_ec;
fx_rk2 = x_a - x_rk2;
fx_rk4 = x_a - x_rk4;

fPE_e = PE_a - PE_e;
fPE_ec = PE_a - PE_ec;
fPE_rk2 = PE_a - PE_rk2;
fPE_rk4 = PE_a - PE_rk4;

fKE_e = KE_a - KE_e;
fKE_ec = KE_a - KE_ec;
fKE_rk2 = KE_a - KE_rk2;
fKE_rk4 = KE_a - KE_rk4;

%t teen v
subplot(2,2,1);
plot(t,v_a,'b',t,v_e,'r');
title(['Spoed met waardes m = ',num2str(m),' k = ',num2str(k),' x0 = ',num2str(x0),' v0 = ',num2str(v0),' dt = ',num2str(dt),' siklusse = ',num2str(aantal_siklusse) ]);
xlabel('t');
ylabel('v');
legend('Analities','Euler');
subplot(2,2,2);
plot(t,v_a,'b',t,v_ec,'r');

xlabel('t');
ylabel('v');
legend('Analities','Euler Cromer');
subplot(2,2,3);
plot(t,v_a,'b',t,v_rk2,'r');

xlabel('t');
ylabel('v');
legend('Analities','Runge Kutta 2');
subplot(2,2,4);
plot(t,v_a,'b',t,v_rk4,'r');

xlabel('t');
ylabel('v');
legend('Analities','Runge Kutta 4');

%t teen fout
figure;
subplot(2,2,1);
plot(t,f_e,'r');
title(['Spoed fout met waardes m = ',num2str(m),' k = ',num2str(k),' x0 = ',num2str(x0),' v0 = ',num2str(v0),' dt = ',num2str(dt),' siklusse = ',num2str(aantal_siklusse) ]);
xlabel('t');
ylabel('fout');
legend('Euler');

subplot(2,2,2);
plot(t,f_ec,'r');
xlabel('t');
ylabel('fout');
legend('Euler Cromer');

subplot(2,2,3);
plot(t,f_rk2,'r');
xlabel('t');
ylabel('fout');
legend('Runge Kutta 2');

subplot(2,2,4);
plot(t,f_rk4,'r');
xlabel('t');
ylabel('fout');
legend('Runge Kutta 4');

%t teen x
figure;
subplot(2,2,1);
plot(t,x_a,'b',t,x_e,'r');
title(['Verplasing met waardes m = ',num2str(m),' k = ',num2str(k),' x0 = ',num2str(x0),' v0 = ',num2str(v0),' dt = ',num2str(dt),' siklusse = ',num2str(aantal_siklusse) ]);
xlabel('t');
ylabel('x');
legend('Analities','Euler');

subplot(2,2,2);
plot(t,x_a,'b',t,x_ec,'r');
xlabel('t');
ylabel('x');
legend('Analities','Euler Cromer');

subplot(2,2,3);
plot(t,x_a,'b',t,x_rk2,'r');
xlabel('t');
ylabel('x');
legend('Analities','Runge Kutta 2');

subplot(2,2,4);
plot(t,x_a,'b',t,x_rk4,'r');
xlabel('t');
ylabel('x');
legend('Analities','Runge Kutta 4');

%t teen fx
figure;
subplot(2,2,1);
plot(t,fx_e,'r');
title(['Verplasing fout met waardes m = ',num2str(m),' k = ',num2str(k),' x0 = ',num2str(x0),' v0 = ',num2str(v0),' dt = ',num2str(dt),' siklusse = ',num2str(aantal_siklusse) ]);
xlabel('t');
ylabel('fout');
legend('Euler');

subplot(2,2,2);
plot(t,fx_ec,'r');
xlabel('t');
ylabel('fout');
legend('Euler Cromer');

subplot(2,2,3);
plot(t,fx_rk2,'r');
xlabel('t');
ylabel('fout');
legend('Runge Kutta 2');

subplot(2,2,4);
plot(t,fx_rk4,'r');
xlabel('t');
ylabel('fout');
legend('Runge Kutta 4');

%v teen x
figure;
subplot(2,2,1);
plot(v_a,x_a,'b',v_e,x_e,'r');
title(['Verplasing teenoor spoed met waardes m = ',num2str(m),' k = ',num2str(k),' x0 = ',num2str(x0),' v0 = ',num2str(v0),' dt = ',num2str(dt),' siklusse = ',num2str(aantal_siklusse) ]);
xlabel('v');
ylabel('x');
legend('Analities','Euler');

subplot(2,2,2);
plot(v_a,x_a,'b',v_ec,x_ec,'r');
xlabel('v');
ylabel('x');
legend('Analities','Euler Cromer');

subplot(2,2,3);
plot(v_a,x_a,'b',v_rk2,x_rk2,'r');
xlabel('v');
ylabel('x');
legend('Analities','Runge Kutta 2');

subplot(2,2,4);
plot(v_a,x_a,'b',v_rk4,x_rk4,'r');
xlabel('v');
ylabel('x');
legend('Analities','Runge Kutta 4');

%t teen KE
figure;
subplot(2,2,1);
plot(t,KE_a,'b',t,KE_e,'r');
title(['Kineties energie met waardes m = ',num2str(m),' k = ',num2str(k),' x0 = ',num2str(x0),' v0 = ',num2str(v0),' dt = ',num2str(dt),' siklusse = ',num2str(aantal_siklusse) ]);
xlabel('t');
ylabel('Kineties Energie');
legend('Analities','Euler');

subplot(2,2,2);
plot(t,KE_a,'b',t,KE_ec,'r');
xlabel('t');
ylabel('Kineties Energie');
legend('Analities','Euler Cromer');

subplot(2,2,3);
plot(t,KE_a,'b',t,KE_rk2,'r');
xlabel('t');
ylabel('Kineties Energie');
legend('Analities','Runge Kutta 2');

subplot(2,2,4);
plot(t,KE_a,'b',t,KE_rk4,'r');
xlabel('t');
ylabel('Kineties Energie');
legend('Analities','Runge Kutta 4');

%t teen PE
figure;
subplot(2,2,1);
plot(t,PE_a,'b',t,PE_e,'r');
title(['Potënsiele energie met waardes m = ',num2str(m),' k = ',num2str(k),' x0 = ',num2str(x0),' v0 = ',num2str(v0),' dt = ',num2str(dt),' siklusse = ',num2str(aantal_siklusse) ]);
xlabel('t');
ylabel('Potënsiele Energie');
legend('Analities','Euler');

subplot(2,2,2);
plot(t,PE_a,'b',t,PE_ec,'r');
xlabel('t');
ylabel('Potënsiele Energie');
legend('Analities','Euler Cromer');

subplot(2,2,3);
plot(t,PE_a,'b',t,PE_rk2,'r');
xlabel('t');
ylabel('Potënsiele Energie');
legend('Analities','Runge Kutta 2');

subplot(2,2,4);
plot(t,PE_a,'b',t,PE_rk4,'r');
xlabel('t');
ylabel('Potënsiele Energie');
legend('Analities','Runge Kutta 4');

%t teen fKE
figure;
subplot(2,2,1);
plot(t,fKE_e,'b');
title(['Kineties energie fout met waardes m = ',num2str(m),' k = ',num2str(k),' x0 = ',num2str(x0),' v0 = ',num2str(v0),' dt = ',num2str(dt),' siklusse = ',num2str(aantal_siklusse) ]);
xlabel('t');
ylabel('fout');
legend('Euler');

subplot(2,2,2);
plot(t,fKE_ec,'b');
xlabel('t');
ylabel('fout');
legend('Euler Cromer');

subplot(2,2,3);
plot(t,fKE_rk2,'b');
xlabel('t');
ylabel('fout');
legend('Runge Kutta 2');

subplot(2,2,4);
plot(t,fKE_rk4,'b');
xlabel('t');
ylabel('fout');
legend('Runge Kutta 4');

%t teen fPE
figure;
subplot(2,2,1);
plot(t,fPE_e,'b');
title(['Potensiële energie fout met waardes m = ',num2str(m),' k = ',num2str(k),' x0 = ',num2str(x0),' v0 = ',num2str(v0),' dt = ',num2str(dt),' siklusse = ',num2str(aantal_siklusse) ]);
xlabel('t');
ylabel('fout');
legend('Euler');

subplot(2,2,2);
plot(t,fPE_ec,'b');
xlabel('t');
ylabel('fout');
legend('Euler Cromer');

subplot(2,2,3);
plot(t,fPE_rk2,'b');
xlabel('t');
ylabel('fout');
legend('Runge Kutta 2');

subplot(2,2,4);
plot(t,fPE_rk4,'b');
xlabel('t');
ylabel('fout');
legend('Runge Kutta 4');

figure;
subplot(2,2,1);
plot(t,PE_a+KE_a,'r',t,PE_e+KE_e,'b');
title(['Totale meganiese energie met waardes m = ',num2str(m),' k = ',num2str(k),' x0 = ',num2str(x0),' v0 = ',num2str(v0),' dt = ',num2str(dt),' siklusse = ',num2str(aantal_siklusse) ]);
xlabel('t');
ylabel('Totale meganiese energie');
legend('Analities','Euler');

subplot(2,2,2);
plot(t,PE_a+KE_a,'r',t,PE_ec+KE_ec,'b');
xlabel('t');
ylabel('Totale meganiese energie');
legend('Analities','Euler Cromer');

subplot(2,2,3);
plot(t,PE_a+KE_a,'r',t,PE_rk2+KE_rk2,'b');
xlabel('t');
ylabel('Totale meganiese energie');
legend('Analities','Runge Kutta 2');

subplot(2,2,4);
plot(t,PE_a+KE_a,'r',t,PE_rk4+KE_rk4,'b');
xlabel('t');
ylabel('Totale meganiese energie');
legend('Analities','Runge Kutta 4');