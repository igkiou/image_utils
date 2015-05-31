function outImg = invCompanding(inImg, spec)

if ((nargin < 2) || isempty(spec)),
	spec = 'srgb';
end;

if strcmpi(spec, 'srgb'),
	outImg = zeros(size(inImg));
	inds = (inImg <= 0.0404482362771076);
	outImg(inds) = inImg(inds) / 12.92;
	outImg(~inds) = real(((inImg(~inds) + 0.055) / 1.055) .^ 2.4);
elseif strcmpi(spec, 'lstar'),
	epsilon = 216 / 24389;
	kappa = 24389 / 27;
	inds = inImg < epsilon * kappa / 100;
	outImg(inds) = inImg(inds) * 100 / kappa;
	outImg(~inds) = ((inImg(~inds) + 0.16) / 1.16) .^ 3;
elseif isnumeric(spec),
	outImg = inImg .^ spec;
end;