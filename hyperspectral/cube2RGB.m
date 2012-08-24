function RGB = cube2RGB(img, varargin)

XYZ = cube2XYZ(img, varargin{:});
RGB = XYZ2RGB(XYZ);
