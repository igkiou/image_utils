function RGB = XYZ2RGB(XYZ, whitepoint)

if ((nargin < 2) || isempty(whitepoint)),
	whitepoint = 'D65';
end;

if (strcmpi(whitepoint, 'd65')),
	RGB = colorspace('XYZ->RGB', XYZ);
	return;
else
	Y = getXYZ2RGBTransform(whitepoint);
	RGB = reshape((Y * [reshape(XYZ(:, :, 1), 1, size(XYZ, 1) * size(XYZ, 2));...
				reshape(XYZ(:, :, 2), 1, size(XYZ, 1) * size(XYZ, 2));...
				reshape(XYZ(:, :, 3), 1, size(XYZ, 1) * size(XYZ, 2))])',...
			size(XYZ));

	val = min(RGB(:));
	if (val < 0);
		RGB = RGB - val;
	end;
	return;
end;
