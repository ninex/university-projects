N=input('hoeveel keer wil jy gooi?');
HH = 0;
HT = 0;
TH = 0;
TT = 0;
for i=1:N
    t1=round(rand);
    t2=round(rand);
    if (t1==1 & t2==1)
       HH = HH + 1;
    end
    if (t1==1 & t2==0)
       HT = HT + 1;
    end
    if (t1==0 & t2==1)
       TH = TH + 1;
    end
    if (t1==0 & t2==0)
       TT = TT + 1;
    end
end
HH
HT
TH
TT