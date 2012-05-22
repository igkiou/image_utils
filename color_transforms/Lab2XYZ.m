function XYZ = Lab2XYZ(LAB, wtpoint)

if ((nargin < 2) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

if (strcmpi(whitepoint, 'd65')),
	XYZ = colorspace('Lab->XYZ', LAB);
	return;
else
	WXYZ = whitepoint(wtpoint);
	Xn = WXYZ(1);
	Yn = WXYZ(2);
	Zn = WXYZ(3);

	XYZ(:, :, 2) = Yn * fm1((LAB(:, :, 1) + 16) / 116);
	XYZ(:, :, 1) = Xn * fm1((LAB(:, :, 1) + 16) / 116 + LAB(:, :, 2) / 500);
	XYZ(:, :, 3) = Zn * fm1((LAB(:, :, 1) + 16) / 116 - LAB(:, :, 3) / 200);
	return;
end;

end

function fm1t = fm1(t)

inds = t > 6 / 29;
fm1t = zeros(size(t));
fm1t(inds) = t(inds) .^ 3;
fm1t(~inds) = 3 * (6 / 29) ^ 2 * (t(~inds) + 4 / 29);

end
