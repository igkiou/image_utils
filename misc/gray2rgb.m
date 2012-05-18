function RGBImg = gray2rgb(Img, colormap, scale)

if (nargin < 3),
	scale = 1;
end;

Img = Img(:, :, 1);
vals = unique(Img);
numVals = length(vals);
tempImg = zeros(size(Img));
for iter = 1:numVals,
	tempImg(Img == vals(iter)) = scale * iter;
end;
eval(sprintf('mapVec = %s(%d);', colormap, numVals * scale));
RGBImg = ind2rgb(tempImg, mapVec);
