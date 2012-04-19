function [x,y,v_x,v_y,r, KE, PE, E] = RK4(x0, y0, v_x0, v_y0, t, dt);

x(1)=x0;
y(1)=y0;
v_x(1)=v_x0;
v_y(1)=v_y0;

r(1) = sqrt(x0^2+y0^2);
%faktor(1) = -(4*pi^2 * dt) / r(1)^3;

for i=2:length(t)
    faktor(i-1) = - (4*pi^2 * dt) ;
    
    dx1 = v_x(i-1) * dt;
    dy1 = v_y(i-1) * dt;

    r(i) = sqrt((x(i-1)+0.5*dx1)^2+(y(i-1)+0.5*dy1)^2);
    
    dvx1 = faktor(i-1)/ r(i-1)^3 * x(i-1);
    dvy1 = faktor(i-1)/ r(i-1)^3 * y(i-1);

    dx2 = (v_x(i-1) + dvx1/2) * dt;
    dy2 = (v_y(i-1) + dvy1/2) * dt;

    r(i) = sqrt((x(i-1)+0.5*dx2)^2+(y(i-1)+0.5*dy2)^2);
    
    dvx2 = faktor(i-1)/ r(i-1)^3 * (x(i-1) + dx1/2);
    dvy2 = faktor(i-1)/ r(i-1)^3 * (y(i-1) + dy1/2);

    dx3 = (v_x(i-1) + dvx2/2) * dt;
    dy3 = (v_y(i-1) + dvy2/2) * dt;

    r(i) = sqrt((x(i-1)+0.5*dx3)^2+(y(i-1)+0.5*dy3)^2);
    
    dvx3 = faktor(i-1)/ r(i-1)^3 * (x(i-1) + dx2/2);
    dvy3 = faktor(i-1)/ r(i-1)^3 * (y(i-1) + dy2/2);

    dx4 = (v_x(i-1) + dvx3) * dt;
    dy4 = (v_y(i-1) + dvy3) * dt;

    r(i) = sqrt((x(i-1)+0.5*dx4)^2+(y(i-1)+0.5*dy4)^2);
    
    dvx4 = faktor(i-1)/ r(i-1)^3 * (x(i-1) + dx3);
    dvy4 = faktor(i-1)/ r(i-1)^3 * (y(i-1) + dy3);

    x(i) = x(i-1) + dx1/6 + dx2/3 + dx3/3 + dx4/6;
    y(i) = y(i-1) + dy1/6 + dy2/3 + dy3/3 + dy4/6;

    v_x(i) = v_x(i-1) + dvx1/6 + dvx2/3 + dvx3/3 + dvx4/6;
    v_y(i) = v_y(i-1) + dvy1/6 + dvy2/3 + dvy3/3 + dvy4/6;

    r(i) = sqrt(x(i)^2+y(i)^2);
    
    %subplot(2,2,1);
    %plot(x(i),y(i),'r');
    %drawnow;
end;
[KE, PE, E]=energie(v_x,v_y,r);