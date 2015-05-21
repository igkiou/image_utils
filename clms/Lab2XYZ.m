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
