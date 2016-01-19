% Balancer_model_init.m
% Initialize model parameters for Balancer.slx
% Aaron Bonnell-Kangas, Jan. 2016

%% Pack parameters
PACK_SIZE			=	12;								% number of series cells
PACK_NUM_PARALLEL	=	10;								% number of parallel cells

%% Cell parameters
Capacity			=	2.5 * PACK_NUM_PARALLEL * 3600;	% Cell capacity, coulomb
Ri					=	0.025 / PACK_NUM_PARALLEL;		% Cell internal resistance, ohm
Q0_nominal			=	1.9 * 3600;						% Initial charge, coulomb (baseline)
Q0_variance_max		=	0.5 * 3600;						% Maximum variance of initial charge, coulomb
Q0_variance			=	rand(1, PACK_SIZE) .* Q0_variance_max - Q0_variance_max/2;
Q0 = (ones(1, PACK_SIZE)*Q0_nominal + Q0_variance).*PACK_NUM_PARALLEL;

load('Samsung-25R', 'Voc', 'R1', 'C1');					% Load OCV curve and cell parameters from file

%% Balancer parameters
Rbalance			=	0.69;							% Balance resistor value, ohm

%% Charger parameters
%Ibulk				=	10;								% Bulk charge current setpoint, amp
Ibulk				=	0;								% Disable charging

%% Control parameters
% CHARGE_OFF_SAFETY_MAX_VOLT
%	The controller will forcibly disable bulk charging (disconnect AC
%	power) when any cell reaches this voltage.
CHARGE_OFF_SAFETY_MAX_VOLT	= 4.15;

% CHARGE_ON_MAX_VOLT
%	Maximum cell voltage during bulk charging. The controller will disable
%	the charger (by requesting charge stop) if any cell reaches this 
%	voltage.
CHARGE_ON_MAX_VOLT			= 4.10;

% CHARGE_ON_VOLT_TOL
%	Hysteresis voltage for bulk charge on/off. No cell can be higher than
%	CHARGE_ON_MAX_VOLT - CHARGE_ON_VOLT_TOL in order for bulk charging to
%	be enabled.
% 
%	To avoid oscillation, this parameter should be set greater than 
%	approx. (Ibulk) * Ri, where Ibulk is the DC charge current setpoint and
%	Ri is the typical internal resistance of a series cell at high SOC.
CHARGE_ON_VOLT_TOL			= 0.3;

% BALANCE_MIN_VOLT_THRESH
%	The minimum voltage that a cell should have in order to be eligible for
%	balancing.
BALANCE_MIN_VOLT_THRESH		= 3.0;

% BALANCE_ON_VOLT_TOL
%	Voltage difference threshold required to enable balancing for a cell.
%	If a cell is not currently balancing, it must be more than
%	BALANCE_ON_VOLT_TOL volts above the lowest cell in order to begin being
%	balanced.
BALANCE_ON_VOLT_TOL			= 0.06;

% BALANCE_OFF_VOLT_TOL
%	Voltage difference threshold required to disable balancing for a cell.
%	If a cell is currently balancing, it must be within
%	BALANCE_OFF_VOLT_TOL volts of the minimum cell in order to stop being
%	balanced.
%
%	If this value is negative, a balancing cell will be balanced until it
%	has a lower voltage (under load) than the minimum non-balancing cell.
BALANCE_OFF_VOLT_TOL		= -0.04;

% BALANCE_RELAXATION_TIME
%	document
BALANCE_RELAXATION_TIME		= 5;