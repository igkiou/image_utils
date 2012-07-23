function imgOut=NormalizeTMO(img)
%
%        NormalizeTMO(img)
%
%       
%        Simple TMO, which divides an image by the maximum
%
%        Input:
%           -img: input HDR image
%
%        Output:
%           -imgOut: tone mapped image
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

%is it a three color channels image?
check3Color(img);

%Luminance channel
L=lum(img);

%Roboust max
MaxQ95=MaxQuart(L,0.999);

imgOut=img/MaxQ95;

end