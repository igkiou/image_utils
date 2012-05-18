function rgb = cube2rgb2(img, compensation_function)

if (nargin < 2),
	compensation_function = ones(size(img, 3), 1);
end;

l = load('-ascii','CMF_CIE_1931_2_degree.txt');
l = l([9:2:69], [1:4]); %color matching
l = l(:, 2:4);
l = l / max(l(:));
xyz1 = zeros(size(img, 1), size(img, 2), 3);
  
for i = 1:3
	for j = 1:size(l,1)
		xyz1(:, :, i) = xyz1(:, :, i) + l(j, i) * img(:, :, j) / compensation_function(j);
	end;
end;
rgb = xyz2rgb(xyz1);

% if (any(rgb(:) < 0)),
% 	rgb = rgb - min(rgb(:));
% end;

%   rgb = rgb .^ (1 / 4);
