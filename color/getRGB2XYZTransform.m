function [T, Tinv] = getRGB2XYZTransform(whitepoint, colorspace)

if ((nargin < 1) || isempty(whitepoint)),
	whitepoint = 'd65';
end;

if ((nargin < 2) || isempty(colorspace)),
	colorspace = 'srgb';
end;

XYZW = getWhitepoint(whitepoint);
xyRGB = getRGBChromaticities(colorspace);

xdyRGB = xyRGB(1, :) ./ xyRGB(2, :);
zRGB = (1 - xyRGB(1, :) - xyRGB(2, :)) ./ xyRGB(2, :);

YRGB = [xdyRGB; 1, 1, 1; zRGB] \ XYZW';

xyYRGB = [xyRGB; YRGB'];

T(:, 1) = image2Color(xyY2XYZ(color2Image(xyYRGB(:, 1))));
T(:, 2) = image2Color(xyY2XYZ(color2Image(xyYRGB(:, 2))));
T(:, 3) = image2Color(xyY2XYZ(color2Image(xyYRGB(:, 3))));

if (nargout >= 2),
	Tinv = T \ eye(3);
end;