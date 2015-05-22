function LUV = RGB2Luv(RGB, wtpoint)

if ((nargin < 2) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

XYZ = RGB2XYZ(RGB, wtpoint);
LUV = XYZ2Luv(XYZ, wtpoint);
