function [carr] = shift(p,initial)
sh = initial;
m = size(p,2);
carr(1) = sh(m);
k=0;
for i=2:m
    if p(i) == 1
        k = k+1;
        val(k) = sh(i);        
    end
end
for l=2:k
    val(1) = bitxor(val(1),val(l));
end
for i=1:(m-1)
    sh(m-(i-1)) = sh(m-i);
end
sh(1) = val(1);
disp(['Shit: ',num2str(sh),' output bits: ',num2str(carr)]);
o = 1;
while strcmp(char(sh),char(initial)) == 0
    o = o + 1;
    carr(o) = sh(m);
    k=0;
    for i=2:m
        if p(i) == 1
            k = k+1;
            val(k) = sh(i);        
        end
    end
    for l=2:k
        val(1) = bitxor(val(1),val(l));
    end
    for i=1:(m-1)
        sh(m-(i-1)) = sh(m-i);
    end
    sh(1) = val(1);
    disp(['Shit: ',num2str(sh),' output bits: ',num2str(carr)]);
end
disp(['Period = ',num2str(size(carr,2))]);