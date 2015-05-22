function imsOut = tonemap_luminance_photographic_batch(imsIn, whitepoint, varargin)

if ((nargin < 2) || isempty(whitepoint)),
	whitepoint = 'D65';
end;

[numPixels numIms] = size(imsIn);
imsLumi = zeros(size(imsIn, 1) / 3, numIms);
for iterIm = 1:numIms,
	imsLumi(:, iterIm) = vec(getLuminance(reshape(imsIn(:, iterIm), [numPixels / 3 1 3]), 'RGB', whitepoint));
end;
imsLumi = tonemap_photographic(imsLumi, varargin{:});
imsOut = zeros(size(imsIn));
for iterIm = 1:numIms,
	imsOut(:, iterIm) = vec(setLuminance(reshape(imsIn(:, iterIm), [numPixels / 3 1 3]), imsLumi(:, iterIm), 'RGB', whitepoint));
end;
