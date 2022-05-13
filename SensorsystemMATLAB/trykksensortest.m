%Oppover
faktisk_trykk = 10^3*[0.0 0.0 0.0 0.525 0.525 0.525 1.06 1.06 1.06 1.52 1.52 1.52 2.03 2.03 2.03 2.52 2.52 2.52 3.01 3.01 3.01 3.51 3.51 3.51 4.01 4.00 3.99 4.51 4.52 4.53 4.99 5.01 ];%5.02];
maalt_trykk = 1/100*[98695, 98664, 98664, 151101, 151056, 151002, 204303, 204249, 204249, 251168, 250725, 250652, 301572, 301524, 301334, 350849, 351080, 350984, 400071, 400590, 400578, 450646, 450772, 450626, 501234, 500718, 499523, 551162, 552527, 553469 599745, 601002]-1023;%, 609342]-1023;
antall = 1:length(maalt_trykk);

differanse = maalt_trykk - faktisk_trykk;
gjennomsnitt = mean(differanse);
minst = min(differanse);
storst = max(differanse);
u = storst - minst;

%Nedover
faktisk_trykk_ned = 10^3*[4.51 4.51 4.51 4.01 4.01 4.01 3.51 3.51 3.51 3.01 3.01 3.01 2.5 2.5 2.5 2.0 2.0 2.0 1.49 1.49 1.49 1.0 1.0 1.0 0.5 0.5 0.5 0.0 0.0 0.0];
maalt_trykk_ned = 1/100*[558651, 558567, 558470, 507383, 507725, 507723, 456545, 456852, 456818, 405542, 406009, 406022, 354091, 354475, 354558, 303169, 303561, 303605, 251715, 251915, 251979, 202116, 202198, 202259, 151722, 151853, 152120, 101511, 101565, 101599]-1023;
antall2 = 1:length(maalt_trykk_ned);

differanse2 = maalt_trykk_ned - faktisk_trykk_ned;
gjennomsnitt2 = mean(differanse2);
minst2 = min(differanse2);
storst2 = max(differanse2);
u2 = storst - minst;

figure(1)
plot(antall, faktisk_trykk);
hold on
plot(antall, maalt_trykk)
hold off
legend('Faktisk trykk', 'Målt trykk')
grid on
xlabel('Måling nr')
ylabel('mbar')
title('Økende trykk')
tekst = {"Gjennomsnittlig differanse: " + gjennomsnitt, "Minste differanse: " + storst, "Største differanse: " + minst};
text(20,0,tekst, 'FontSize', 12)

figure(2)
plot(antall2, faktisk_trykk_ned);
hold on
plot(antall2, maalt_trykk_ned)
hold off
legend('Faktisk trykk', 'Målt trykk')
grid on
xlabel('Måling nr')
ylabel('mbar')
title('Synkende trykk')
tekst2 = {"Gjennomsnittlig differanse: " + gjennomsnitt2, "Minste differanse: " + minst2, "Største differanse: " + storst2};
text(5,0,tekst2, 'FontSize', 12)

