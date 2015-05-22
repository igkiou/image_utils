function sRGB = msCube2sRGB(cube, varargin)

RGB = msCube2RGB(cube, varargin{:});
sRGB = RGB2sRGB(RGB);
