function [xyY z] = XYZ2xyY(XYZ)

xyY = colorspace('XYZ->xyY', XYZ);

% my implementation
% xyY = zeros(size(XYZ));
% xyY(:, :, 1) = XYZ(:, :, 1) ./ sum(XYZ, 3);
% xyY(:, :, 2) = XYZ(:, :, 2) ./ sum(XYZ, 3);
% xyY(:, :, 3) = XYZ(:, :, 2);

if (nargout > 1),
	z = XYZ(:, :, 3) ./ sum(XYZ, 3);	
end;
