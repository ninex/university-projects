function [ret] = subkey(di,key)
di = di';
k=1;
bit = zeros(1,8);
n = (size(di,2)*4);
for i=1:n
    if bitget(key,i) == 1
        bit(k) = bitget(di(ceil(i/4)),5-(mod(i-1,4)+1));
        k = k+1;
    end
end
ret = bin2dec(num2str(bit));