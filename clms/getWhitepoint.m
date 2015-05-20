function xyz = getWhitepoint(spec, wavelengths)

if (nargin < 2),
	if (~ischar(spec)),
		error('When a single argument is provided, it must be a string.');
	end;

	spec = lower(spec);
	if (strcmp(spec, 'flat')),
		spec = 'ee';
	end;
	
	if(any(strcmp(spec, {'d65', 'a', 'ee'}))),
		[wavelengths, spectrum] = illuminant('d65');
		xyz = cube2XYZ(spectrum2cube(spectrum), wavelengths); 
		xyz = squeeze(xyz)' / xyz(2);
	else
		xyz = whitepoint(spec);
	end;
else
	if (numel(spec) ~= numel(wavelengths)),
		error('When two arguments are provided, they must be of equal size.');
	end;
	xyz = cube2XYZ(spectrum2cube(spec), wavelengths);
	xyz = xyz / xyz(2);
end;
