function writeSingle(im, name, EXT, wavelength, exposure, flipFlag)

if (isempty(EXT)),
	EXT = 'tif';
end;

if (nargin < 5),
	exposure = [];
end;

if ((nargin < 6) || isempty(flipFlag)),
	flipFlag = 0;
end;

if (numel(wavelength) ~= 1),
	warning('More than one wavelengths provided. Only first wavelength will be used.')
	wavelength = wavelength(1);
end;

if (isempty(exposure)),
	I = im;
	if (flipFlag == 1),
		I = imflip(I);
	end;
	imwrite(I, sprintf('%s_%d.%s', name, wavelength, EXT));
else
	if (numel(exposure) ~= 1),
		warning('More than one exposures provided. Only first exposure will be used.')
		exposure = exposure(1);
	end;
	I = im;
	if (flipFlag == 1),
		I = imflip(I);
	end;
	imwrite(I, sprintf('%s_exp%gms_%d.%s', name, exposure, wavelength, EXT));
end;
