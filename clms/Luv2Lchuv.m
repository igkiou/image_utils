function [LCHuv suv] = Luv2Lchuv(LUV)

LCHuv = zeros(size(LUV));
LCHuv(:, :, 1) = LUV(:, :, 1);
LCHuv(:, :, 2) = hypot(LUV(:, :, 2), LUV(:, :, 3));
LCHuv(:, :, 3) = atan2(LUV(:, :, 3), LUV(:, :, 2));
LCHuv(:, :, 3) = LCHuv(:, :, 3) * 180 / pi + 360 * (LCHuv(:, :, 3) < 0);

if (nargout >= 2),	
	suv = LCHuv(:, :, 2) ./ LUV(:, :, 1);
end;
