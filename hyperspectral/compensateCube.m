function cubeCompensated = compensateCube(cube, compensation_function)

[M N O] = size(cube);
for iter = 1:O,
	cubeCompensated(:, :, iter) = cube(:, :, iter)...
								/ compensation_function(iter);
end;
