function imOut = tonemap_sigmoid(imIn, gainType, gammaCorrection)

if ((nargin < 2) || isempty(gainType)),
	gainType = 'tanh';
end;

if ((nargin < 3) || isempty(gammaCorrection)),
	gammaCorrection = 1;
end;

imOut = tonemap_sigmoid_gain(imIn, gainType);

if (ischar(gammaCorrection)),
	imOut = feval(gammaCorrection, imOut);
elseif (gammaCorrection ~= 1),
	imOut = imOut .^ gammaCorrection;
end;

end
