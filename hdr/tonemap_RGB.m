function imOut = tonemap_RGB(imIn, varargin)

imOut = zeros(size(imIn));
imOut(:, :, 1) = tonemap_clipping(imIn(:, :, 1), varargin{:});
imOut(:, :, 2) = tonemap_clipping(imIn(:, :, 2), varargin{:});
imOut(:, :, 3) = tonemap_clipping(imIn(:, :, 3), varargin{:});
