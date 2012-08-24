function RGB = nuanceCube2RGB(img, varargin)

XYZ = nuanceCube2XYZ(img, varargin{:});
RGB = XYZ2RGB(XYZ);
