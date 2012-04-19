function [ret] = enc_permutation(str,a)
 
n = size(str,2);
for i=1:n
    ret(i)=a(Zm(str(i))+1);
end