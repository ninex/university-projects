function euler(m)

fact = factor(m);
n = size(fact,2);
k=1;
num=fact(1);
sum=0;
ret=1;
for i=1:n
    if (fact(i) == fact(k))
        sum = sum+1;
    else
        ret = ret * (num^sum - num^(sum-1));
        k = i;
        num = fact(i);
        sum = 1;
    end
end
ret = ret * (num^sum - num^(sum-1));
disp(['Euler value is ',num2str(ret)]);