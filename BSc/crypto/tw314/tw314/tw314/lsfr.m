function [p] = lsfr(k,t,m)
s = bitxor(k,t)
need = 2*m;
for i =1:m
    for j = 1:m
        ma(i,j) = s(m + (i-1) - (j-1));        
    end
    left(i,1) = s(m+i);
end
modinv = mod(det(ma),2)
mod(inv(ma),2)
p = mod(mod(inv(ma),2)*left,2);
format rat;
p
format long;