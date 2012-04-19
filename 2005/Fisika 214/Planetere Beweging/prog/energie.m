function [KE, PE, E]=energie(v_x,v_y,r);
GM = 4*pi^2;
v = sqrt(v_x.^2+v_y.^2);
KE = 1/2*v.^2;
PE = -GM ./ r;
E = KE + PE;