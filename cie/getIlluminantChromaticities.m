function xy = getIlluminantChromaticities(illumName)

if ((nargin < 1) || isempty(illumName)),
	illumName = 'd65';
end;

if (ischar(illumName)),
	xy = T2xy(getIlluminantTemperature(illumName));
elseif (isnumeric(illumName)),
	xy = T2xy(illumName);
else
	error('Unknown illuminant');
end;
