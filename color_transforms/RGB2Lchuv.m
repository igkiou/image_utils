function [LCHuv suv] = RGB2Lchuv(RGB, wtpoint)

if ((nargin < 2) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

LUV = RGB2Luv(RGB, wtpoint);
[LCHuv suv] = Luv2Lchuv(LUV);
