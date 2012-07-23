function imgOut=ColorCorrection(img, correction)
%
%       imgOut=ColorCorrection(img, correction)
%
%       This function saturates/desaturates colors in img
%
%       input:
%         - img: an image
%	      - correction: the saturation correction's factor.
%                       If correction>1 saturation is increased,
%                       otherwise the image is desaturated.
%
%       output:
%         - imgOut: corrected values
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

if(correction<=0)
    correction = 1.0;
end

L=lum(img);
imgOut = zeros(size(img));
for i=1:3
    imgOut(:,:,i) = ((img(:,:,i)./L).^correction).*L;
end

imgOut = RemoveSpecials(imgOut);

end