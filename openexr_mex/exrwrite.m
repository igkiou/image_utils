function exrwrite(img, a, varargin)
% Properly handle single-dimensional images.

[dim1 dim2 ndims] = size(img);
if (ndims == 1),
	img = repmat(img, [1 1 3]);
	ndims = 3;
end;
imgTrans = single(zeros([dim2 dim1 ndims]));
for iterDim = 1:ndims,
	imgTrans(:, :, iterDim) = img(:, :, iterDim)';
end;

if ((nargin > 1) && (~isempty(a))),
	aTrans = a';
else
	aTrans = [];
end;

exrwrite_sub(imgTrans, aTrans, varargin{:});
