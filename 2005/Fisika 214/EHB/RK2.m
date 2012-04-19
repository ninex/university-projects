function [x, v, KE, PE, E] = RK2(m, k, x0, v0, t, dt);
faktor1 = -k/m*dt; faktor2 = 1+faktor1*dt/2;
x(1)=x0;
v(1) =v0;
for i=2:length(t),
x(i) = x(i-1)*faktor2 + v(i-1)*dt;
v(i) = v(i-1)*faktor2 + x(i-1)*faktor1;
end;
[KE, PE, E]=energie(m, k, x, v);