function [compensation_function foundWavelengths indsOrig] = getCompensationFunction(compensation, wavelengths)
% compensation_function is the amount by which to divide.

if ((nargin < 1) || isempty(compensation)),
	compensation = 'flat';
end;

if ((nargin < 2) || isempty(wavelengths)),
	wavelengths = getDefaultWavelengths;
end;

numWavelengths = length(wavelengths);
if (strcmp(compensation, 'flat')),
	compensation_function = ones(size(wavelengths));
	foundWavelengths = wavelengths;
	indsOrig = 1:numWavelengths;
elseif (strcmp(compensation, 'nuance_sensitivity')),
	l = load('-ascii','nuance_sensitivity.txt');
	[foundWavelengths indsFunc indsOrig] = intersect(l(:, 1), wavelengths);
	numFoundWavelengths = length(foundWavelengths);
	if (numFoundWavelengths ~= numWavelengths),
		warning('Not all wavelengths in the original vector found in nuance sensitivity file.');
	end;
	compensation_function = l(indsFunc, 2);
end;

compensation_function = compensation_function / sum(compensation_function);
