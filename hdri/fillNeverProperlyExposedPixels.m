function hdr = fillNeverProperlyExposedPixels(hdr, someUnderExposed, someOverExposed, someProperlyExposed)

hdr(someOverExposed & ~someUnderExposed & ~someProperlyExposed) = max(hdr(someProperlyExposed));
hdr(someUnderExposed & ~someOverExposed & ~someProperlyExposed) = min(hdr(someProperlyExposed));
fillMask = imdilate(someUnderExposed & someOverExposed & ~someProperlyExposed, ones(3, 3));
if any(fillMask(:))
	warning('There exist pixels that were never properly exposed, and not always underexposed or always overexposed.');
	hdr(:, :, 1) = roifill(hdr(:, :, 1), fillMask(:, :, 1));
	hdr(:, :, 2) = roifill(hdr(:, :, 2), fillMask(:, :, 2));
	hdr(:, :, 3) = roifill(hdr(:, :, 3), fillMask(:, :, 3));
end;
