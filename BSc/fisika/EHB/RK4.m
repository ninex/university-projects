function [x, v, KE, PE, E] = RK4(m, k, x0, v0, t, dt);
faktor = -k/m*dt;
x(1)=x0;
v(1) =v0;
for i=2:length(t),
dx1 = v(i-1) * dt;
dv1 = faktor * x(i-1);
dx2 = (v(i-1) + dv1/2) * dt;
dv2 = faktor * (x(i-1) + dx1/2);
dx3 = (v(i-1) + dv2/2) * dt;
dv3 = faktor * (x(i-1) + dx2/2);
dx4 = (v(i-1) + dv3) * dt;
dv4 = faktor * (x(i-1) + dx3);
x(i) = x(i-1) + dx1/6 + dx2/3 + dx3/3 + dx4/6;
v(i) = v(i-1) + dv1/6 + dv2/3 + dv3/3 + dv4/6;
end;
[KE, PE, E]=energie(m, k, x, v);