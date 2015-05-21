function [illum, foundWavelengths, indsOrig] = getIlluminant(illumName, wavelengths)

if ((nargin < 1) || isempty(illumName)),
	illumName = 'd65';
end;

if (strcmpi(illumName, 'flat')),
	if ((nargin < 2) || isempty(wavelengths)),
		illumWavelengths = (300:5:830)';
	else
		illumWavelengths = wavelengths;
	end;
	illum = ones(size(illumWavelengths));
elseif (strcmpi(illumName, 'a')),
	tempMat = csvread('a.csv');
	illumWavelengths = tempMat(:, 1);
	illum = tempMat(:, 2);
elseif (strcmpi(illumName, 'd65')),
	tempMat = csvread('d65.csv');
	illumWavelengths = tempMat(:, 1);
	illum = tempMat(:, 2);
elseif (strcmpi(illumName, 'd50')),
	[illum, illumWavelengths] = T2DIlluminant(5003);
elseif (isnumeric(illumName)),
	[illum, illumWavelengths] = T2DIlluminant(illumName);
else
	error('Unknown illuminant');
end;

if ((nargin < 2) || isempty(wavelengths)),
	foundWavelengths = illumWavelengths;
	indsOrig = 1:length(foundWavelengths);
else
	illumInterp = interp1(illumWavelengths, illum, wavelengths, 'linear');
	indsOrig = find(~isnan(illumInterp));
	illum = illumInterp(indsOrig);
	foundWavelengths = wavelengths(indsOrig);
	numFoundWavelengths = length(foundWavelengths);
	if (numFoundWavelengths ~= numel(wavelengths)),
		warning('Not all wavelengths in the original vector found in illuminant file.');
	end;
end;
