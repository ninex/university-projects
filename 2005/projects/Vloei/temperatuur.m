function [ret] = temperatuur(n,iter)
%linker sy 20C
links = 20;
%regter sy 80C
regs = 80;
%onder sy 0C
onder = 0;
%bo adiabaties
rows = n-1;
cols = n;

%input
%n rooster dimensie
%iter aantal volle iterasies

%==============init==============
for i=1:rows
    for j=1:cols
        if j == 1
            rooster(i,j) = links;
        else
            if j==cols
                rooster(i,j) = regs;
            else
                rooster(i,j) = 0;
            end
        end
    end
end

%===============itereer===========
for k=1:iter
    for i=2:rows-1
        for j=2:cols-1
            if i == 2
                c=j-1;
                rooster(i,j) = 1/3*(rooster(i+1,j)+rooster(i,c)+rooster(i,j+1));
            else
                c=j-1;
                d=i-1;
                rooster(i,j) = 1/4*(rooster(d,j)+rooster(i+1,j)+rooster(i,c)+rooster(i,j+1));
            end
        
        end
    end    
    for j=2:cols-1
        for i=2:rows-1
            if i == 2
                c=j-1;
                rooster(i,j) = 1/3*(rooster(i+1,j)+rooster(i,c)+rooster(i,j+1));
            else
                c=j-1;
                d=i-1;
                rooster(i,j) = 1/4*(rooster(d,j)+rooster(i+1,j)+rooster(i,c)+rooster(i,j+1));
            end
        
        end
    end  
    for j=2:cols-1
        rooster(1,j) = rooster(2,j);
    end
end
middle = round(n/2);
rooster
ret = rooster(middle,middle);