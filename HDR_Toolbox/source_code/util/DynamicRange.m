function [dyn,dynClassic]=DynamicRange(img)
%
%
%        [dyn,dynClassic]=DynamicRange(img)
%
%
%        Input:
%           -img: the input image
%
%        Output:
%           -dyn: dynamic range in base 10 Logarithm space with roboust
%           maximum and minimum values of the image
%           -dynClassic: dynamic range in base 10 Logarithm space with
%           maximum and minimum values of the image
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

%is it a three color channels image?
check3Color(img);

L=lum(img);

dyn=log10(MaxQuart(L,0.999)/MaxQuart(L,0.001));

dynClassic=log10(max(max(L))/min(min(L)));

end