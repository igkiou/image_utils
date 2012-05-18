function compensation_function = getCompensationFunction(compensation, white_balance_type)

if (nargin < 2),
	white_balance_type = 'free_strong';
end;

if (strcmp(compensation, 'none')),
	v = load('-ascii','calib.txt'); v = v(:);
	compensation_function = ones(size(v));
elseif (strcmp(compensation, 'sensitivity')),
	v = load('-ascii','calib.txt'); v = v(:);
	compensation_function = v;
elseif (strcmp(compensation, 'white_balance')),
	load('/home/igkiou/MATLAB/translucency/acquisition/white_balance_compensation.mat',...
	sprintf('%s_compensation', white_balance_type));
	eval(sprintf('compensation_function = %s_compensation;', white_balance_type));
end;
