function RGB = msCube2RGB(img, varargin)

XYZ = msCube2XYZ(img, varargin{:});
RGB = XYZ2RGB(XYZ);
