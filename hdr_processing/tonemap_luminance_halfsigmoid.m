function imOut = tonemap_luminance_halfsigmoid(imIn, gainType, gammaCorrection)

if ((nargin < 2) || isempty(gainType)),
	gainType = 'tanh';
end;

if ((nargin < 3) || isempty(gammaCorrection)),
	gammaCorrection = 1;
end;

imInXYZ = RGB2XYZ(imIn, 'D65');
imInxyY = XYZ2xyY(imInXYZ);

imOutxyY = imInxyY;
imOutxyY(:, :, 3) = tonemap_halfsigmoid(imInxyY(:, :, 3), gainType);
imOutXYZ = xyY2XYZ(imOutxyY);
imOut = XYZ2RGB(imOutXYZ, 'D65');

if (ischar(gammaCorrection)),
	imOut = feval(gammaCorrection, imOut);
elseif (gammaCorrection ~= 1),
	imOut = imOut .^ gammaCorrection;
end;
