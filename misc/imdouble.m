function im = imdouble(im, maxVal)

if ((nargin < 2) || isempty(maxVal)),
% 	maxVal = 4095;
	maxVal = 2 ^ 14 - 1;
end;

im = double(im) / maxVal;
