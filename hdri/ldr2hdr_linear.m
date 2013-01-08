function hdr = ldr2hdr_linear(ldrIms, exposures, minLimit, maxLimit, maxVal)
%	Combine different exposures assuming a linear camera response. Note that
%	this assumes that images are taken under the same aperture and
%	sensitivity (ISO) setting. 
%
%	The default parameters assume 8-bit LDR images.
%
%	Input:
%	ldrIms		-	M x N x S exposure stack of S LDR images, where M x N the
%						size of each image, and S the number of exposures.
%	exposures	-	S x 1 vector of exposure times.
%	minLimit		-	Underexposure value, in gray tones (default 5).
%	maxLimit		-	Overexposure value, in gray tones (default 250).
%	maxVal		-	Maximum gray tone of original LDR images (default 255).

[M N numIms] = size(ldrIms);
if (length(exposures) ~= numIms),
	error('Number of exposure values must be equal to number of LDR images.');
end;

if ((nargin < 3) || (isempty(minLimit))),
	minLimit = 5;
end;

if ((nargin < 4) || (isempty(maxLimit))),
	maxLimit = 250;
end;

if ((nargin < 5) || (isempty(maxVal))),
	maxVal = 255;
end;

numPixels = M * N;
ldrIms = reshape(ldrIms, [numPixels, numIms]);
hdr = zeros(numPixels, 1);

properlyExposedCount = zeros(numPixels, 1);
someUnderExposed = false(numPixels, 1);
someOverExposed = false(numPixels, 1);
someProperlyExposed = false(numPixels, 1);
	
% Construct the HDR image by iterating over the LDR images.
for iterIm = 1:numIms,

	ldr = ldrIms(:, iterIm);
	t = exposures(iterIm);

	underExposed = ldr < minLimit;
	someUnderExposed = someUnderExposed | underExposed;

	overExposed = ldr > maxLimit;
	someOverExposed = someOverExposed | overExposed;

	properlyExposed = ~(underExposed | overExposed);
	someProperlyExposed = someProperlyExposed | properlyExposed;

	properlyExposedCount(properlyExposed) = properlyExposedCount(properlyExposed) + 1;
	hdr(properlyExposed) = hdr(properlyExposed) ...
								+ double(ldr(properlyExposed)) ./ t; 
end;

hdr = hdr ./ max(properlyExposedCount, 1);

hdr = fillNeverProperlyExposedPixels(hdr, ...
						someUnderExposed, someOverExposed, someProperlyExposed);

hdr = hdr / maxVal;
hdr = single(hdr);
hdr = reshape(hdr, [M N]);
