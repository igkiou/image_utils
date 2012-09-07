function [LCHuv s] = cube2Lchuv(img, wavelengths, wtpoint, varargin)

if ((nargin < 3) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

XYZ = cube2XYZ(img, wavelengths, varargin{:});
[LCHuv suv] = XYZ2Lchuv(XYZ, wtpoint);
