function [LCH s] = XYZ2Lch(XYZ, wtpoint)

if ((nargin < 2) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

LAB = XYZ2Lab(XYZ, wtpoint);
[LCH s] = Lab2Lch(LAB);
