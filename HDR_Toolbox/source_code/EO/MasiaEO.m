function imgOut = MasiaEO(img, gammaRemoval)
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

%is it a three color channels image?
check3Color(img);

if(gammaRemoval>0.0)
    img=img.^gammaRemoval;
end

%Calculate luminance
L=lum(img);

%Calculate image statistics
Lav=logMean(L);
[r,c]=size(L);
maxL = MaxQuart(L,0.99);
minL = MaxQuart(L(find(L>0)),0.01);

imageKey = (log(Lav) - log(minL))/(log(maxL) - log(minL));

%Calculate the gamma correction value
a_var = 10.44;
b_var = -6.282;

gamma_cor = imageKey*a_var + b_var;

imgOut = zeros(size(img));
for i=1:3
    imgOut(:,:,i) = (img(:,:,i)./L).*(L.^gamma_cor);
end
imgOut = RemoveSpecials(imgOut);
end