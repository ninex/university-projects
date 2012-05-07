function all_inv(m)

k = 1;

for i=1:m
    inverse = m_inv(i,m);
    if inverse ~= -1
        all(k) = inverse;
        which(k) = i;
        k = k+1;
    end
end
disp(['Following have inverses: ',num2str(which)]);
disp(['Multiplicative inverses: ',num2str(all)]);