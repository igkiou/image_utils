function sRGB = cube2sRGB(cube, varargin)

RGB = cube2RGB(cube, varargin{:});
sRGB = RGB2sRGB(RGB);
