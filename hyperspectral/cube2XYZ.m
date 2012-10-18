function XYZ = cube2XYZ(cube, wavelengths, compensation_function, formulary)

if ((nargin < 2) || isempty(wavelengths)),
	wavelengths = getDefaultWavelengths;
end;
numWavelengths = length(wavelengths);

if ((nargin < 3) || isempty(compensation_function)),
	compensation_function = getCompensationFunction([], wavelengths);
end;

if ((nargin < 4) || isempty(formulary)),
	formulary = 'CIE_1931';
end;

if (length(compensation_function) ~= numWavelengths),
	error('Length of wavelengths and compensation functions must be equal.');
end;

if (size(cube, 3) ~= numWavelengths)
	error('Number of channels in the cube and length of wavelenghts must be equal.');
end;

[CMF foundWavelengths indsOrig] = getCMF(formulary, wavelengths);
numFoundWavelengths = length(foundWavelengths);
CMF = CMF / sum(CMF(:, 2), 1);
XYZ = zeros(size(cube, 1), size(cube, 2), 3);
  
for iterChannel = 1:3,
	for iterWavelength = 1:numFoundWavelengths,
		XYZ(:, :, iterChannel) = XYZ(:, :, iterChannel) ...
						+ CMF(iterWavelength, iterChannel) ...
						* cube(:, :, indsOrig(iterWavelength)) ...
						/ compensation_function(indsOrig(iterWavelength));
	end;
end;
