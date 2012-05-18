function imOut = tonemap_clipping(imIn, type, value, gammaCorrection)

if ((nargin < 2) || isempty(type)),
	type = 'percentile';
end;

if ((nargin < 3) || isempty(value)),
	value = 99.85;
end;

if ((nargin < 4) || isempty(gammaCorrection)),
	gammaCorrection = 1;
end;

if (strcmpi(type, 'percentile')),
	clipLimit = prctile(vec(imIn), value);
elseif (strcmpi(type, 'absolute')),
	clipLimit = value;
elseif (strcmpi(type, 'absolute_bi') || strcmpi(type, 'percentile_bi')),
	if (numel(value) ~= 2),
		error('When clipping at both low and high ends, VALUE must have 2 elements.');
	end;
	if (strcmpi(type, 'percentile_bi')),
		clipLimit = [prctile(vec(imIn), 100 - value(1)) prctile(vec(imIn), value(2))];
	elseif (strcmpi(type, 'absolute_bi')),
		clipLimit = value;
	end;
end;

if (numel(clipLimit) == 1),
	indsHigh = imIn > clipLimit;
	imOut = zeros(size(imIn));
	imOut(~indsHigh) = imIn(~indsHigh);
	imOut(indsHigh) = clipLimit;
elseif (numel(clipLimit) == 2),
	indsLow = imIn < clipLimit(1);
	indsHigh = imIn > clipLimit(2);
	imOut = zeros(size(imIn));
	imOut(indsLow) = clipLimit(1);
	imOut(indsHigh) = clipLimit(2);
	imOut(~(indsLow | indsHigh)) = imIn(~(indsLow | indsHigh));
end;

if (ischar(gammaCorrection)),
	imOut = feval(gammaCorrection, imOut);
elseif (gammaCorrection ~= 1),
	imOut = imOut .^ gammaCorrection;
end;
