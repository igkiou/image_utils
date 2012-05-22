function RGB = sRGB2RGB(sRGB)

RGB = colorspace('sRGB->RGB', sRGB);

% my implementation
% RGB = zeros(size(sRGB));
% indsLinear = (sRGB <= 0.0404482362771076);
% indsNonLinear = ~indsLinear;
% 
% % linear part
% RGB(indsLinear) = sRGB(indsLinear)/ 12.92;
% 
% % nonlinear part
% RGB(indsNonLinear) = real(((sRGB(indsNonLinear) + 0.055) / 1.055) .^ 2.4);
