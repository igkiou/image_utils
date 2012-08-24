function writeCube(cube, name, EXT, wavelengths, exposures, flipFlag)

if ((nargin < 3) || (isempty(EXT))),
	EXT = 'tif';
end;

if ((nargin < 4) || (isempty(wavelengths))),
	wavelengths = 420:10:720;
end;

if (nargin < 5),
	exposures = [];
end;

if ((nargin < 6) || (isempty(flipFlag))),
	flipFlag = 0;
end;
	
numWavelengths = numel(wavelengths);
if (isempty(exposures)),
	for iterWavelength = 1:numWavelengths,
		I = cube(:, :, iterWavelength);
		if (flipFlag == 1),
			I = imflip(I);
		end;
		imwrite(I,...
			sprintf('%s_%d.%s', name, wavelengths(iterWavelength), EXT));
	end;
elseif (numel(exposures) == numWavelengths),
	for iterWavelength = 1:numWavelengths,
		I = cube(:, :, iterWavelength);
		if (flipFlag == 1),
			I = imflip(I);
		end;
		imwrite(I,...
			sprintf('%s_exp%gms_%d.%s', name,...
				exposures(iterWavelength),...
				wavelengths(iterWavelength), EXT));
	end;
else
	if (numel(exposures) ~= 1),
		warning('Number of exposures provided different than number of wavelengths and larger than one. Only first exposure will be used.');
		exposures = exposures(1);
	end;
	for iterWavelength = 1:numWavelengths,
		I = cube(:, :, iterWavelength);
		if (flipFlag == 1),
			I = imflip(I);
		end;
		imwrite(I,...
			sprintf('%s_exp%gms_%d.%s', name,...
				exposures,...
				wavelengths(iterWavelength), EXT));
	end;
end;
