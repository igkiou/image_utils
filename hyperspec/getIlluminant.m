function [illum foundWavelengths indsOrig] = getIlluminant(illumName, wavelengths)

if ((nargin < 1) || isempty(illumName)),
	illumName = 'd65';
end;

if ((nargin < 2) || isempty(wavelengths)),
	wavelengths = getDefaultWavelengths;
end;
numWavelengths = length(wavelengths);

if (strcmpi(illumName, 'flat')),
	illum = ones(size(wavelengths));
	foundWavelengths = wavelengths;
	indsOrig = 1:numWavelengths;
else
	[illumWavelengths illum] = illuminant('d65');
	[foundWavelengths indsIllum indsOrig] = intersect(illumWavelengths, wavelengths);
	numFoundWavelengths = length(foundWavelengths);
	if (numFoundWavelengths ~= numWavelengths),
		warning('Not all wavelengths in the original vector found in illuminant file.');
	end;
	illum = illum(indsIllum)';
end;
