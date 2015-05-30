function XYZ = Luv2XYZ(LUV, whitepoint)

if ((nargin < 2) || isempty(whitepoint)),
	whitepoint = 'd65';
end;

% if (strcmpi(whitepoint, 'd65')),
% 	XYZ = colorspace('Luv->XYZ', LUV);
% 	return;
% else
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
% end;

end

function fm1t = fm1(t)

inds = t > 6 / 29;
fm1t = zeros(size(t));
fm1t(inds) = t(inds) .^ 3;
fm1t(~inds) = 3 * (6 / 29) ^ 2 * (t(~inds) + 4 / 29);

end


function XYZ = Lab2XYZ(LAB, whitepoint)

if ((nargin < 2) || isempty(whitepoint)),
	whitepoint = 'd65';
end;

XYZ = zeros(size(LAB));
XYZ(:, :, 2) = (LAB(:, :, 1) + 16) / 116;
XYZ(:, :, 1) = LAB(:, :, 2) / 500 + XYZ(:, :, 2);
XYZ(:, :, 3) = XYZ(:, :, 2) - LAB(:, :, 3) / 200;

XYZ = fm1(XYZ);
WXYZ = getWhitepoint(whitepoint);
XYZ(:, :, 1) = XYZ(:, :, 1) * WXYZ(1);
XYZ(:, :, 2) = XYZ(:, :, 2) * WXYZ(2);
XYZ(:, :, 3) = XYZ(:, :, 3) * WXYZ(3);

end

function fm1t = fm1(t)

epsilon = 216 / 24389; % (6 / 29) ^ 3;
epsilonCbrt = 6 / 29; % cbrt(epsilon)
kappa = 24389 / 27;

inds = t > epsilonCbrt;
fm1t = zeros(size(t));
fm1t(inds) = t(inds) .^ 3;
fm1t(~inds) = (116 * t(~inds) - 16) / kappa;
% fm1t(~inds) = 3 * (6 / 29) ^ 2 * (t(~inds) + 4 / 29);

end
