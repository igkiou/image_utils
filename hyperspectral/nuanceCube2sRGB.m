function sRGB = nuanceCube2sRGB(cube, varargin)

RGB = nuanceCube2RGB(cube, varargin{:});
sRGB = RGB2sRGB(RGB);
