function im = rawread(inFileName, keepImageFile)

if ((nargin < 2) || isempty(keepImageFile)),
	keepImageFile = true;
end;

outFileName = [inFileName '.tiff'];
system(sprintf('dcraw -D -6 -W -j -T -M -o 0 -g 1 1 -c %s > %s',...
			inFileName, outFileName));
		
im = imread(outFileName);
if (~keepImageFile),
	system(sprintf('rm %s', outFileName));
end;