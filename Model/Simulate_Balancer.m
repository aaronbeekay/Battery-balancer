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
ax1 = subplot(211);
	% Voltages
	plot(Vc)
	ylim([3.2 4.2]);
	
ax2 = subplot(212);
	% Balancing target
	MinCell			=	Vc;
	BalMask			=	squeeze(balance_command.Data(1,:,:))'.*100 + 1;
	MinCell.Data	=	min(MinCell.Data .* BalMask, [], 2);
	MinCell.Name	=	'Balance target voltage';
	
	plot(MinCell, 'LineWidth', 1.5);
	ylim([3.2 4.2]);
	clear BalMask;
	

% State-of-charge
figure(2); clf; hold on;
	plot(SOC);
	ax3 = gca;

	% Overlay indicators when cell is balancing
	for i = 1:PACK_SIZE
		soc_data	= squeeze(SOC.Data(1,i,:));
		bal_data	= squeeze(balance_command.Data(1,i,:));
		plot(SOC.Time(bal_data), soc_data(bal_data), '*k', 'LineWidth', 2 );
	end
	
	clear bal_data soc_data;
	
linkaxes([ax1 ax2 ax3], 'x');
linkaxes([ax1 ax2], 'y');

% Balance power
Pbal		= Vc;
vbal		= squeeze(balance_command.Data)' .* Vc.Data;
Pbal.Data	= (vbal.^2) ./ Rbalance;
Pbal.Data	= sum(Pbal.Data, 2);

Pbal.Name	= 'Balance power dissipated';

figure(3); clf; hold on
plot(Pbal);