function imgOut = ConvertRGBXYZ(img, inverse)
%
%       imgOut = ConvertRGBXYZ(img, inverse)
%
%
%        Input:
%           -img: image to convert from RGB to XYZ or from XYZ to RGB.
%           -inverse: takes as values 0 or 1. If it is set to 1 the
%                     transformation from XYZ to RGB is applied, otherwise
%                     the transformation from RGB to XYZ.
%
%        Output:
%           -imgOut: converted image in XYZ or RGB.
%
%     Copyright (C) 2011  Francesco Banterle
% 
%     This program is free software: you can redistribute it and/or modify
%     it under the terms of the GNU General Public License as published by
%     the Free Software Foundation, either version 3 of the License, or
%     (at your option) any later version.
% 
%     This program is distributed in the hope that it will be useful,
%     but WITHOUT ANY WARRANTY; without even the implied warranty of
%     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%     GNU General Public License for more details.
% 
%     You should have received a copy of the GNU General Public License
%     along with this program.  If not, see <http://www.gnu.org/licenses/>.
%

%ITU-R BT.709 matrix conversion from RGB to XYZ
mtxRGB2XYZ = [  0.4124, 0.3576, 0.1805;...
                0.2126, 0.7152, 0.0722;...
                0.0193, 0.1192, 0.9505];

if(inverse==0)
    imgOut = ConvertLinearSpace(img, mtxRGB2XYZ);
else

if(inverse==1)
    imgOut = ConvertLinearSpace(img, inv(mtxRGB2XYZ));
end
            
end