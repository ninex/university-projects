function I = simpson(f,a,b,n)
h = (b-a)/n;
x=a+[0:n]*h;
I = (h/3)*((f(x(1))+4*sum(f(x(2:2:n)))+2*sum(f(x(3:2:n-1))))+f(x(n+1)));