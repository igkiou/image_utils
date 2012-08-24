function sRGB = cube2sRGB(img, varargin)

RGB = cube2RGB(img, varargin{:});
sRGB = RGB2sRGB(RGB);
