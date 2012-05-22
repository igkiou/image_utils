function [key burn] = getParams_photographic(imIn, mask)

if ((nargin < 2) || isempty(mask)),
	mask = true(size(imIn, 1), size(imIn, 2));
end;

mask = repmat(mask, [1 1 size(imIn, 3)]);
vals = imIn(mask);
inds = ((vals > prctile(vals, 1)) & (vals < prctile(vals, 99)));
vals = vals(inds);
log2Lmax = log2(max(vals));
log2Lmin = log2(min(vals));
log2Lw = log2(meanlogv(vals));

key = 0.18 * 2 ^ ((2 * log2Lw - log2Lmin - log2Lmax) / (log2Lmax - log2Lmin));
burn = 1.5 * 2 ^ (log2Lmax - log2Lmin - 5) / max(vals);
