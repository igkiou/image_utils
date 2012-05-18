function Inorm = normalize_rgb(I)

if (size(I, 3) == 1),
	Inorm = I;
elseif (size(I, 3) == 3),
	Inorm = I ./ repmat(sqrt(sum(I.^2, 3)), [1 1 3]);
	Inorm(isnan(Inorm)) = 0;
else
	error('Problem with dimensions.');
end;
