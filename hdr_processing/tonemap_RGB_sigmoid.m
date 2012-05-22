function imOut = tonemap_RGB_sigmoid(imIn, varargin)

imOut = zeros(size(imIn));
imOut(:, :, 1) = tonemap_sigmoid(imIn(:, :, 1), varargin{:});
imOut(:, :, 2) = tonemap_sigmoid(imIn(:, :, 2), varargin{:});
imOut(:, :, 3) = tonemap_sigmoid(imIn(:, :, 3), varargin{:});
