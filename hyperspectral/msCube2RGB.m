function RGB = msCube2RGB(cube, varargin)

XYZ = msCube2XYZ(cube, varargin{:});
RGB = XYZ2RGB(XYZ);
