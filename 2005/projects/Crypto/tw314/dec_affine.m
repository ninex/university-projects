function [ret] = dec_affine(str,a,b)

ret = mZ(mod( (Zm(str) - b)*m_inv(a,26),26));