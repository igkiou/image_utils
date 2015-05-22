function imOut = tonemap_photographic(imIn, key, burn, mask)
% Implementation of Reinhard et al., Photographic Tone Reproduction for
% Digital Images, SIGGRAPH 2002.

if ((nargin < 2) || isempty(key)),
	key = 0.18;
end;

if ((nargin < 3) || isempty(burn)),
	burn = 0.85;
end;

if ((nargin < 4) || isempty(mask)),
	mask = true(size(imIn, 1), size(imIn, 2));
end;

% TODO: Perhaps add parameter to exclude bottom/top 1% of pixels?
% mask = (imIn(:,:,1) ~= 0) & (imIn(:,:,2) ~= 0) & (imIn(:,:,3) ~= 0);
mask = repmat(mask, [1 1 size(imIn, 3)]);
imOut = key / meanlogv(imIn(mask)) * imIn;
imOut = imOut .* (1 + imOut / (burn * maxv(imOut)) ^ 2) ./ (1 + imOut);
% imOut = imOut ./ (1 + imOut);
