function [ret] = decode(kriptoteks,key)
s = key(2);
key = key(1);

e_vect = bitxor(double(kriptoteks),key);

n = size(e_vect,2);
amount = ceil(n/s^2);
index = 0;

for i = 0:(amount-1)

    %compute a matrix
    for row = 1:s
        for col = 1:s            
            m(row,col) = e_vect(s^2*i + col + s*(row - 1));
        end
    end

    %do operations
    m = m';
    %m = bitxor(m,key);
    op = subkey(diag(m),key); 
    
    m = bitxor(m,op);
    for i=1:s
        m(i,i) = bitxor(m(i,i),op);
    end
    
    %add to decoded vector    
    for row = 1:s
        for col = 1:s
            index = index+1;
            vect(index) = m(row,col);            
        end
    end
end
ret = char(vect);