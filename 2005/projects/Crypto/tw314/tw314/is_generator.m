function is_generator(alpha,n)
f = factor(n-1);

for k=1:size(f,2)
    mul = 1;
    for i=k:size(f,2)
        mul=mul*f(i);
        antw = mod(alpha^mul,n);        
        if antw == 1
            disp(['generator ',num2str(alpha),'^',num2str(mul)]);        
        else
            disp(['toets ',num2str(alpha),'^',num2str(mul),' en kry ',num2str(antw)]);
        end
    end
end