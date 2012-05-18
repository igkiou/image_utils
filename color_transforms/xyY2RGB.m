function RGB = xyY2RGB(xyY, wtpoint)

if ((nargin < 2) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

XYZ = xyY2XYZ(xyY);
RGB = XYZ2RGB(XYZ, wtpoint);

