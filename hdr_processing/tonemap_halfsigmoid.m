function imOut = tonemap_halfsigmoid(imIn, gainType, gammaCorrection)

if ((nargin < 2) || isempty(gainType)),
	gainType = 'tanh';
end;

if ((nargin < 3) || isempty(gammaCorrection)),
	gammaCorrection = 1;
end;

imOut = gainFunction(imIn, gainType);

if (ischar(gammaCorrection)),
	imOut = feval(gammaCorrection, imOut);
elseif (gammaCorrection ~= 1),
	imOut = imOut .^ gammaCorrection;
end;

end

function Y = gainFunction(X, gainType)

inds = X <= 0.5;
Y = zeros(size(X));
Y(inds) = X(inds);
Y(~inds) = tonemap_sigmoid_gain(X(~inds), gainType);

end
