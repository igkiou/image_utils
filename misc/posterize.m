function [Y levels] = posterize(X, N, bounds)
% POSTERIZE Reduces number of intensity values in an image.
%
%    Y = POSTERIZE(X,N) creates the image Y by reduncing the number
%    of different colors of X to N. The N color values of Y are evenly
%    spaced in the same range as the values of X, however Y has only N 
%    different values. The values of X are rounded to the closet allowed
%    values of Y.
%    
%    Y = POSTERIZE(X,N,[MIN MAX]) scales the N allowed values of Y so 
%    that they have the new [MIN MAX] range.
%
%    Y = POSTERIZE(x) uses N=10 and the min and max intensity value of the
%    original image as MIN and MAX.
%

if (nargin < 2),
	N = 10;
end;

mX = min(X(:));
MX = max(X(:));

step = (MX - mX) / (N - 1);

Y = floor((X - mX) / step) * step + mX;
levels = unique(Y);
if ((nargin > 2) && (~isempty(bounds))),
	Y = imnorm(Y, bounds);
end;
