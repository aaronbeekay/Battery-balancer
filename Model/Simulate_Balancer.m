% Simulate_Balancer.m
% Run the balancer model and plot results
% Aaron Bonnell-Kangas, Jan. 2016

clear all; close all; clc

%% Initialize model
Balancer_model_init

%% Simulate
sim( 'Balancer' );

% Cell voltages
Vc			=	get(logsout, 'Cell voltages');
Vc			=	Vc.Values;


% Cell SOCs
SOC			=	get(logsout, 'SOC1');
SOC			=	SOC.Values;
SOC.Data	=	zeros(1, PACK_SIZE, length(SOC.Time));		% pre-allocate data

for i = 1:PACK_SIZE
	soc_i	= get(logsout, ['SOC' num2str(i)]);
	SOC.Data(1,i,:) = soc_i.Values.Data;
end

% Balance command
balance_command		= get(logsout, 'Balance command');
balance_command		= balance_command.Values;

%% Plot

% Terminal voltages and balancing target
figure(1); clf;
ax1 = gca;
	% Voltages
	plot(Vc)
	ylim([3.5 4.2]);
	
	voltage_imbalance(1) = max(Vc.Data(2,:)) - min(Vc.Data(2,:));
	voltage_imbalance(2) = max(Vc.Data(end,:)) - min(Vc.Data(end,:));
	
	imbalance_stats_string = sprintf( ['Initial voltage imbalance: %.3f V \n '	...
										'Final voltage imbalance: %.3f V' ],	...
										voltage_imbalance(1),					...
										voltage_imbalance(2)					);
									
	annotation( 'textbox', [0.2 0.2 0.1 0.1],		...
				'String', imbalance_stats_string,	...
				'FitBoxToText', 'on'				);
			
	clear voltage_imbalance imbalance_stats_string;
	

% State-of-charge
figure(2); clf; hold on;
	plot(SOC);
	ax2 = gca;

	% Overlay indicators when cell is balancing
	for i = 1:PACK_SIZE
		soc_data	= squeeze(SOC.Data(1,i,:));
		bal_data	= squeeze(balance_command.Data(1,i,:));
		plot(SOC.Time(bal_data), soc_data(bal_data), '*k', 'LineWidth', 2 );
	end
	
	clear bal_data soc_data;
	
linkaxes([ax1 ax2], 'x');

% Balance power
Pbal		= Vc;
vbal		= squeeze(balance_command.Data)' .* Vc.Data;
Pbal.Data	= (vbal.^2) ./ Rbalance;
Pbal.Data	= sum(Pbal.Data, 2);

Pbal.Name	= 'Balance power dissipated';

figure(3); clf; hold on
plot(Pbal);