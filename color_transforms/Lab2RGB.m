function RGB = Lab2RGB(LAB, wtpoint)

if ((nargin < 2) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

XYZ = Lab2XYZ(LAB, wtpoint);
RGB = XYZ2RGB(XYZ, wtpoint);

