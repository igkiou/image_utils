function hdr = ldr2hdr(ldrIms, exposureVec, minLimit, maxLimit, baseFile)
%LDR2HDR    Create high dynamic range image.
%   HDR = LDR2HDR(LDR, EXPOSURE, MINLIMIT, MAXLIMIT, BASEFILE) creates the
%   single-precision high dynamic range image HDR from the set of spatially
%   registered low dynamic range images, stacked in the columns of the LDR
%   matrix. EXPOSURE contains (possibly relative) exposure values for each
%   of the LDR images. MINLIMIT and MAXLIMIT control the minimum and
%   maximum "correctly exposed" values for each LDR image. BASEFILE
%   provides the index of the LDR image whose exposure will be used as base
%   exposure. The algorithm works by assuming that, for properly exposed
%   pixels, the sensor response curve is linear. LDR images should be
%   prealigned. 

[numPixels numFiles] = size(ldrIms);
if (length(exposureVec) ~= numFiles),
	error('Number of exposure values must be equal to number of LDR images.');
end;

if ((nargin < 3) || (isempty(minLimit))),
	minLimit = 5;
end;

if ((nargin < 4) || (isempty(maxLimit))),
	minLimit = 250;
end;

hdr = zeros(numPixels, 1, 'single');
properlyExposedCount = zeros(size(hdr));
someUnderExposed = false(size(hdr));
someOverExposed = false(size(hdr));
someProperlyExposed = false(size(hdr));

if ((nargin < 5) || (isempty(baseFile))),
	if (isempty(find(exposureVec == 1, 1)))
		baseExposure = median(exposureVec);
	else
		baseExposure = 1;
	end;
else
	baseExposure = exposureVec(baseFile);
end;
	
% Construct the HDR image by iterating over the LDR images.
for iterFile = 1:numFiles,

	ldr = ldrIms(:, iterFile);
	relExposure = exposureVec(iterFile) / baseExposure;

	underExposed = ldr < minLimit;
	someUnderExposed = someUnderExposed | underExposed;

	overExposed = ldr > maxLimit;
	someOverExposed = someOverExposed | overExposed;

	properlyExposed = ~(underExposed | overExposed);
	someProperlyExposed = someProperlyExposed | properlyExposed;

	properlyExposedCount(properlyExposed) = properlyExposedCount(properlyExposed) + 1;

	% Remove over- and under-exposed values.
	ldr(~properlyExposed) = 0;

	% Bring the intensity of the LDR image into a common HDR domain by
	% "normalizing" using the relative exposure, and then add it to the
	% accumulator.
	hdr = hdr + single(ldr) ./ relExposure; 
end;

% Average the values in the accumulator by the number of LDR images that
% contributed to each pixel to produce the HDR radiance map.
hdr = hdr ./ max(properlyExposedCount, 1);

% For pixels that were completely over-exposed, assign the maximum
% value computed for the properly exposed pixels.
hdr(someOverExposed & ~someUnderExposed & ~someProperlyExposed) = max(hdr(someProperlyExposed));

% For pixels that were completely under-exposed, assign the
% minimum value computed for the properly exposed pixels.
hdr(someUnderExposed & ~someOverExposed & ~someProperlyExposed) = min(hdr(someProperlyExposed));

% For pixels that were sometimes underexposed, sometimes
% overexposed, and never properly exposed, use roifill.
fillMask = imdilate(someUnderExposed & someOverExposed & ~someProperlyExposed, ones(3,3));
if any(fillMask(:))
	warning('There exist pixels that were never properly exposed, and not always underexposed or always overexposed.');
	hdr(:,:,1) = roifill(hdr(:,:,1), fillMask(:,:,1));
	hdr(:,:,2) = roifill(hdr(:,:,2), fillMask(:,:,2));
	hdr(:,:,3) = roifill(hdr(:,:,3), fillMask(:,:,3));
end;
