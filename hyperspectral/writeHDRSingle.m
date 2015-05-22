function writeHDRSingle(hdr, name, EXT, wavelength, exposures, varargin)

numExposures = length(exposures);
for iterExposure = 1:numExposures,
	writeSingle(hdr(:, :, iterExposure), name, EXT, wavelength,...
				exposures(iterExposure), varargin{:});
end;
