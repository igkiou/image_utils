function xy = getRGBChromaticities(colorspace)

if ((nargin < 1) || isempty(colorspace)),
	colorspace = 'srgb';
end;

if (strcmpi(colorspace, 'srgb')),
	xy = [0.64 0.3 0.15;... 
		0.33 0.6 0.06];
elseif (strcmpi(colorspace, 'adobe')),
	xy = [0.64 0.21 0.15;... 
		0.33 0.71 0.06];
elseif (strcmpi(colorspace, 'apple')),
	xy = [0.625 0.28 0.155;... 
		0.34 0.595 0.07];
elseif (strcmpi(colorspace, 'prophoto')),
	xy = [0.7347 0.1596 0.0366;... 
		0.2653 0.8404 0.0001];
elseif (strcmpi(colorspace, 'aces')),
	xy = [0.73470 0 0.00010;...
		0.26530 1 -0.07700];
else
	error('Unknown RGB colorspace');
end;