function LUV = XYZ2Luv(XYZ, whitepoint)

if ((nargin < 2) || isempty(whitepoint)),
	whitepoint = 'D65';
end;

if (strcmpi(whitepoint, 'd65')),
	LUV = colorspace('XYZ->LUV', XYZ);
	return;
else
	WXYZ = getWhitepoint(whitepoint);
	Xn = WXYZ(1);
	Yn = WXYZ(2);
	Zn = WXYZ(3);
	un = 4 * Xn / (Xn + 15 * Yn + 3 * Zn);
	vn = 9 * Yn / (Xn + 15 * Yn + 3 * Zn);
	
	denom = XYZ(:, :, 1) + 15 * XYZ(:, :, 2) + 3 * XYZ(:, :, 3);
	LUV = zeros(size(XYZ));
	LUV(:, :, 1) = 116 * f(XYZ(:, :, 2) / Yn) - 16;
	LUV(:, :, 2) = (4 * XYZ(:, :, 1)) ./ (denom + (denom == 0));
	LUV(:, :, 2) = 13 * LUV(:, :, 1) .* (LUV(:, :, 2) - un);
	LUV(:, :, 3) = (9 * XYZ(:, :, 2)) ./ (denom + (denom == 0));
	LUV(:, :, 3) = 13 * LUV(:, :, 1) .* (LUV(:, :, 3) - vn);
end;
	
end

function ft = f(t)

inds = t > (6 / 29) ^ 3;
ft = zeros(size(t));
ft(inds) = t(inds) .^ (1 / 3);
ft(~inds) = 1 / 3 * (29 / 6) ^ 2 * t(~inds) + 4 / 29;

end

