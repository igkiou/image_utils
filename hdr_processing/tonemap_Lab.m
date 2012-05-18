function imOut = tonemap_Lab(imIn, value, gammaCorrection)

if ((nargin < 3) || isempty(gammaCorrection)),
	gammaCorrection = 1;
end;

imInLab = RGB2Lab(imIn, 'D65');

imOutLab = imInLab;
imOutLab(:, :, 1) = tonemap_clipping(imInLab(:, :, 1), 'percentile', value);
imOut = Lab2RGB(imOutLab, 'D65');

if (ischar(gammaCorrection)),
	imOut = feval(gammaCorrection, imOut);
elseif (gammaCorrection ~= 1),
	imOut = imOut .^ gammaCorrection;
end;
