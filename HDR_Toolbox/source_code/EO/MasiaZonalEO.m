function imgOut = MasiaZonalEO(img, gammaRemoval)
%
%       imgOut = MasiaEO(img, gammaRemoval)
%
%
%        Input:
%           -img: input LDR image
%           -gammaRemoval: the gamma value to be removed if known
%
%        Output:
%           -imgOut: an expanded image
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

%Is it a three color channels image?

%is it a three color channels image?
check3Color(img);

if(gammaRemoval>0.0)
    img=img.^gammaRemoval;
end

zones = 1:9;
v = 5.25;
phi = 1.0/2.2;

p = (exp(v*sin(pi*(zones-1)/16))-1)/(exp(v)-1);%zone limits

L = lum(img);





end