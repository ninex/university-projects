clear
n=12;
for i=1:n-1
    y(i) = factorial(4*(i+1)) - factorial(4*(i+1) - 8);
end
y
i=1:n-1
plot(i,y);