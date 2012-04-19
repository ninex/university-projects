function [m, k, x0, v0, dt, aantal_siklusse] = initial();
% initial vra die gebruiker vir die massa [m], die veerkonstante
%[k],
% die beginverplasing [x0], die beginsnelheid [v0], die tyd
%inkrement [dt]
% en die aantal siklusse waaroor die berekeninge gedoen moet word.
% Vir gebruik indien data per aanvraag ingetik moet word
% m=input('Die massa van die sisteem [kg] ? ');
% k=input('Die veerkonstante [N/m] ? ');
% x0=input('Die beginverplasing [m] ?');
% v0=input('Die beginsnelheid [m/s] ?');
% dt=input('Tyd inkrement ? ');
% aantal_siklusse=input('Aantal siklusse waaroor bereken moet word
% ? ');
% Vir gebruik vir herhaaldelike identiese data invoer
m = 1;
k = 0.1;
x0 = 0.01;
v0=0;
dt = 2*pi*sqrt(m/k)/100;
aantal_siklusse = 5;