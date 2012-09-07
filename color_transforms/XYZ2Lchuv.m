function [LCHuv suv] = XYZ2Lchuv(XYZ, wtpoint)

if ((nargin < 2) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

LUV = XYZ2Luv(XYZ, wtpoint);
[LCHuv suv] = Luv2Lchuv(LUV);
