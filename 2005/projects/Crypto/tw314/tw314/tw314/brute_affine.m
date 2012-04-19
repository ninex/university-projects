function brute_affine(str)

for i=1:25
    if m_inv(i,26) ~= -1
        for j=0:25
            disp(['s=',num2str([i j]),' skoonteks = ',dec_affine(str,i,j)]);
        end
    end
end