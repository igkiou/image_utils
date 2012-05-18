function cube = getCube(name, EXT, wavelengthInds, flipFlag)

if ((nargin < 2) || (isempty(EXT))),
	EXT = 'tif';
end;

if ((nargin < 3) || (isempty(wavelengthInds))),
	wavelengthInds = 420:10:720;
end;

if ((nargin < 4) || (isempty(flipFlag))),
	flipFlag = 0;
end;

numWavelength = length(wavelengthInds);
I = imread(sprintf('%s_%d.%s', name, wavelengthInds(1), EXT));
[M N K] = size(I);
cube = zeros(M, N, numWavelength);

for iter = 1:numWavelength,
	I = imread(sprintf('%s_%d.%s', name, wavelengthInds(iter), EXT));
	I = double(I);
	if (flipFlag == 1),
		I = imflip(I);
	end;
	cube(:, :, iter) = I(:, :, 1);
end;
