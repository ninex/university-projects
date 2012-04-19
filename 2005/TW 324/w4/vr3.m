n=41;
[t,w] = gauss(n);
G = w*f(t);
disp([n G abs(G-eksak)])
plot(n,G,'+b');

n=177;
[t,w] = gauss(n);
G = w*f(t);
disp([n G abs(G-eksak)])
plot(n,G,'+b');
