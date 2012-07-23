function val=Sum1D(imgOut,xMin,xMax,yMin,yMax)
%
%
%        Y=FallOff(r,c)
%
%
%        Input:
%           -(xMin,xMax,yMin,yMax): the bounding box of a region in img
%           -imgOut: an image
%
%        Output:
%           -val: the sum of values (xMin,xMax,yMin,yMax) in imgOut
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

[r,c,col]=size(imgOut(yMin:yMax,xMin:xMax,:));
tmp=reshape(imgOut(yMin:yMax,xMin:xMax,:),r*c,1,col);
val=sum(tmp); 
        
end