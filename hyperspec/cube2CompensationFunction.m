function [comp_func foundWavelengths indsOrig] = cube2CompensationFunction(cube, wavelengths, illumName)

if ((nargin < 2) || isempty(wavelengths)),
	wavelengths = getDefaultWavelengths;
end;
numWavelengths = length(wavelengths);

if ((nargin < 3) || isempty(illumName)),
	illumName = 'd65';
end;

if (size(cube, 3) ~= numWavelengths)
	error('Number of channels in the cube and length of wavelenghts must be equal.');
end;

[illum foundWavelengths indsOrig] = getIlluminant(illumName, wavelengths);
numFoundWavelengths = length(foundWavelengths);

comp_func = mean(reshape(cube(:, :, indsOrig), [size(cube, 1) * size(cube, 2), numFoundWavelengths]), 1);
comp_func = comp_func ./ illum;
comp_func = comp_func / sum(comp_func);
