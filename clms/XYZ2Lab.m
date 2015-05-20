function LAB = XYZ2Lab(XYZ, wtpoint)

if ((nargin < 2) || isempty(wtpoint)),
	wtpoint = 'D65';
end;

if (strcmpi(wtpoint, 'd65')),
	LAB = colorspace('XYZ->LAB', XYZ);
	return;
else
	WXYZ = getWhitepoint(wtpoint);
	XYZ(:, :, 1) = XYZ(:, :, 1) / WXYZ(1);
	XYZ(:, :, 2) = XYZ(:, :, 2) / WXYZ(2);
	XYZ(:, :, 3) = XYZ(:, :, 3) / WXYZ(3);
	XYZ = f(XYZ);
	
	LAB = zeros(size(XYZ));
	LAB(:, :, 1) = 116 * XYZ(:, :, 2) - 16;
	LAB(:, :, 2) = 500 * (XYZ(:, :, 1) - XYZ(:, :, 2));
	LAB(:, :, 3) = 200 * (XYZ(:, :, 2) - XYZ(:, :, 3));
	return;
end;

end

function ft = f(t)

epsilon = 216 / 24389; % (6 / 29) ^ 3;
kappa = 24389 / 27;

inds = t > epsilon;
ft = zeros(size(t));
ft(inds) = t(inds) .^ (1 / 3);
ft(~inds) = (kappa * t(~inds) + 16) / 116;

end

% implementation by matlab
% function lab = XYZ2Lab(xyz)
% % Convert XYZ to Lab using a D65 whitepoint.
% 
% % See <http://brucelindbloom.com/index.html?Eqn_XYZ_to_Lab.html> for
% % implementation details and explanation of E and K.
% E = 216/24389;
% K = 24389/27;
% 
% % Whitepoint adjustment.
% xyz(1,:) = xyz(1,:) ./ 0.95047;
% xyz(3,:) = xyz(3,:) ./ 1.08883;
% 
% mask = (xyz > E);
% 
% f = xyz;
% f(mask) = f(mask) .^ (1/3);
% f(~mask) = (K * f(~mask) + 16) ./ 116;
% 
% lab = [116 * f(2,:) - 16;
%        500 * (f(1,:) - f(2,:));
%        200 * (f(2,:) - f(3,:))];
