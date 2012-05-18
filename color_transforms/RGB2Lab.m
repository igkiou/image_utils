function LAB = RGB2Lab(RGB, wtpoint)

if ((nargin < 2) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

XYZ = RGB2XYZ(RGB, wtpoint);
LAB = XYZ2Lab(XYZ, wtpoint);
