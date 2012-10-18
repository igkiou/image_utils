function xyz = getWhitepoint(spec, wavelengths)

if (nargin < 2),
	if (~ischar(spec)),
		error('When a single argument is provided, it must be a string.');
	end;
	
	switch lower(spec)
		case 'flat'
			xyz = [1.000081049323616 1.0 1.000339540500596];
		case 'd65'
			xyz = [0.950456, 1, 1.088754];
		otherwise
			xyz = whitepoint(spec);
	end;
else
	if (numel(spec) ~= numel(wavelengths)),
		error('When two arguments are provided, they must be of equal size.');
	end;
	xyz = cube2XYZ(spectrum2cube(spec), wavelengths);
	xyz = xyz / xyz(2);
end;
