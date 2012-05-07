function [x,y,v_x,v_y,r, KE, PE, E] = Euler(x0, y0, v_x0, v_y0, t, dt);
% Bereken die verplasing en snelheid m.b.v. die Euler benadering

x(1)=x0;
y(1)=y0;
v_x(1)=v_x0;
v_y(1)=v_y0;

r(1) = sqrt(x0^2+y0^2);


for i=2:length(t),
    v_x(i)=v_x(i-1) - (4*pi^2 * x(i-1) * dt) / r(i-1)^3;
    v_y(i)=v_y(i-1) - (4*pi^2 * y(i-1) * dt) / r(i-1)^3;
    
    x(i)=x(i-1) + v_x(i-1)*dt;
    y(i)=y(i-1) + v_y(i-1)*dt;
    
    r(i) = sqrt(x(i)^2+y(i)^2);
end;

[KE, PE, E]=energie(v_x,v_y,r);