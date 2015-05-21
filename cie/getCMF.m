function [CMF, foundWavelengths, indsOrig] = getCMF(formulary, wavelengths)

if ((nargin < 1) || isempty(formulary)),
	formulary = 'cie1931';
end;

if ((nargin < 2) || isempty(wavelengths)),
	wavelengths = getDefaultWavelengths;
end;
numWavelengths = length(wavelengths);

if (strcmpi(formulary, 'cie1931')),
	tempMat = csvread('cmf31.csv');
	cmfWavelengths = tempMat(:, 1);
	CMF = tempMat(:, 2:4);
elseif (strcmpi(formulary, 'cie1964')),
	tempMat = csvread('cmf64.csv');
	cmfWavelengths = tempMat(:, 1);
	CMF = tempMat(:, 2:4);
else
	error('Unknown illuminant');
end;

if ((nargin < 2) || isempty(wavelengths)),
	foundWavelengths = cmfWavelengths;
	indsOrig = 1:length(foundWavelengths);
else
	CMFInterp = interp1(cmfWavelengths, CMF, wavelengths, 'linear');
	indsOrig = find(~isnan(CMFInterp(:, 1)));
	CMF = CMFInterp(indsOrig, :);
	foundWavelengths = wavelengths(indsOrig);
	numFoundWavelengths = length(foundWavelengths);
	if (numFoundWavelengths ~= numel(wavelengths)),
		warning('Not all wavelengths in the original vector found in CMF file.');
	end;
end;
