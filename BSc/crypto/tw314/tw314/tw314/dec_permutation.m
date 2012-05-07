function [ret] = dec_permutation(str,a)
 
n = size(str,2);

for i=1:26
    b(Zm(a(i))+1) = mZ(i-1);
end
for i=1:n
    ret(i)=b(Zm(str(i))+1);
end