function harmonic
[m, k, c, F0, omega_d, x0, v0, dt, aantal_siklusse, eerste_indeks] = initial();
%[m, k, c, F0, omega_d, x0, v0, dt, aantal_siklusse, F0] = initial();
% die hoekfrekwensie en periode van die EHB
omega = sqrt(k/m);
tau = 2 * pi / omega;
t=0:dt:tau*aantal_siklusse; % tyd vektor




[x, v, KE, PE, E, F, t] = RK4(m, k, c, F0, omega_d, x0, v0,t, dt,aantal_siklusse);

grafiek(m, x, v, KE, PE, E, F, t, dt, 'RK4', 1, eerste_indeks);
