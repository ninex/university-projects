I = 1.07466918884810;
F = inline('0.5*sqrt(1+(1/2*t+1/2).^5)');
[t,w] = gauss(2);
G2 = w*F(t)
abs(I-G2)
[t,w] = gauss(3);
G3 = w*F(t)
abs(I-G3)