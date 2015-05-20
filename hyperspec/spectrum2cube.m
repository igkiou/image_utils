function cube = spectrum2cube(spectrum)

cube = reshape(vec(spectrum), [1 1 numel(spectrum)]);
