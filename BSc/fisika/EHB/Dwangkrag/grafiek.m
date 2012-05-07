function [] = grafiek(m, x, v, KE, PE, E, F, t, dt, naam, indeks, eerste_indeks);
% funksie om die verplasing en totale energie as 'n funksie van tyd te plot
whitebg('w');
figure % open 'n nuwe grafiek sonder om voriges te oorskryf
subplot(2,2,1);
plot(t, x(indeks,:), 'b-');
title(['Verplasing :' naam ' benadering']);
xlabel('Tyd [s]');
ylabel('Verplasing [m]');
%text(10,max(x(indeks,:))*0.9, ['Tyd inkrement = ' num2str(dt) 's']);
subplot(2,2,2);
plot(x(indeks,eerste_indeks:size(x,2)),m*v(indeks,eerste_indeks:size(x,2)), 'b-');
title('Fase diagram');
xlabel('Verplasing (m)');
ylabel('Momentum (kg m/s)');
subplot(2,2,3);
plot(x(indeks,eerste_indeks:size(x,2)),F(indeks,eerste_indeks:size(x,2)), 'b-');
title('Fase-ellips van dwangkrag vs verplasing');
xlabel('verplasing [m]');
ylabel('Dwangkrag [N]');
subplot(2,2,4);
plot(t, E(indeks,:), 'b-',t, E(1,:), 'r-');
xlabel('Tyd (s)');
ylabel('Energie [J]');
title(['Totale energie - Gemiddeld = ' num2str(mean(E(indeks,:)),4)
'; %RSD = ' num2str(std(E(indeks,:))/mean(E(1,:))*100,4)]);
xlabel('Tyd (s)');
ylabel('Energie [J]');