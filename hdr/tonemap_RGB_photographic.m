function imOut = tonemap_RGB_photographic(imIn, varargin)

imOut = zeros(size(imIn));
imOut(:, :, 1) = tonemap_photographic(imIn(:, :, 1), varargin{:});
imOut(:, :, 2) = tonemap_photographic(imIn(:, :, 2), varargin{:});
imOut(:, :, 3) = tonemap_photographic(imIn(:, :, 3), varargin{:});
