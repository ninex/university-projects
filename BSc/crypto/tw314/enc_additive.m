function [ret] = enc_additive(str,a)
 
ret = mZ(mod(Zm(str)+a,26));