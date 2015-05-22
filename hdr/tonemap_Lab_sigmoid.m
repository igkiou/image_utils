function imOut = tonemap_Lab_sigmoid(imIn, whitepoint, varargin)

if ((nargin < 2) || isempty(whitepoint)),
	whitepoint = 'D65';
end;

imOut = RGB2Lab(imIn, whitepoint);
imOut(:, :, 1) = tonemap_sigmoid_gain(imOut(:, :, 1), varargin{:});
imOut = Lab2RGB(imOut, whitepoint);
