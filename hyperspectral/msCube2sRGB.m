function sRGB = msCube2sRGB(img, varargin)

RGB = msCube2RGB(img, varargin{:});
sRGB = RGB2sRGB(RGB);
