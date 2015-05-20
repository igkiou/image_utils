function im = rawread_system(inFileName, dcrawFlags, keepImageFile)

if ((nargin < 2) || isempty(dcrawFlags)),
	dcrawFlags = '-D -6 -W -j -M -o 0 -g 1 1';
end;

if ((nargin < 3) || isempty(keepImageFile)),
	keepImageFile = true;
end;

outFileName = [inFileName '.tiff'];
system(sprintf('dcraw %s -T -c %s > %s', dcrawFlags, inFileName, outFileName));
		
im = imread(outFileName);
if (~keepImageFile),
	system(sprintf('rm %s', outFileName));
end;