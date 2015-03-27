function luminance = getLuminanceScotopic(imIn, colorSpace, whitepoint)

if ((nargin < 2) || isempty(colorSpace)),
	colorSpace = 'sRGB';
end;

if ((nargin < 3) || isempty(whitepoint)),
	whitepoint = 'D65';
end;

if (strcmpi(colorSpace, 'sRGB')),
	imIn = sRGB2RGB(imIn);
elseif (~strcmpi(colorSpace, 'RGB')),
	error('Invalid color space');
end;

imXYZ = RGB2XYZ(imIn, whitepoint);

eps = 1e-6;
t = (imXYZ(:,:,2) + imXYZ(:,:,3)) ./ (imXYZ(:,:,1) + eps);
luminance = imXYZ(:,:,2) .* (1.33 * (1.0 + t) -1.68);
