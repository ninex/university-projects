function [ret] = enc_affine(str,a,b)

ret = mZ(mod(a*Zm(str)+b,26));