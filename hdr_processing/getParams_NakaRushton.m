function [f m exponent] = getParams_NakaRushton(imIn, mask)

if ((nargin < 2) || isempty(mask)),
	mask = true(size(imIn, 1), size(imIn, 2));
end;

mask = repmat(mask, [1 1 size(imIn, 3)]);
vals = imIn(mask);

Lmax = max(vals);
Lmin = min(vals);
Lw = meanlogv(vals);

k = (Lmax - Lw) / (Lmax - Lmin);
m = 0.3 + 0.7 * k ^ 1.4;
f = 1;
exponent = 1;
