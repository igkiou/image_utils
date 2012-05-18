function sRGB = RGB2sRGB(RGB)

sRGB = zeros(size(RGB));
indsLinear = (RGB <= 0.0031306684425005883);
indsNonLinear = ~indsLinear;

% linear part
sRGB(indsLinear) = 12.92 * RGB(indsLinear);

% nonlinear part
sRGB(indsNonLinear) = real(1.055 * RGB(indsNonLinear) .^ (1.0 / 2.4) - 0.055);
