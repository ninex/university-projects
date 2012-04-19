function tempOfplate()
%This function calculates the temperature of the plate and 
%the total energy flow. The function will display the temperature
% at the middelpoint when the accuracy was reached and the
% minimum_iterations was reached. It will display an graph to
% show the temperture for the whole plate and a graph to show how the
% temperature of the midpoint changed with each iteration.It will also
% display the final accuracy. It will also display the energy flow for each
% side and the total energie flow when finished.\

size = 40; %specifies the size of the grid,(square plate)
accuracy = 10^-6;   %specifies the accuracy at which to stop
minimum_iterations = 10000; %The minimum iterations to run even after accuracy was reached

plate = zeros(size);
%set the begin values for each side of the plate
plate(:,1) = 20; %lefthand side
plate(:,size) = 80; %righthand side
plate(1,:) = 0;     %the bottom
plate(size,:) = 0;  %the top

is_adiabatic = 1;   %tells the program if a side is adiabatic
%make a matrix to keep track of which side is adiabatic
adiabatic_points = ones(size);
adiabatic_points(size,:) = -1; %marks the top as adiabatic

%Nou work out the points
iterations = 0;
accuracy_of_midpoint = 1;
endpoint = size - 1;
middle_temp = 0;
midpoint_pos = round(size/2);
hand = figure(1);
set(hand,'DoubleBuffer','on');
Energy = 0;

while ((accuracy_of_midpoint > accuracy) | (iterations < minimum_iterations) | (plate(midpoint_pos,midpoint_pos) < 1) )
    %calculate one iteration and update the plate
    plate = UpdateGrid(plate,adiabatic_points,endpoint);
        
    %save the old midpoint's temperatures and add the new temp
    middle_temp = [middle_temp plate(midpoint_pos,midpoint_pos)];
    iterations = iterations + 1;
    %get the accuracy of the new midpoint temp versus the old
    accuracy_of_midpoint  = abs(middle_temp(iterations) - middle_temp(iterations+1));
    
    if (is_adiabatic == 1)
    %update the adiabatic points
    if (adiabatic_points(1,2) < 0) %Updates the top
       plate(1,:) = plate(2,:); 
    end
    if (adiabatic_points(2,1) < 0) %updates the left
       plate(:,1) = plate(:,2);
    end
    if (adiabatic_points(size,2) < 0) %Updates the bottom
       plate(size,:) = plate(size-1,:); 
    end
   
    if (adiabatic_points(2,size) < 0) %Updates the right
       plate(:,size) = plate(:,size-1); 
    end
    end
    
    %calculate the energieflow for each side
    [Etotal,Eleft,Eright,Etop,Ebottom] = EnergyFlow(plate,size,1);
    Energy = [Energy Etotal];
end



subplot(2,2,1);
%draw the temperature graph for the plate
surf(0:1/(size-1):1,0:1/(size-1):1,plate,'EdgeColor','none'); 
drawnow;
shading interp;
title1 = ['The plate''s temperature after '  num2str(iterations) ' iterations in 3d.']; 
title(title1);
zlabel('Temperature');

subplot(2,2,2);
surface(0:1/(size-1):1,0:1/(size-1):1,plate,'EdgeColor','none'); 
drawnow;
shading interp;
title1 = ['The plate''s temperature after '  num2str(iterations) ' iterations from the top.']; 
title(title1);
zlabel('Temperature');


middle_temp = middle_temp(2:iterations+1);
Energy = Energy(2:iterations+1);
divide = 1 - 2/size;

%Get DEL T and DEL^2 T 
delTi = (plate(size-1,midpoint_pos) - plate(2,midpoint_pos))/divide;
delTj = (plate(midpoint_pos,size-1) - plate(midpoint_pos,2))/divide;
delsquareT = 16*(plate(midpoint_pos,midpoint_pos-1) + plate(midpoint_pos,midpoint_pos+1) + ...
        plate(midpoint_pos-1,midpoint_pos) + plate(midpoint_pos+1,midpoint_pos) - ...
        4*plate(midpoint_pos,midpoint_pos));  

