function XYZ = RGB2XYZ(RGB, whitepoint, colorspace)

if ((nargin < 2) || isempty(whitepoint)),
	whitepoint = 'd65';
end;

if ((nargin < 3) || isempty(colorspace)),
	colorspace = 'srgb';
end;

T = getRGB2XYZTransform(whitepoint, colorspace);
XYZ = reshape((T * [reshape(RGB(:, :, 1), 1, size(RGB, 1) * size(RGB, 2));...
			reshape(RGB(:, :, 2), 1, size(RGB, 1) * size(RGB, 2));...
			reshape(RGB(:, :, 3), 1, size(RGB, 1) * size(RGB, 2))])',...
			size(RGB));

% val = min(XYZ(:));
% if (val < 0);
% 	XYZ = XYZ - val;
% end;
