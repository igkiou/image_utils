function Rp = gammacorrection(R)
Rp = zeros(size(R));
i = (R <= 0.0031306684425005883);
Rp(i) = 12.92*R(i);
Rp(~i) = real(1.055*R(~i).^0.416666666666666667 - 0.055);

%%outImg = zeros(size(inImg));
%%indsLinear = (inImg <= 0.0031306684425005883);
%%indsNonLinear = ~indsLinear;

% linear part
%%outImg(indsLinear) = 12.92 * inImg(indsLinear);

% nonlinear part
%%outImg(indsNonLinear) = real(1.055 * inImg(indsNonLinear) .^ (1.0 / 2.4) - 0.055);

% what mitsuba does (same as here)
% inline Float toSRGBComponent(Float value) {
% 	if (value <= (Float) 0.0031308)
% 		return (Float) 12.92 * value;
% 	return (Float) (1.0 + 0.055)
% 		* std::pow(value, (Float) (1.0/2.4))
% 		 - (Float) 0.055;
% }
