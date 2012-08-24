function sRGB = nuanceCube2sRGB(img, varargin)

RGB = nuanceCube2RGB(img, varargin{:});
sRGB = RGB2sRGB(RGB);
