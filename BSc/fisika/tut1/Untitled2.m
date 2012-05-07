a=input('A=');
y=input('Lambda=');
w=input('daai w ding=');
r=input('die hoek=');
fplot(inline('-y/2*a*exp(-y/2*t)*cos(w*t+r)-w*a*exp(-y/2*t)*sin(w*t+r)'),[0 100]);