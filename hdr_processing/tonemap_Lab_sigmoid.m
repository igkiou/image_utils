function imOut = tonemap_Lab_sigmoid(imIn, gainType, gammaCorrection)

if ((nargin < 2) || isempty(gainType)),
	gainType = 'tanh';
end;

if ((nargin < 3) || isempty(gammaCorrection)),
	gammaCorrection = 1;
end;

imInLab = RGB2Lab(imIn, 'D65');

imOutLab = imInLab;
imOutLab(:, :, 1) = tonemap_sigmoid_gain(imInLab(:, :, 1), gainType);
imOut = Lab2RGB(imOutLab, 'D65');

if (ischar(gammaCorrection)),
	imOut = feval(gammaCorrection, imOut);
elseif (gammaCorrection ~= 1),
	imOut = imOut .^ gammaCorrection;
end;
