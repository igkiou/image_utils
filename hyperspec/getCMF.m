function [CMF foundWavelengths indsOrig] = getCMF(formulary, wavelengths)

if ((nargin < 1) || isempty(formulary)),
	formulary = '1931_FULL';
end;

if ((nargin < 2) || isempty(wavelengths)),
	wavelengths = getDefaultWavelengths;
end;
numWavelengths = length(wavelengths);

% l = load('-ascii','CMF_CIE_1931_2_degree.txt');
[cmfWavelengths CMF(:, 1) CMF(:, 2) CMF(:, 3)] = colorMatchFcn(formulary);
[foundWavelengths indsCIE indsOrig] = intersect(cmfWavelengths, wavelengths);
numFoundWavelengths = length(foundWavelengths);
if (numFoundWavelengths ~= numWavelengths),
	warning('Not all wavelengths in the original vector found in CIE CMF file.');
end;

CMF = CMF(indsCIE, :); 
