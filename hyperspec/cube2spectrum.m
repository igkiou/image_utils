function spectrum = cube2spectrum(cube)

spectrum = reshape(vec(cube), [1 numel(cube)]);
