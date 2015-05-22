function RGB = Luv2RGB(LUV, wtpoint)

if ((nargin < 2) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

XYZ = Luv2XYZ(LUV, wtpoint);
RGB = XYZ2RGB(XYZ, wtpoint);

