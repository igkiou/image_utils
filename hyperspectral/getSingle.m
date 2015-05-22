function im = getSingle(name, EXT, wavelength, exposure, flipFlag)

if ((nargin < 2) || (isempty(EXT))),
	EXT = 'tif';
end;

if (nargin < 4),
	exposure = [];
end;

if ((nargin < 5) || (isempty(flipFlag))),
	flipFlag = 0;
end;

if (numel(wavelength) ~= 1),
	warning('More than one wavelengths provided. Only first wavelength will be used.')
	wavelength = wavelength(1);
end;

if (isempty(exposure)),
	im = imread(sprintf('%s_%d.%s', name, wavelength, EXT));
	if (flipFlag == 1),
		im = imflip(im);
	end;
else
	if (numel(exposure) ~= 1),
		warning('More than one exposures provided. Only first exposure will be used.')
		exposure = exposure(1);
	end;
	im = imread(sprintf('%s_exp%gms_%d.%s', name, exposure, wavelength, EXT));
	if (flipFlag == 1),
		im = imflip(im);
	end;
end;
