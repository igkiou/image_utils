function [LCH s] = RGB2Lch(RGB, wtpoint)

if ((nargin < 2) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

LAB = RGB2Lab(RGB, wtpoint);
[LCH s] = Lab2Lch(LAB);
