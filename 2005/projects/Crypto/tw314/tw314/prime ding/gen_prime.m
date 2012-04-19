function gen_prime(n)
e = log2(n);
%is it even
if mod(e,2) == 0
    k=1;
    for i=1:2*e        
        if mod(i,2) == 1
            i
            t(k) = i;
            k = k+1;
        end
    end    
else
    t(1) = 1;
    k=2
    for i=1:2*e-2        
        if mod(i,2) == 1
            t(k) = 3*i;
            k = k+1;
        end
    end 
end
n-t