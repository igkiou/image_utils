function XYZ = getWhitepoint(spec, wavelengths)

if ((nargin < 1) || isempty(spec)),
	spec = 'd65';
end;

if (nargin < 2),
	spec = lower(spec);
	if(any(strcmp(spec, {'d65', 'a', 'flat'}))),
		[spectrum, wavelengths] = getIlluminant(spec);
		XYZ = image2Color(cube2XYZ(spectrum2cube(spectrum), wavelengths)); 
		XYZ = XYZ / XYZ(2);
	elseif (strcmp(spec, 'd50')),
		xyY(1:2) = T2xy(5003);
		xyY(3) = 1;
		XYZ = image2Color(xyY2XYZ(color2Image(xyY)));
	elseif (isnumeric(spec)),	
		xyY(1:2) = T2xy(spec);
		xyY(3) = 1;
		XYZ = image2Color(xyY2XYZ(color2Image(xyY)));
	else
		XYZ = whitepoint(spec);
	end;
else
	if (numel(spec) ~= numel(wavelengths)),
		error('When two arguments are provided, they must be of equal size.');
	end;
	XYZ = cube2XYZ(spectrum2cube(spec), wavelengths);
	XYZ = XYZ / XYZ(2);
end;
