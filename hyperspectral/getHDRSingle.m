function hdr = getHDRSingle(name, EXT, wavelength, exposures, varargin)

numExposures = length(exposures);
I = imread(sprintf('%s_%d.%s', name, wavelengths(1), EXT));
[M N K] = size(I);
hdr = zeros(M, N, numExposures, class(I));
for iterExposure = 1:numExposures,
	hdr(:, :, iterExposure) = getSingle(name, EXT, wavelength,...
				exposures(iterExposure), varargin{:});
end;
