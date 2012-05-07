function harmonic
[m, k, c, F0, omega_d, x0, v0, dt, aantal_siklusse, eerste_indeks] = initial();
%[m, k, c, F0, omega_d, x0, v0, dt, aantal_siklusse, F0] = initial();
% die hoekfrekwensie en periode van die EHB
%omega = sqrt(k/m);
%tau = 2 * pi / omega;
%t=0:dt:tau*aantal_siklusse; % tyd vektor

indeks=1;

[x_a, v_a, KE_a, PE_a, E_a] = analities(m, k, x0, v0, t);

[x_rk4, v_rk4, KE_rk4, PE_rk4, E_rk4,F_rk4,t] = RK4(m, k, c, F0, omega_d,x0, v0, t, dt);

f_rk4 = v_a - v_rk4;

fx_rk4 = x_a - x_rk4;

fPE_rk4 = PE_a - PE_rk4;

fKE_rk4 = KE_a - KE_rk4;

grafiek(m, x_a, v_a, KE_a, PE_a, E_a, F_rk4, t_rk4, dt, 'RK4', 1, eerste_indeks);
%t teen v
figure;
subplot(2,2,1);
plot(t,v_a,'b',t,v_rk4,'r');
title(['Snelheid']);
xlabel('t');
ylabel('v');
legend('Analities','Runge Kutta 4');

%t teen fout
subplot(2,2,2);
plot(t,f_rk4,'r');
title(['Snelheid fout']);
xlabel('t');
ylabel('fout');
legend('Runge Kutta 4');

%t teen x
subplot(2,2,3);
plot(t,x_a,'b',t,x_rk4,'r');
title(['Verplasing']);
xlabel('t');
ylabel('x');
legend('Analities','Runge Kutta 4');

%t teen fx
subplot(2,2,4);
plot(t,fx_rk4,'r');
title(['Verplasing fout']);
xlabel('t');
ylabel('fout');
legend('Runge Kutta 4');

%v teen x
figure;
subplot(2,2,1);
plot(v_a,x_a,'b',v_rk4,x_rk4,'r');
title(['Verplasing teenoor snelheid']);
xlabel('v');
ylabel('x');
legend('Analities','Runge Kutta 4');

%t teen KE
subplot(2,2,2);
plot(t,KE_a,'b',t,KE_rk4,'r');
title(['Kineties energie']);
xlabel('t');
ylabel('Kineties Energie');
legend('Analities','Runge Kutta 4');

%t teen PE
subplot(2,2,3);
plot(t,PE_a,'b',t,PE_rk4,'r');
title(['Potënsiele energie']);
xlabel('t');
ylabel('Potënsiele Energie');
legend('Analities','Runge Kutta 4');

%t teen fKE
figure;
subplot(2,2,1);
plot(t,fKE_rk4,'b');
title(['Kineties energie fout']);
xlabel('t');
ylabel('fout');
legend('Runge Kutta 4');

%t teen fPE
subplot(2,2,2);
plot(t,fPE_rk4,'b');
title(['Potensiële energie fout']);
xlabel('t');
ylabel('fout');
legend('Runge Kutta 4');

%Meganies energie
subplot(2,2,3);
plot(t,PE_a+KE_a,'r',t,PE_rk4+KE_rk4,'b');
title(['Meganiese energie']);
xlabel('t');
ylabel('Totale Enegie');
legend('Analities','Runge Kutta 4');

%Meganies energie fout
subplot(2,2,4);
plot(t,PE_a+KE_a - (PE_rk4+KE_rk4),'r');
title(['Meganiese energie fout']);
xlabel('t');
ylabel('Totale Enegie');
legend('Runge Kutta 4');

figure;
subplot(1,2,1);
plot(x_rk4,F0,'r');
title(['Meganiese energie fout']);
xlabel('t');
ylabel('Totale Enegie');
legend('Runge Kutta 4');

subplot(1,2,2);
plot(t,PE_a+KE_a - (PE_rk4+KE_rk4),'r');
title(['Meganiese energie fout']);
xlabel('t');
ylabel('Totale Enegie');
legend('Runge Kutta 4');