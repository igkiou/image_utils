function imOut = tonemap_photographic(imIn, key, burn)
% Implementation of Reinhard et al., Photographic Tone Reproduction for
% Digital Images, SIGGRAPH 2002.

if ((nargin < 2) || isempty(key)),
	key = 0.18;
end;

if ((nargin < 3) || isempty(burning)),
	burn = 0.85;
end;

imOut = key / meanlogv(imIn) * imIn;
imOut = imOut .* (1 + imOut / (burn * maxv(imOut)) ^ 2) ./ (1 + imOut);
