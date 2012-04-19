function [ret] = m_inv(a,m)
ret = -1;
if gcd(a,m) == 1
    i = 1;
    if a > m
        p(1) = a;
        q(1) = m;
    else
        p(1) = m;
        q(1) = a;
    end
    x(1) = 0;
    y(1) = 1;
    while (q(i) ~= 0)
        r(i) = mod(p(i),q(i));
        s(i) = floor(p(i)/q(i));
        p(i+1) = q(i);
        q(i+1) = r(i);
        y(i+1) = x(i) - (y(i)*s(i));
        x(i+1) = y(i);
        i = i + 1;
    end
    ret = mod(y(i-1),m);
end