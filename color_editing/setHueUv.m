function imOut = setHueUv(imIn, hueuv, colorSpace, whitepoint)

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

imLchuv = RGB2Lchuv(imIn, whitepoint);
imLchuv(:, :, 3) = hueuv;
imOut = Lchuv2RGB(imLchuv, whitepoint);

if (strcmpi(colorSpace, 'sRGB')),
	imOut = RGB2sRGB(imOut);
end;
