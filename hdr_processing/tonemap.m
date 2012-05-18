function imOut = tonemap(imIn, value, gammaCorrection)

if (nargin < 3),
	imOut = tonemap_clipping(imIn, 'percentile', value);
else
	imOut = tonemap_clipping(imIn, 'percentile', value, gammaCorrection);
end;
