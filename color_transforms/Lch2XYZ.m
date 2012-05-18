function XYZ = Lch2XYZ(LCH, wtpoint)

if ((nargin < 2) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

LAB = Lch2Lab(LCH);
XYZ = Lab2XYZ(LAB, wtpoint);

