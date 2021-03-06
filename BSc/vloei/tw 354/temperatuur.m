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

rooster(1:n,1)=links;
rooster(1,1:n)=0;
rooster(n,1:n)=onder;
rooster(1:n,n)=regs;

%===============itereer===========
for k=1:iter
    for i=2:rows
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
        for i=2:rows
            if (i == 2) 
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
    z(k) = rooster(round(n/2),round(n/2));
end
middle = round(n/2);
plot(1:iter,z);
ret = rooster(middle,middle);


div = (2/(n-1)); 
deli = (rooster(middle+1,middle) - rooster(middle-1,middle))/div;
dely = (rooster(middle,middle+1) - rooster(middle,middle-1))/div;
del2 = 16*( rooster(middle,middle-1)+ rooster(middle,middle+1) ...
    + rooster(middle-1,middle)+ rooster(middle+1,middle)...
    -4*rooster(middle,middle));
disp(['Temperatuur = ',num2str(ret,'%2.30g')]);
disp(['Eerste afgeleide = ',num2str(deli),'i + ',num2str(dely),'j']);
disp(['Tweede afgeleide = ',num2str(del2,'%1.20f')]);

k = 0.39;

Eleft = sum(rooster(3:(n-2),2)) - sum(rooster(3:(n-2),1)) ...
             + (rooster(2,2) - rooster(2,1))*1.5 + ...
             (rooster(n-1,2) - rooster(n-1,1))*1.5;
Eleft = k * Eleft;    
    
Eright = sum(rooster(3:(n-2),n-1)) - sum(rooster(3:(n-2),n)) ...
             + (rooster(2,n-1) - rooster(2,n))*1.5 + ...
             (rooster(n-1,n-1) - rooster(n-1,n))*1.5;
Eright = k * Eright;
    
Etop = sum(rooster(2,3:(n-2))) - sum(rooster(1,3:(n-2))) ...
             + (rooster(2,2) - rooster(1,2))*1.5 + ...
             (rooster(2,n-1) - rooster(1,n-1))*1.5;
Etop = k * Etop;
    
Ebottom = sum(rooster(n-1,3:(n-2))) - sum(rooster(n,3:(n-2))) ...
             + (rooster(n-1,2) - rooster(n,2))*1.5 + ...
             (rooster(n-1,n-1) - rooster(n,n-1))*1.5;
Ebottom = k * Ebottom;
Etotal = Eleft + Eright + Etop + Ebottom;  % total energyflow

disp(['Energievloei links: ', num2str(Eleft)]);
disp(['Energievloei regs: ', num2str(Eright)]);
disp(['Energievloei bo: ', num2str(Etop)]);
disp(['Energievloei onder: ', num2str(Ebottom)]);
disp(['Totale energievloei: ', num2str(Etotal)]);
