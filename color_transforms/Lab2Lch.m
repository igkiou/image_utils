function [LCH s] = Lab2Lch(LAB)

LCH = colorspace('Lab->Lch', LAB);

% my implementation
% LCH = zeros(size(LAB));
% LCH(:, :, 1) = LAB(:, :, 1);
% LCH(:, :, 2) = hypot(LAB(:, :, 2), LAB(:, :, 3));
% H = atan2(LAB(:, :, 3), LAB(:, :, 2)) * 180 / pi;
% 
% indsNeg = H < 0;
% indsLarge = H > 360;
% H(indsNeg) = H(indsNeg) + 360;
% H(indsLarge) = H(indsLarge) - 360;
% LCH(:, :, 3) = H;

if (nargout > 1),
	s = LCH(:, :, 2) ./ LCH(:, :, 1);
end;
