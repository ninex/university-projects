function [x,y,v_x,v_y,r, KE, PE, E] = adaptive_RK4(x0, y0, v_x0, v_y0, t, dt);
clf;
x(1)=x0;
y(1)=y0;
v_x(1)=v_x0;
v_y(1)=v_y0;
hmin=0.001;
hmax=0.01;
% STEP 1
TOL = 1e-10;
H = hmax;
T = 2;i=2;
B = length(t);
r(1) = sqrt(x0^2+y0^2);
Fx = inline('-4*pi^2*x/r^3','x','r');
Fy = inline('-4*pi^2*y/r^3');
ra = 1.4*sqrt(x0^2+y0^2);
W=r(1);
i=2;
% STEP 2
while T < B 
    
    % STEP 3
%     faktor(i-1) = - (4*pi^2 * dt) ;
%     
%     dx1 = v_x(i-1) * dt;
%     dy1 = v_y(i-1) * dt;
% 
%     r(i) = sqrt((x(i-1)+0.5*dx1)^2+(y(i-1)+0.5*dy1)^2);
%     
%     dvx1 = faktor(i-1)/ r(i-1)^3 * x(i-1);
%     dvy1 = faktor(i-1)/ r(i-1)^3 * y(i-1);
% 
%     dx2 = (v_x(i-1) + dvx1/2) * dt;
%     dy2 = (v_y(i-1) + dvy1/2) * dt;
% 
%     r(i) = sqrt((x(i-1)+0.5*dx2)^2+(y(i-1)+0.5*dy2)^2);
%     
%     dvx2 = faktor(i-1)/ r(i-1)^3 * (x(i-1) + dx1/2);
%     dvy2 = faktor(i-1)/ r(i-1)^3 * (y(i-1) + dy1/2);
% 
%     dx3 = (v_x(i-1) + dvx2/2) * dt;
%     dy3 = (v_y(i-1) + dvy2/2) * dt;
% 
%     r(i) = sqrt((x(i-1)+0.5*dx3)^2+(y(i-1)+0.5*dy3)^2);
%     
%     dvx3 = faktor(i-1)/ r(i-1)^3 * (x(i-1) + dx2/2);
%     dvy3 = faktor(i-1)/ r(i-1)^3 * (y(i-1) + dy2/2);
% 
%     dx4 = (v_x(i-1) + dvx3) * dt;
%     dy4 = (v_y(i-1) + dvy3) * dt;
% 
%     r(i) = sqrt((x(i-1)+0.5*dx4)^2+(y(i-1)+0.5*dy4)^2);
%     
%     dvx4 = faktor(i-1)/ r(i-1)^3 * (x(i-1) + dx3);
%     dvy4 = faktor(i-1)/ r(i-1)^3 * (y(i-1) + dy3);
% 
%     x(i) = x(i-1) + dx1/6 + dx2/3 + dx3/3 + dx4/6;
%     y(i) = y(i-1) + dy1/6 + dy2/3 + dy3/3 + dy4/6;
% 
%     v_x(i) = v_x(i-1) + dvx1/6 + dvx2/3 + dvx3/3 + dvx4/6;
%     v_y(i) = v_y(i-1) + dvy1/6 + dvy2/3 + dvy3/3 + dvy4/6;
% 
%     r(i) = sqrt(x(i)^2+y(i)^2);
    

    
    % STEP 4
    W = sqrt(x(i-1)^2+y(i-1)^2);

 Ky1 = H*Fy(T,W);
 Ky2 = H*Fy(T+H/4,W+Ky1/4);
 Ky3 = H*Fy(T+3*H/8,W+(3*Ky1+9*Ky2)/32);
 Ky4 = H*Fy(T+12*H/13,W+(1932*Ky1-7200*Ky2+7296*Ky3)/2197);
 Ky5 = H*Fy(T+H,W+439*Ky1/216-8*Ky2+3680*Ky3/513-845*Ky4/4104);
 Ky6 = H*Fy(T+H/2,W-8*Ky1/27+2*Ky2-3544*Ky3/2565+1859*Ky4/4104-11*Ky5/40);
 
 Kx1 = H*Fx(T,W);
 Kx2 = H*Fx(T+H/4,W+Kx1/4);
 Kx3 = H*Fx(T+3*H/8,W+(3*Kx1+9*Kx2)/32);
 Kx4 = H*Fx(T+12*H/13,W+(1932*Kx1-7200*Kx2+7296*Kx3)/2197);
 Kx5 = H*Fx(T+H,W+439*Kx1/216-8*Kx2+3680*Kx3/513-845*Kx4/4104);
 Kx6 = H*Fx(T+H/2,W-8*Kx1/27+2*Kx2-3544*Kx3/2565+1859*Kx4/4104-11*Kx5/40);
 
 
 v_y(i)=v_y(i-1)-Ky6;
 v_x(i)=v_x(i-1)-Kx6;
 
 x(i)=x(i-1)+v_x(i)*H;
 y(i)=y(i-1)+v_y(i)*H;

 
     %subplot(2,2,1);
     %clf;
     hold on;
%      plot(-ra:0.1:ra,0,'k');
%      plot(0,-ra:0.1:ra,'k');
text(0,0,'*');
     plot(x(i),y(i),'b');
     %plot(v_x(i),v_y(i),'r');
     hold off;
     %axis([-20, 10, -20 20])
     drawnow;
 i=i+1;
% STEP 4
 R = abs(Ky1/360-128*Ky3/4275-2197*Ky4/75240.0+Ky5/50+2*Ky6/55)/H;

    
    % STEP 5
    if R <= TOL 
        % STEP 6
        %Approximation accepted
        T = T+H;
        W = sqrt(x(i-1)^2+y(i-1)^2);
        %W = W+25*Ky1/216+1408*Ky3/2565+2197*Ky4/4104-Ky5/5;
        % STEP 7
        %fprintf(OUP, '%12.7f %11.7f %11.7f %11.7f\n', T, W, H, R);
    end;
    % STEP 8
    % To avoid underflow
    if R > 1.0E-20 
        DELTA = 0.84 * exp(0.25 * log(TOL / R));
    else
        DELTA = 10.0;
    end;
    % STEP 9
    % Calculate new H
    if DELTA <= 0.1 
        H = 0.1*H;
    else
        if DELTA >= 4 
            H = 4.0 * H;
        else
            H = DELTA * H;
        end;
    end;
    % STEP 10
    if H > hmax
        H = hmax;
    end;
    % STEP 11
    if H < hmin 
        h = hmin;
        %disp('May be error');
    else
        if T+H > B 
            if abs(B-T) < TOL 
                T = B;
            else
                H = B-T;
            end;
        end;
    end;
 end;
 [KE, PE, E]=energie(v_x,v_y,r);