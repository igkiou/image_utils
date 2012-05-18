function xyY = RGB2xyY(RGB, wtpoint)

if ((nargin < 2) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

XYZ = RGB2XYZ(RGB, wtpoint);
xyY = XYZ2xyY(XYZ);
