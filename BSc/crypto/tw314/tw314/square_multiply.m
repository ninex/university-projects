function square_multiply(a,b,n)
%a = base
%b = exponent
%c = die ander een
b = dec2bin(b);
l = size(b,2) + 1;
z(l) = 1; i = l;
disp('i   b     z      y');
while i ~= 1
y(i) = mod(z(i)^2,n);
if b(l-i+1) == '1'
    z(i-1) = mod(a*y(i),n);
else
    z(i-1) = y(i);
end
disp([num2str(i-1),'   ',num2str(b(l-i+1)),'   ',num2str(z(i)),'   ',num2str(y(i))]);
i = i-1;
end
c = z(1)