% Oppover
faktisk_trykk = 10^3*[0.0 0.0 0.0 0.525 0.525 0.525 1.06 1.06 1.06 1.52 1.52 1.52 ...
2.03 2.03 2.03 2.52 2.52 2.52 3.01 3.01 3.01 3.51 3.51 3.51 4.01 4.00 3.99 4.51 ... 
4.52 4.53 4.99 5.01 ];
maalt_trykk = 1/100*[98695, 98664, 98664, 151101, 151056, 151002, 204303, 204249, ...
204249, 251168, 250725, 250652, 301572, 301524, 301334, 350849, 351080, 350984, ...
400071, 400590, 400578, 450646, 450772, 450626, 501234, 500718, 499523, 551162,...
552527, 553469 599745, 601002]-1023;
antall = 1:length(maalt_trykk);



% Hysterese:
index = [1,4,7,10,13,16,19,22,25,28,31];

differanse = maalt_trykk - faktisk_trykk;
gjennomsnitt = mean(differanse);
minst = min(differanse);
storst = max(differanse);

% Nedover
faktisk_trykk_ned = 10^3*[4.99 5.01, 4.51 4.51 4.51 4.01 4.01 4.01 3.51 3.51 3.51 3.01 3.01 ...
3.01 2.5 2.5 2.5 2.0 2.0 2.0 1.49 1.49 1.49 1.0 1.0 1.0 0.5 0.5 0.5 0.0 0.0 0.0];
maalt_trykk_ned = 1/100*[599745, 601002, 558651, 558567, 558470, 507383, 507725, 507723, 456545, ...
456852, 456818, 405542, 406009, 406022, 354091, 354475, 354558, 303169, 303561,...
303605, 251715, 251915, 251979, 202116, 202198, 202259, 151722, 151853, 152120,...
101511, 101565, 101599]-1023;
antall2 = 1:length(maalt_trykk_ned);

differanse2 = maalt_trykk_ned - faktisk_trykk_ned;
gjennomsnitt2 = mean(differanse2);
minst2 = min(differanse2);
storst2 = max(differanse2);
u2 = storst - minst;
makshysterese = max(maalt_trykk(index) - flip(maalt_trykk_ned(index)));

% plot(antall2, faktisk_trykk_ned, antall2, maalt_trykk_ned)
% legend('Faktisk trykk', 'Målt trykk')
% ylim([-200, 1200])
% title('Synkende trykk')
% xlabel('Måling nr')
% ylabel('Trykk [mbar]')
% grid on


% plot(faktisk_trykk(index), maalt_trykk(index),faktisk_trykk_ned(index),maalt_trykk_ned(index),faktisk_trykk(index) ,faktisk_trykk(index), '--')
% legend('Målt trykk økende', 'Målt trykk synkende', 'Location', 'northwest')
% xlabel('Faktisk trykk')
% ylabel('Målt trykk')
% tekst = {'Maksimal hysterese', num2str(makshysterese)};
% text(3000, 500, tekst, 'fontsize', 14)
% sgtitle('Hysterese')


plot(faktisk_trykk, maalt_trykk,faktisk_trykk_ned,maalt_trykk_ned,faktisk_trykk ,faktisk_trykk, '--')
legend('Målt trykk økende', 'Målt trykk synkende', 'Faktisk trykk' ,'Location', 'northwest')
xlabel('Faktisk trykk')
ylabel('Målt trykk')
ylim([0,5000])
xlim([0,5000])
tekst = {'Maksimal hysterese', num2str(makshysterese)};
text(3000, 500, tekst, 'fontsize', 14)
sgtitle('Hysterese')
grid on



