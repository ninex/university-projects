function pairs(m,alpha,beta,n)
t = mod(alpha^m,n);
for i=0:m-1
    L1(i+1) = mod(t^i,n);
    L2(i+1) = mod(beta*alpha^(-i),n);
end
disp('   L1     L2');
disp([L1' L2']);
L1;
L2;