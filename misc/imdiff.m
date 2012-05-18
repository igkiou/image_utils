function val = imdiff(varargin)
%IMDIFF Difference between intensity images.    
%   I = IMDIFF(IMAGE1,IMAGE2) returns I, a scalar value representing the 
%   2-norm of the pixel-by-pixel absolute difference between two intensity 
%   images, I = NORM(ABS(IMAGE1 - IMAGE2)).
%
%   I = IMDIFF(IMAGE1,IMAGE2,OPTION1,VALUE1,...) returns different 
%   quantities, depending on the specified options.
%
%   - Possible options
%
%       'normalized'	Specify whether to return normalized difference,
%						I = NORM(ABS(IMAGE1 - IMAGE2)) / NORM(IMAGE2).
%						Possible values: 1 (normalized), 0 (non-normalized,
%						default).
%
%       'normSelection' Specifies which vector norm to be used. Arguments
%						are same as for NORM. Default method is regular 2-norm.
%
%   IMAGE1 and IMAGE2 can be multidimensional images of the same dimensions.
%   IMAGE1 and IMAGE2 must be of the same size in every dimension. Images 
%   are converted to vectors, for the purposes of norm calculation. 
%  
%   Class Support
%   -------------  
%   IMAGE1 and IMAGE2 must be logical, uint8, uint16, or double, and must be
%   of the same type, real, nonempty, and nonsparse. I is double.
%  
%   See also NORM, IMABSDIFF, IM2DOUBLE.

[image1 image2 normalized normSelection] = ParseInputs(varargin{:});

diff = double(imabsdiff(image1, image2));
val = norm(diff(:), normSelection);
if (normalized == 1),
	val = val / norm(image2(:), normSelection);
end;
  
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [Image1 Image2 normalized normSelection] = ParseInputs(varargin)

iptchecknargin(2,6,nargin,mfilename);

iptcheckinput(varargin{1},{'uint8','uint16', 'double', 'logical'},...
              {'real', 'nonempty', 'nonsparse'},mfilename, 'IMAGE1',1);

iptcheckinput(varargin{2},{'uint8','uint16', 'double', 'logical'},...
              {'real', 'nonempty', 'nonsparse'},mfilename, 'IMAGE2',2);

Image1 = varargin{1};
Image2 = varargin{2};
normalized = 0;
normSelection = 2;

eid = sprintf('%s:invalidImages',mfilename);

if (~isempty(find(size(Image1) ~= size(Image2), 1))),
	msg = 'This function expects IMAGE1 and IMAGE2 to be of the same size.';
	error(eid,'%s',msg);
end;

if (~strcmp(class(Image1),class(Image2))),
	msg = 'This function expects IMAGE1 and IMAGE2 to be of the same class.';
	error(eid,'%s',msg);
end;

eid = sprintf('%s:invalidOptions',mfilename);

for argCount = 3:2:length(varargin)
	if((~strcmp(varargin{argCount},'normSelection'))...
			&& (~strcmp(varargin{argCount},'normalized')))	
		msg = 'Invalid option.';
		error(eid,'%s',msg);
	end;
	eval(sprintf('%s = varargin{argCount + 1};',varargin{argCount}));
end;

eid = sprintf('%s:invalidOptionValues',mfilename);

if ((strcmp(class(normSelection),'double')) && (normSelection ~= 1)...
		&& (normSelection ~= 2)	&& (~isinf(normSelection)))...
		|| ((ischar(normSelection)) && (~strcmp(normSelection,'fro')))
	msg = 'Invalid norm selection.';
	error(eid,'%s',msg);
end;

if ((normalized ~= 1) && (normalized ~= 0)),
	msg = 'Invalid normalized selection.';
	error(eid,'%s',msg);
end;
