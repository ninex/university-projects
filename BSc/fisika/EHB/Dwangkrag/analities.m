function [x, v, KE, PE, E] = analities(m, k, x0, v0, t);
omega=sqrt(k/m); % hoekfrekwensie
phi=atan2(-v0/omega,x0); % beginfasehoek
Amplitude=sqrt(x0^2+(v0/omega)^2); % Amplitude
x=Amplitude*cos(omega*t+phi); % verplasing vektor
v=-Amplitude*omega*sin(omega*t+phi); % snelheid vektor
[KE, PE, E]=energie(m, k, x, v);