function RGB = Lch2RGB(LCH, wtpoint)

if ((nargin < 2) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

LAB = Lch2Lab(LCH);
RGB = Lab2RGB(LAB, wtpoint);

