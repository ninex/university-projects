function [m, k, c, F0, omega_d, x0, v0, dt, aantal_siklusse, eerste_indeks] = initial();
% initial vra die gebruiker vir die massa [m], die veerkonstante [k],
% die beginverplasing [x0], die beginsnelheid [v0], die tyd inkrement [dt]
% en die aantal siklusse waaroor die berekeninge gedoen moet word.
% Vir gebruik vir herhaaldelike identiese data invoer
m = 1;
k = 0.01;
c = 0.01;
F0 = 10.1;
omega_d = sqrt(k/m)*1.1;
x0 = 0.0;
v0=0;
dt = 2*pi*sqrt(m/k)/100;
aantal_siklusse = 50;
aantal_tydkonstantes=4; % aantal tydkonstantes om oorgangsverskynsel te laat uitsterf;
wagtyd=2*m/c*aantal_tydkonstantes;
eerste_indeks=round(wagtyd/dt)+1;