function imout = imflip(imin)

imout = imin(end:-1:1, :, :);
% 
% [M N O] = size(imin);
% imout = zeros(M, N, O);
% for iterChannel = 1:O,
% 	imout(:, :, iterChannel) = rot90(rot90(imin(:, :, iterChannel)));
% end;
