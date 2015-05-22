function RGB = cube2RGB(cube, varargin)

XYZ = cube2XYZ(cube, varargin{:});
RGB = XYZ2RGB(XYZ);
