function LAB = XYZ2Lab(XYZ, whitepoint)

if ((nargin < 2) || isempty(whitepoint)),
	whitepoint = 'd65';
end;

WXYZ = getWhitepoint(whitepoint);
XYZ(:, :, 1) = XYZ(:, :, 1) / WXYZ(1);
XYZ(:, :, 2) = XYZ(:, :, 2) / WXYZ(2);
XYZ(:, :, 3) = XYZ(:, :, 3) / WXYZ(3);
XYZ = f(XYZ);

LAB = zeros(size(XYZ));
LAB(:, :, 1) = 116 * XYZ(:, :, 2) - 16;
LAB(:, :, 2) = 500 * (XYZ(:, :, 1) - XYZ(:, :, 2));
LAB(:, :, 3) = 200 * (XYZ(:, :, 2) - XYZ(:, :, 3));

end

function ft = f(t)

epsilon = 216 / 24389; % (6 / 29) ^ 3;
kappa = 24389 / 27;

inds = t > epsilon;
ft = zeros(size(t));
ft(inds) = t(inds) .^ (1 / 3);
ft(~inds) = (kappa * t(~inds) + 16) / 116;

end