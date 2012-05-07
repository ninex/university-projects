eksak = 1.07466918884810;
f = inline('sqrt(1+x.^5)');
a=0;
b=1;

for i=3:6
    n=2^i;
    T(i-2) = trapes(f,a,b,n);
    h(i-2) = (b-a)/n;
    e(i-2) = abs(T(i-2)-eksak);
    disp(['h = ',num2str(h(i-2)),' T = ',num2str(T(i-2)),' E = ',num2str(e(i-2)),' vir n = ',num2str(n)])
end

for i=4:6
    disp(['En/2 / En = ',num2str(e(i-3)/e(i-2)),' vir n = ',num2str(2^i)]);
end

plot(log10(e(1:4)),log10(h),'r');
hold on;

disp('Simpson');
for i=3:6
    n=2^i;
    T(i-2) = simpson(f,a,b,n);
    h(i-2) = (b-a)/n;
    e(i-2) = abs(T(i-2)-eksak);
    disp(['h = ',num2str(h(i-2)),' T = ',num2str(T(i-2)),' E = ',num2str(e(i-2)),' vir n = ',num2str(n)])
end

for i=4:6
    disp(['En/2 / En = ',num2str(e(i-3)/e(i-2)),' vir n = ',num2str(2^i)]);
end
plot(log10(e(1:4)),log10(h),'b');