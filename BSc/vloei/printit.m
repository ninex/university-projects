x=0;
y=0;
z=0;
clear x,
clear y;
clear z;
iter = 500;
for i=1:10
    disp(['Loop ',int2str(i)]);
    x(i)=i*10;
    y(i)=temperatuur(40,x(i)*10);
    z(i)=temperatuur(x(i),500);
end
hold on;

plot(x,y,'b');
plot(x,z,'r');
legend('verskillende iterasies rooster = 40','verskillende roostergroottes iterasies = 500');

%16*(4 neighbours - 4*hy)