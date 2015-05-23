function T = getIlluminantTemperature(illumName)

if ((nargin < 1) || isempty(illumName)),
	illumName = 'd65';
end;

if (strcmpi(illumName, 'd65')),
	T = 6504;
elseif (strcmpi(illumName, 'd50')),
	T = 5003;
else
	error('Unknown illuminant');
end;