%Print out the results
['Accuracy of midpoint = ' num2str(accuracy_of_midpoint)]
['Amount of iterations done = ' num2str(iterations)]
['Temperature of midpoint = ' num2str(plate(midpoint_pos,midpoint_pos))]
['DelT i direction = ' num2str(delTi)]
['DelT j direction = ' num2str(delTj)]
['Del^2T = ' num2str(delsquareT)] 
['Energie flow out left = ' num2str(Eleft)]
['Energy flow out right = ' num2str(Eright)]
['Energy flow out top = ' num2str(Etop)]
['Energy flow out bottom = ' num2str(Ebottom)]
['Total energy flow out plate = ' num2str(Etotal)]
[''];

  
%Display a graph of how the midpoints's temperature changed 
subplot(2,2,3);
plot(1:iterations,middle_temp);
xlabel('Iterations');
ylabel('Midpoint''s temperature');

%Display a graph to show how the total energy flow out changed 
subplot(2,2,4);
plot(1:iterations,Energy);
xlabel('Iterations');
ylabel('Total energy flow out of plate');


end

%This function does one iteration.It checks whether which sides are
%adiabatic and leaves the out of calculation.
function [plate] = UpdateGrid(plate,adiabatic,n)
    
    for (y = n:-1:2)    %run through the grid y
        for (x = 2:n)   %run through the grid x
           divide = 0;  %what to devide through.Will be 3 if adiabatic  else 4
           plate(y,x) = 0;
           if (adiabatic(y-1,x) > 0) %checks the bottom of plate
               plate(y,x) = plate(y,x) + plate(y-1,x);
               divide = divide + 1; %adds one to devide because not adiabatic
           end
           if (adiabatic(y+1,x) > 0) %Checks the top of plate
               plate(y,x) = plate(y,x) + plate(y+1,x);
               divide = divide + 1;%adds one to devide because not adiabatic
           end
           if (adiabatic(y,x-1) > 0) %checks the lefthand side
               plate(y,x) = plate(y,x) + plate(y,x-1);
               divide = divide + 1;%adds one to devide because not adiabatic
           end
           if (adiabatic(y,x+1) > 0) %checks the righthand side
               plate(y,x) = plate(y,x) + plate(y,x+1);
               divide = divide + 1;%adds one to devide because not adiabatic
           end
           plate(y,x) = plate(y,x) / divide;    %get the value of the point
        end
    end
end

%This function calculates the energy flow over the plate
function [Etotal,Eleft,Eright,Etop,Ebottom] = EnergyFlow(plate,size,k)

    Eleft = sum(plate(3:(size-2),2)) - sum(plate(3:(size-2),1)) ...
             + (plate(2,2) - plate(2,1))*1.5 + ...
             (plate(size-1,2) - plate(size-1,1))*1.5;
    Eleft = k * Eleft;    %k *delTleft
    
    Eright = sum(plate(3:(size-2),size-1)) - sum(plate(3:(size-2),size)) ...
             + (plate(2,size-1) - plate(2,size))*1.5 + ...
             (plate(size-1,size-1) - plate(size-1,size))*1.5;
    Eright = k * Eright;%k *delTright
    
    Ebottom = sum(plate(2,3:(size-2))) - sum(plate(1,3:(size-2))) ...
             + (plate(2,2) - plate(1,2))*1.5 + ...
             (plate(2,size-1) - plate(1,size-1))*1.5;
    Ebottom = k * Ebottom;%k * delTbottom
    
    Etop = sum(plate(size-1,3:(size-2))) - sum(plate(size,3:(size-2))) ...
             + (plate(size-1,2) - plate(size,2))*1.5 + ...
             (plate(size-1,size-1) - plate(size,size-1))*1.5;
    Etop = k * Etop;%k * delTtop
    Etotal = Eleft + Eright + Etop + Ebottom;  % total energyflow
end

