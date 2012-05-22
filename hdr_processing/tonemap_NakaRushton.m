function imOut = tonemap_NakaRushton(imIn, f, m, exponent, mask)
% Implementation of Naka and Rushton tonemapping operator.

if ((nargin < 2) || isempty(f)),
	f = 1;
end;

if ((nargin < 3) || isempty(m)),
	m = 0.5;
end;

if ((nargin < 4) || isempty(exponent)),
	exponent = 1;
end;

if ((nargin < 5) || isempty(mask)),
	mask = true(size(imIn, 1), size(imIn, 2));
end;

mask = repmat(mask, [1 1 size(imIn, 3)]);
sigma = (f * meanlogv(imIn(mask))) ^ m;
imOut = imIn .^ exponent;
imOut = imOut ./ (sigma ^ exponent + imOut);
