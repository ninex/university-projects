function I = midpoint(f,a,b,n)
h = (b-a)/n;
x = a+([0:n-1]+1/2)*h;
I = h*sum(f(x));