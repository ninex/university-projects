eksak = 0.0973350648636472;
f = inline('1./(exp(x)+1000*x.^2)');
figure;
hold on;
[I n]=quad(f,-1,1,1e-4)
plot(n,I,'*r')
[I n]=quad(f,-1,1,1e-8)
plot(n,I,'*r')
line([0 n],[eksak eksak])

