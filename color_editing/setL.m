function imOut = setL(imIn, L, colorSpace, whitepoint)

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

imLch = RGB2Lch(imIn, whitepoint);
imLch(:, :, 1) = L;
imOut = Lch2RGB(imLch, whitepoint);

if (strcmpi(colorSpace, 'sRGB')),
	imOut = RGB2sRGB(imOut);
end;
