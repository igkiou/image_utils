function RGB = nuanceCube2RGB(cube, varargin)

XYZ = nuanceCube2XYZ(cube, varargin{:});
RGB = XYZ2RGB(XYZ);
