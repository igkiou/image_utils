function imOut = imtranspose(imIn)

[M N O] = size(imIn);
imOut = zeros(N, M, O);
for iter = 1:O,
	imOut(:, :, iter) = imIn(:, :, iter)';
end;
