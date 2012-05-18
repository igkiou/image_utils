function exrwrite(img, a, varargin)

[dim1 dim2 ndims] = size(img);
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
