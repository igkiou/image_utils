function xy = getRGBChromaticities(colorspace)

if ((nargin < 1) || isempty(colorspace)),
	colorspace = 'srgb';
end;

colorspace = lower(colorspace);
if (strcmp(colorspace, 'srgb')),
	xy = [0.64 0.3 0.15;... 
		0.33 0.6 0.06];
else
	error('Unknown RGB colorspace');
end;