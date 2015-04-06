function Irgb = indexed2rgb(I, rgb)

If = floor(I * 256);
If(If == 256) = 255;
Ir = rgb(If + 1, 1);
Ig = rgb(If + 1, 2);
Ib = rgb(If + 1, 3);
Irgb = cat(3, Ir, Ig, Ib);
Irgb = reshape(Irgb, [size(If) 3]);