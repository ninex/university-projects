function [p] = fluct(teks)
t = ['a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 'u' 'v' 'w' 'x' 'y' 'z'];
n = size(teks,2);
disp(['size = ',num2str(n)]);
freq = zeros(1,26);
for i=1:n
    freq(Zm(teks(i))+1) = freq(Zm(teks(i))+1) + 1;
end
p = freq / sum(freq);

disp(['Freq   Prob']);
space = ones(26,1)*32;
disp([t' space space num2str(freq') space num2str(p')])

disp(['Index of correspondence: ',num2str(sum(p.^2))]);
disp(['Fluctutation index: ',num2str(sum(p.^2)-1/26)]);