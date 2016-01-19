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

figure(1); clf
	plot(Vc)

figure(2); clf; hold on;
	plot(SOC);

	% Overlay indicators when cell is balancing
	for i = 1:PACK_SIZE
		soc_data	= squeeze(SOC.Data(1,i,:));
		bal_data	= squeeze(balance_command.Data(1,i,:));
		plot(SOC.Time(bal_data), soc_data(bal_data), '*k', 'LineWidth', 2 );
	end