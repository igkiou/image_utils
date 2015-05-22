function imOut = tonemap_Lab_NakaRushton(imIn, whitepoint, varargin)

if ((nargin < 2) || isempty(whitepoint)),
	whitepoint = 'D65';
end;

imOut = RGB2Lab(imIn, whitepoint);
imOut(:, :, 1) = tonemap_NakaRushton(imOut(:, :, 1), varargin{:});
imOut = Lab2RGB(imOut, whitepoint);
