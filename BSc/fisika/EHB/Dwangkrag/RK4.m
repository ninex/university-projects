function [x, v, KE, PE, E, F, t] = RK4(m, k, c, F0, omega_d, x0, v0,t, dt,aantal_siklusse);
omega_0 = sqrt(k/m);
tau = 2 * pi / omega_0; % die hoekfrekwensie
faktor1 = -(k/m)*dt;
faktor2 = -c/m*dt;
faktor3 = F0/m*dt;
faktor4 = omega_d*dt;

F(1)=F0;
x(1)=x0;
v(1) =v0;
for i=2:length(t),
x1 = v(i-1) * dt;
v1 = faktor1 * x(i-1) + faktor2 * v(i-1) + faktor3 * cos(faktor4 * (i-1));
x2 = (v(i-1) + v1/2) * dt;
v2 = faktor1 * (x(i-1) + x1/2) + faktor2 * (v(i-1) + v1/2) + faktor3 *cos(faktor4 * (i-1/2));
x3 = (v(i-1) + v2/2) * dt;
v3 = faktor1 * (x(i-1) + x2/2) + faktor2 * (v(i-1) + v2/2) + faktor3 *cos(faktor4 * (i-1/2));
x4 = (v(i-1) + v3) * dt;
v4 = faktor1 * (x(i-1) + x3) + faktor2 * (v(i-1) + v3) + faktor3 *cos(faktor4 * i);
x(i) = x(i-1) + x1/6 + x2/3 + x3/3 + x4/6;
v(i) = v(i-1) + v1/6 + v2/3 + v3/3 + v4/6;
end;
[KE, PE, E]=energie(m, k, x, v);
F=F0*(cos(omega_d*t));