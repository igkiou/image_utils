function LAB = cube2Lab(img, wavelengths, wtpoint, varargin)

if ((nargin < 3) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

XYZ = cube2XYZ(img, wavelengths, varargin{:});
LAB = XYZ2Lab(XYZ, wtpoint);
