function projek2(t,p,path,n)

ps = p;
pc = 1 - ps;

captured = 0;
transmitted = 0;
reflected = 0;
for i=1:n
    busy = true;
    while busy
        neutron = rand;
        if neutron >= pc
            captured = captured +1;
            busy = false;
        else if neutron <= ps
                ct = 1 - 2*rand;
                l = -path*log(rand);
                z = l*ct;
                if z < 0
                    reflected = reflected +1;
                    busy = false;
                end
                if z > t
                    transmitted = transmitted +1;
                    busy = false;
                end
            end
        end
    end
end
disp(['transmitted = ' num2str(transmitted)]);
disp(['captured = ' num2str(captured)]);
disp(['reflected = ' num2str(reflected)]);

pie([transmitted captured reflected]);
title(['path = ',num2str(path)]);
%legend('transmitted','captured','reflected');