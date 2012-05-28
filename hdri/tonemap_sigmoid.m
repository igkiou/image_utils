function imOut = tonemap_sigmoid(imIn, gainType, linthresh)

if ((nargin < 2) || isempty(gainType)),
	gainType = 'tanh';
end;

if ((nargin < 3) || isempty(linthresh)),
	linthresh = 0;
end;

imOut = tonemap_sigmoid_thresh_gain(imIn, gainType, linthresh);

end

function Y = tonemap_sigmoid_thresh_gain(X, gainType, linthresh)

inds = X < linthresh;
Y = zeros(size(X));
Y(inds) = X(inds);
Y(~inds) = tonemap_sigmoid_gain(X(~inds), gainType);

end

function Y = tonemap_sigmoid_gain(X, gainType)

if ((nargin < 2) || isempty(gainType)),
	gainType = 'tanh';
end;

if (strcmpi(gainType, 'tanh')),
	Y = (tanh(2 * (X - 0.5)) + 1) / 2;
elseif (strcmpi(gainType, 'erf')),
	Y = (erfnorm(2 * (X - 0.5)) + 1) / 2;
else
	error('Unrecognized gain function type.');
end;

end

function Y = erfnorm(X)

Y = erf(sqrt(pi) * X / 2);

end
