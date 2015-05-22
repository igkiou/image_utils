function RGB = Lchuv2RGB(LCHuv, wtpoint)

if ((nargin < 2) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

LUV = Lchuv2Luv(LCHuv);
RGB = Luv2RGB(LUV, wtpoint);

