function imout = imflip(imin)

[M N O] = size(imin);
imout = zeros(M, N, O);
for iterChannel = 1:O,
	imout(:, :, iterChannel) = rot90(rot90(imin(:, :, iterChannel)));
end;
