function [LUV c s h] = XYZ2Luv(XYZ)

LUV = colorspace('XYZ->LUV', XYZ);
c = hypot(LUV(:, :, 2), LUV(:, :, 3));
s = c ./ LUV(:, :, 1);
h = atan2(LUV(:, :, 3), LUV(:, :, 2));
h = h * 180 / pi + 360 * (h < 0);
