function LUV = Lchuv2Luv(LCHuv)

LUV = zeros(size(LCHuv));
LUV(:, :, 1) = LCHuv(:, :, 1);
LUV(:, :, 2) = cos(LCHuv(:, :, 3) * pi / 180) .* LCHuv(:, :, 2);
LUV(:, :, 2) = sin(LCHuv(:, :, 3) * pi / 180) .* LCHuv(:, :, 2);
