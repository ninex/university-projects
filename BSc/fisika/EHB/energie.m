function [KE, PE, E]=energie(m, k, x, v);
KE=1/2*m*v.^2;
PE=1/2*k*x.^2;
E=KE + PE;