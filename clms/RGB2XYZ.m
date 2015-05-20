function XYZ = RGB2XYZ(RGB, whitepoint)

if ((nargin < 2) || isempty(whitepoint)),
	whitepoint = 'D65';
end;

% if (strcmpi(whitepoint, 'd65')),
% 	XYZ = colorspace('RGB->XYZ', RGB);
% 	return;
% else
	Y = getXYZ2RGBTransform(whitepoint);
	Y = eye(3) / Y;
	XYZ = reshape((Y * [reshape(RGB(:, :, 1), 1, size(RGB, 1) * size(RGB, 2));...
				reshape(RGB(:, :, 2), 1, size(RGB, 1) * size(RGB, 2));...
				reshape(RGB(:, :, 3), 1, size(RGB, 1) * size(RGB, 2))])',...
			size(RGB));

	val = min(XYZ(:));
% 	if (val < 0);
% 		XYZ = XYZ - val;
% 	end;
% end;
