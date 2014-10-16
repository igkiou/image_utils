function im = imblur(im, patchSize)

im = convn(convn(im, ones(patchSize, 1, 1), 'valid'),...
			ones(1, patchSize, 1) / patchSize ^ 2, 'valid');
