function XYZ = Lchuv2XYZ(LCHuv, wtpoint)

if ((nargin < 2) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

LUV = Lchuv2Luv(LCHuv);
XYZ = Lab2XYZ(LUV, wtpoint);

