function [T, Tinv] = getLMS2XYZTransform(colorAppearanceModel)

if ((nargin < 1) || isempty(colorAppearanceModel)),
	colorAppearanceModel = 'bradford';
end;

if (any(strcmpi(colorAppearanceModel, {'hunt', 'rlab', 'vonkries'}))),
	Tinv = [ 0.4002400  0.7076000 -0.0808100;...
			-0.2263000  1.1653200  0.0457000;...
			0.0000000  0.0000000  0.9182200];
	xy = [0.64 0.3 0.15;... 
		0.33 0.6 0.06];
elseif (strcmpi(colorAppearanceModel, 'adobe')),
	xy = [0.64 0.21 0.15;... 
		0.33 0.71 0.06];
elseif (strcmpi(colorAppearanceModel, 'apple')),
	xy = [0.625 0.28 0.155;... 
		0.34 0.595 0.07];
elseif (strcmpi(colorAppearanceModel, 'prophoto')),
	xy = [0.7347 0.1596 0.0366;... 
		0.2653 0.8404 0.0001];
elseif (strcmpi(colorAppearanceModel, 'aces')),
	xy = [0.73470 0 0.00010;...
		0.26530 1 -0.07700];
elseif (strcmpi(colorAppearanceModel, 'beta')),
	xy = [0.6888 0.1986 0.1265;...
		0.3112 0.7551 0.0352];
elseif (strcmpi(colorAppearanceModel, 'cie')),
	xy(:, 1) = smp(image2Color(XYZ2xyY(color2Image(getCMF([], 700.0)))), 1:2);
	xy(:, 2) = smp(image2Color(XYZ2xyY(color2Image(getCMF([], 546.1)))), 1:2);
	xy(:, 3) = smp(image2Color(XYZ2xyY(color2Image(getCMF([], 435.8)))), 1:2);
else
	error('Unknown RGB cam');
end;

if (nargout >= 2),
	Tinv = T \ eye(3);
end;