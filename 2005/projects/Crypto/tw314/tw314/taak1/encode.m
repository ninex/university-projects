function [ret] = encode(teks,key)
s = key(2);
key = key(1);

vect = double(teks);
n = size(vect,2);
amount = ceil(n/s^2);
index = 0;

for i = 0:(amount-1)

    %compute a matrix
    for row = 1:s
        for col = 1:s
            if (s^2*i + col + s*(row - 1)) <= n
                m(row,col) = vect(s^2*i + col + s*(row - 1));
            else
                m(row,col) = round(rand*255);
            end
        end
    end
    
    %do operations
    op = subkey(diag(m),key);
    m = m';
    m = bitxor(m,op);    
    for i=1:s
        m(i,i) = bitxor(m(i,i),op);
    end
    
    %add to encoded vector    
    for row = 1:s
        for col = 1:s
            index = index+1;
            e_vect(index) = m(row,col);            
        end
    end
end
ret = char(bitxor(e_vect,key));
