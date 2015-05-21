function RGB = XYZ2RGB(XYZ, whitepoint, colorspace)

if ((nargin < 2) || isempty(whitepoint)),
	whitepoint = 'd65';
end;

if ((nargin < 3) || isempty(colorspace)),
	colorspace = 'srgb';
end;

T = getXYZ2RGBTransform(whitepoint, colorspace);
RGB = reshape((T * [reshape(XYZ(:, :, 1), 1, size(XYZ, 1) * size(XYZ, 2));...
			reshape(XYZ(:, :, 2), 1, size(XYZ, 1) * size(XYZ, 2));...
			reshape(XYZ(:, :, 3), 1, size(XYZ, 1) * size(XYZ, 2))])',...
			size(XYZ));

% val = min(RGB(:));
% if (val < 0);
% 	RGB = RGB - val;
% end;
