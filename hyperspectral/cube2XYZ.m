function XYZ = cube2XYZ(img, wavelengths, compensation_function)

if ((nargin < 2) || isempty(wavelengths)),
	wavelengths = 420:10:720;
end;
numWavelengths = length(wavelengths);

if ((nargin < 3) || isempty(compensation_function)),
	compensation_function = ones(numWavelengths, 1);
end;

l = load('-ascii','CMF_CIE_1931_2_degree.txt');
[foundWavelengths indsCIE indsOrig] = intersect(l(:,1), wavelengths);
numFoundWavelengths = length(foundWavelengths);
if (numFoundWavelengths ~= numWavelengths),
	warning('Not all wavelengths in the original vector found in CIE CMF file.');
end;

cmf = l(indsCie, 2:4); %color matching
% l = l / max(l(:));
XYZ = zeros(size(img, 1), size(img, 2), 3);
  
for iterChannel = 1:3,
	for iterWavelength = 1:numFoundWavelengths,
		XYZ(:, :, iterChannel) = XYZ(:, :, iterChannel) ...
						+ cmf(indsCIE(iterWavelength), iterChannel) ...
						* img(:, :, indsOrig(iterWavelength)) ...
						/ compensation_function(indsOrig(iterWavelength));
	end;
end;
