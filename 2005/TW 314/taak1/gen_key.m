function [ret] = gen_key(key)
s = size(key,2)/4;
key = bin2dec(num2str(key));
ret = [key s];