function XYZ = xyY2XYZ(xyY)

% XYZ = colorspace('xyY->XYZ', xyY);

% my implementation
XYZ = zeros(size(xyY));
XYZ(:, :, 1) = xyY(:, :, 3) ./ xyY(:, :, 2) .* xyY(:, :, 1);
XYZ(:, :, 3) = xyY(:, :, 3) ./ xyY(:, :, 2) .* (1 - xyY(:, :, 1) - xyY(:, :, 2));
XYZ(:, :, 2) = xyY(:, :, 3);
