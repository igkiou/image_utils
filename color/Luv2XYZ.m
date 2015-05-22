function XYZ = Luv2XYZ(LUV, whitepoint)

if ((nargin < 2) || isempty(whitepoint)),
	whitepoint = 'D65';
end;

if (strcmpi(whitepoint, 'd65')),
	XYZ = colorspace('Luv->XYZ', LUV);
	return;
else
	WXYZ = getWhitepoint(whitepoint);
	Xn = WXYZ(1);
	Yn = WXYZ(2);
	Zn = WXYZ(3);
	un = 4 * Xn / (Xn + 15 * Yn + 3 * Zn);
	vn = 9 * Yn / (Xn + 15 * Yn + 3 * Zn);
	
	XYZ = zeros(size(LUV));
	XYZ(:, :, 2) = (LUV(:, :, 1) + 16) / 116;
	XYZ(:, :, 2) = fm1(XYZ(:, :, 2)) * Yn;
	U = LUV(:, :, 2) ./ (13 * LUV(:, :, 1) + 1e-6 * (LUV(:, :, 1) == 0)) + un;
	V = LUV(:, :, 3) ./ (13 * LUV(:, :, 1) + 1e-6 * (LUV(:, :, 1) == 0)) + vn;
	XYZ(:, :, 1) = - (9 * XYZ(:, :, 2) .* U) ./ ((U - 4) .* V - U .* V);
	XYZ(:, :, 3) = (9 * XYZ(:, :, 2) - (15 * V .* XYZ(:, :, 2)) - (V .* XYZ(:,:,1))) ./ (3 * V);
	return;
end;

end

function fm1t = fm1(t)

inds = t > 6 / 29;
fm1t = zeros(size(t));
fm1t(inds) = t(inds) .^ 3;
fm1t(~inds) = 3 * (6 / 29) ^ 2 * (t(~inds) + 4 / 29);

end
