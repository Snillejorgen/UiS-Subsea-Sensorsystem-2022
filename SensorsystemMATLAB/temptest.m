% tid_power = Fikset.tid_power;
% Power = Fikset.Powerconsumption;
tid_sensor = Fikset.tid_sensor;
Temp_hovedkort = Fikset.Temp_hoved;
Temp_power = Fikset.Temp_power;
Temp_sensor = Fikset.Temp_sensor;
tid_PC = Fikset.tid_PC;
Temp_PC = Fikset.Temp_PC;


% h = figure
% subplot(2,1,1)
% plot(tid_power, Power, 'Color', '#0072BD')
% xlabel("Tid [minutter]")
% ylabel("Totalt effektforbruk [W]")
% xlim([0,82])
% 
% title("Effektforbruk over tid")

% subplot(2,1,2)
plot(tid_PC, Temp_PC, 'Color', '#7E2F8E', 'LineWidth', 1.5, 'LineStyle', '-');
hold on
temp1 = plot(tid_sensor, Temp_hovedkort, 'Color', '#0072BD', 'LineWidth', 1.5, 'LineStyle', '-');

temp2 = plot(tid_sensor, Temp_power, 'Color', '#77AC30', 'LineWidth', 1.5, 'LineStyle', '-');

plot(tid_sensor, Temp_sensor, 'Color', 	'#D95319', 'LineWidth', 1.5, 'LineStyle', '-');
hold off


ylabel("Temperatur [C]")
xlabel("Tid [minutter]")
xlim([0,82])


title("Temperaturutviklinger over tid")
legend("Temperatur PC", "Temperatur hovedkort", "Temperatur kraftkort", "Temperatur sensorkort", 'Location', 'east')
% mean(Power)