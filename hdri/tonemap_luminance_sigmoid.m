function imOut = tonemap_luminance_sigmoid(imIn, whitepoint, varargin)

if ((nargin < 2) || isempty(whitepoint)),
	whitepoint = 'D65';
end;

imOut = RGB2XYZ(imIn, whitepoint);
imOut = XYZ2xyY(imOut);
imOut(:, :, 3) = tonemap_sigmoid_gain(imOut(:, :, 3), varargin{:});
imOut = xyY2XYZ(imOut);
imOut = XYZ2RGB(imOut, whitepoint);
