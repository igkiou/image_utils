function [J multConst addConst] =imnorm(I, bounds)
%IMNORM Normalize gray scale image
%
%   IMNORM(I) shifts and scales the input gray image I so that its gray
%   levels are covering the range [0,1]. 
%
%   IMNORM(I,[MIN,MAX]) shifts and scales the input gray image I so that its gray levels
%   are covering the range [MIN,MAX]. 

if (nargin < 1),
	error('Not enough input arguments.');
elseif (nargin == 1),
	mi=min(I(:));
	ma=max(I(:));
	if ((ma == 0) && (mi == 0)),
		J = I;
		addConst = 0;
		multConst = 1;
	elseif (ma == mi),
		J = ones(size(I));
		addConst = 0;
		multConst = 1 / ma;
	% elseif m<0
	%     J=(I+abs(m))/max(max((I+abs(m))));
	% else
	%     J=(I-abs(m))/max(max((I-abs(m))));
	else
		J=(I-mi) / (ma - mi);
		multConst = 1 / (ma - mi);
		addConst = - mi / (ma - mi);
	end;
elseif (nargin == 2),
	if (length(bounds) ~= 2),
		error('BOUNDS must be a vector of length 2');
% 	elseif(bounds(2) < bounds(1)),
% 		error('BOUNDS(1) must be smaller than BOUNDS(2)');
	end;
	mi=min(I(:));
	ma=max(I(:));
	if ((ma == 0) && (mi == 0)),
		J = I;
		addConst = 0;
		multConst = 1;
	elseif (ma == mi),
		J = bounds(2) * ones(size(I));
		addConst = 0;
		multConst = bounds(2) / ma;
	else
		J = (bounds(2) - bounds(1)) / (ma - mi) * I + (bounds(1) * ma - bounds(2) * mi) / (ma - mi);
		multConst = (bounds(2) - bounds(1)) / (ma - mi);
		addConst = (bounds(1) * ma - bounds(2) * mi) / (ma - mi);
	end;
end;
