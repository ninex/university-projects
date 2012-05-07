function I = trapes(f,a,b,n)

%  Die funksie I = trapes(f,a,b,n) bereken 'n 
%  benadering tot die integraal van die funksie f(x) 
%  op die interval [a,b], deur 'n saamgestelde 
%  trapesiumreel met n subintervalle te gebruik.
%  Die funksie f moet as "Inline" gedefinieer word. 

h = (b-a)/n;
x = a+[0:n]*h;
I = h*((f(x(1))+f(x(n+1)))/2+sum(f(x(2:n))));

