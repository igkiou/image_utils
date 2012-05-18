function rgb = cube2rgb(img, compensation_function)

if (nargin < 2),
	compensation_function = ones(size(img, 3), 1);
end;

l = load('-ascii','CMF_CIE_1931_2_degree.txt');
l = l([13:2:73], [2:4]); %color matching
% l = l / max(l(:));
xyz = zeros(size(img, 1), size(img, 2), 3);
  
for i = 1:3
	for j = 1:size(l,1)
		xyz(:, :, i) = xyz(:, :, i) + l(j, i) * img(:, :, j) / compensation_function(j);
	end;
end;
rgb = xyz2rgb(xyz);
