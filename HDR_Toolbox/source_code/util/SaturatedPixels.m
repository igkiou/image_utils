function imgOut=SaturatedPixels(img, min, max)
%
%
%        imgOut=SaturatedPixels(img, min, max)
%
%
%        Input:
%           -img: input image with saturated pixels
%           -min: low level for under-exposed values
%           -max: high level for over-exposed values
%
%        Output:
%           -imgOut: a mask with1 over-exposed and under-exposed pixels
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

imgOut=ones(size(img));
imgOut(find(img<min))=0;
imgOut(find(img>max))=0;

end