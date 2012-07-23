function [imgOut, expand_map] = BanterleEO(img, expansion_operator, eo_parameters, bclampingThreshold, bColorRec, gammaRemoval)
%
%       [imgOut, expand_map] = BanterleEO(img, expansion_operator, params, colorRec, gammaRemoval)
%
%
%        Input:
%           -img: input LDR image. There is no assumption about the
%           linearization of the image. If the gamma was encoded using
%           inverse gamma, this should be removed using the paramter
%           gammaRemoval.
%           -expansion_operator: an expansion function which takes as input
%           img and eo_parameters. This outputs the expanded luminance
%           -eo_parameters: the strecthing factor of the 
%           -bclampingThreshold: if it is greater than 0, this value
%           determines the threshold for clamping light sources. Otherwise
%           this parameter is estimated automatically.
%           -bColorRec: a boolean value. If it is set 1 the expand map will
%           be calculated for each color channel
%           -gammaRemoval: the gamma value to be removed if img was encoded
%           using gamme encoding
%
%        Output:
%           -imgOut: inverse tone mapped image
%           -expand_map: the generated expand map
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

check3Color(img);

%Gamma removal
if(gammaRemoval>0.0)
    img=img.^gammaRemoval;
end

if(~exist('expansion_operator'))
    expansion_operator = @InverseReinhard;
end

%Apply a gentle bilateral filter for removing noise
for i=1:3
    img(:,:,i) = bilateralFilter(img(:,:,i),[],0.0,1.0,8.0,0.0125);
end

%Luminance expansion
L = lum(img);
Lexp = expansion_operator(img,eo_parameters);

%Combining expanded and unexpanded luminance channels
expand_map = BanterleExpandMap(img, bColorRec, bclampingThreshold, 0.95, 'gaussian', 1);

LFinal = zeros(size(img));
for i=1:3
    LFinal(:,:,i) = L.*(1-expand_map(:,:,i))+Lexp.*expand_map(:,:,i);
    LFinal(:,:,i) = RemoveSpecials(LFinal(:,:,i));
end

clear('Lexp');

%Generate the final image with the new luminance
imgOut = zeros(size(img));
for i=1:3
    imgOut(:,:,i)=(img(:,:,i).*LFinal(:,:,i))./L;
end
imgOut = RemoveSpecials(imgOut);

end
