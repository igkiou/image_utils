function outImg = companding(inImg, spec)

if ((nargin < 2) || isempty(spec)),
	spec = 'srgb';
end;

if strcmpi(spec, 'srgb'),
	outImg = zeros(size(inImg));
	inds = (inImg <= 0.0031306684425005883);
	outImg(inds) = 12.92*inImg(inds);
	outImg(~inds) = real(1.055*inImg(~inds).^(1 / 2.4) - 0.055);
elseif strcmpi(spec, 'lstar'),
	epsilon = 216 / 24389;
	kappa = 24389 / 27;
	inds = inImg < epsilon;
	outImg(inds) = inImg(inds) * kappa / 100;
	outImg(~inds) = 1.16 * inImg(~inds) .^ (1 / 3) - 0.16;
elseif isnumeric(spec),
	outImg = inImg .^ (1 / spec);
end;
