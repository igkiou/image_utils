function imOut = tonemap(imIn, value)

imOut = tonemap_clipping(imIn, 'percentile', value);
