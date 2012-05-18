function imOut = setLuminance(imIn, luminance, colorSpace, whitepoint)

if ((nargin < 3) || isempty(colorSpace)),
	colorSpace = 'sRGB';
end;

if ((nargin < 4) || isempty(whitepoint)),
	whitepoint = 'D65';
end;

if (strcmpi(colorSpace, 'sRGB')),
	imIn = sRGB2RGB(imIn);
elseif (~strcmpi(colorSpace, 'RGB')),
	error('Invalid color space');
end;

imXYZ = RGB2XYZ(imIn, whitepoint);
imxyY = XYZ2xyY(imXYZ);
imxyY(:, :, 3) = luminance;
imXYZ = xyY2XYZ(imxyY);
imOut = XYZ2RGB(imXYZ, whitepoint);

if (strcmpi(colorSpace, 'sRGB')),
	imOut = RGB2sRGB(imOut);
end;
