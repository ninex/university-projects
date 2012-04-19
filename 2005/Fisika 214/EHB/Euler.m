function [x, v, KE, PE, E] = Euler(m, k, x0, v0, t, dt);
% Bereken die verplasing en snelheid m.b.v. die Euler benadering
x(1)=x0;
v(1)=v0;
faktor = -k/m*dt;
for i=2:length(t),
v(i)=v(i-1) + x(i-1)*faktor; % Euler benadering
x(i)=x(i-1) + v(i-1)*dt;
end;
[KE, PE, E]=energie(m, k, x, v);