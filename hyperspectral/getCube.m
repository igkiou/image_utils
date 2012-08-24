function cube = getCube(name, EXT, wavelengths, exposures, flipFlag)

if ((nargin < 2) || (isempty(EXT))),
	EXT = 'tif';
end;

if ((nargin < 3) || (isempty(wavelengths))),
	wavelengths = 420:10:720;
end;

if ((nargin < 4) || (isempty(exposures))),
	exposures = [];
end;

if ((nargin < 5) || (isempty(flipFlag))),
	flipFlag = 0;
end;

numWavelengths = length(wavelengths);
if (isempty(exposures)),
	I = imread(sprintf('%s_%d.%s', name, wavelengths(1), EXT));
	[M N K] = size(I);
	cube = zeros(M, N, numWavelengths, class(I));

	for iter = 1:numWavelengths,
		I = imread(sprintf('%s_%d.%s', name, wavelengths(iter), EXT));
	% 	I = double(I);
		if (flipFlag == 1),
			I = imflip(I);
		end;
		cube(:, :, iter) = I(:, :, 1);
	end;
elseif (numel(exposures) == numWavelengths),
	I = imread(sprintf('%s_exp%gms_%d.%s', name, exposures(1), wavelengths(1), EXT));
	[M N K] = size(I);
	cube = zeros(M, N, numWavelengths, class(I));

	for iter = 1:numWavelengths,
		I = imread(sprintf('%s_exp%gms_%d.%s', name, exposures(1), wavelengths(iter), EXT));
	% 	I = double(I);
		if (flipFlag == 1),
			I = imflip(I);
		end;
		cube(:, :, iter) = I(:, :, 1);
	end;
else
	if (numel(exposures) ~= 1),
		warning('Number of exposures provided different than number of wavelengths and larger than one. Only first exposure will be used.');
		exposures = exposures(1);
	end;
	I = imread(sprintf('%s_exp%gms_%d.%s', name, exposures, wavelengths(1), EXT));
	[M N K] = size(I);
	cube = zeros(M, N, numWavelengths, class(I));

	for iter = 1:numWavelengths,
		I = imread(sprintf('%s_exp%gms_%d.%s', name, exposures, wavelengths(iter), EXT));
	% 	I = double(I);
		if (flipFlag == 1),
			I = imflip(I);
		end;
		cube(:, :, iter) = I(:, :, 1);
	end;
end;
