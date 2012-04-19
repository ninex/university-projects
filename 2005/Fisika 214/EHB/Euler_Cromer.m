function [x, v, KE, PE, E] = Euler_Cromer(m, k, x0, v0, t, dt);
% Bereken die verplasing en snelheid m.b.v. die Euler-Cromer
%benadering
x(1)=x0;
v(1)=v0;
faktor = -k/m*dt;
for i=2:length(t),
v(i)=v(i-1) + x(i-1)*faktor; % Euler-Cromer benadering
x(i)=x(i-1) + v(i)*dt;
end;
[KE, PE, E]=energie(m, k, x, v);