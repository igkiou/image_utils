function compensation_function = getCompensationFunction(compensation, wavelengths)
% compensation_function is the amount by which to divide.

if ((nargin < 1) || isempty(compensation)),
	compensation = 'nuance_sensitivity';
end;

if ((nargin < 2) || isempty(wavelengths)),
	wavelengths = 420:10:720;
end;

numWavelengths = length(wavelengths);
if (strcmp(compensation, 'none')),
	compensation_function = ones(numWavelengths);
elseif (strcmp(compensation, 'nuance_sensitivity')),
	l = load('-ascii','nuance_sensitivity.txt');
	[foundWavelengths indsFunc] = intersect(l(:, 1), wavelengths);
	numFoundWavelengths = length(foundWavelengths);
	if (numFoundWavelengths ~= numWavelengths),
		warning('Not all wavelengths in the original vector found in nuance sensitivity file.');
	end;
	compensation_function = l(indsFunc, 2);
end;
