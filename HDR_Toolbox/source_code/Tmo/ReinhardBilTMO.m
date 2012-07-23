function [imgOut,pAlpha,pWhite]=ReinhardBilTMO(img, pAlpha, pWhite, pLocal)
%
%
%      imgOut=ReinhardBilTMO(img, pAlpha, pWhite, pLocal)
%
%
%       Input:
%           -img: input HDR image
%           -pAlpha: value of exposure of the image
%           -pWhite: the white point 
%           -pLocal: boolean value. If it is true a local version is used
%                   otherwise a global version.
%
%       Output:
%           -imgOut: output tone mapped image in linear domain
%           -pAlpha: as in input
%           -pLocal: as in input
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

if(~exist('pWhite')||~exist('pAlpha')||~exist('pLocal'))
    pWhite=2*max(max(L));
    pAlpha=0.15;
    pLocal=0;
end

%Logarithmic mean calcultaion
Lwa=logMean(L);

%Scale luminance using alpha and logarithmic mean
L=(pAlpha*L)/Lwa;

%Local calculation?
if(pLocal)
    sMax = 9;     
    alpha1 = 1/(2*sqrt(2));
    alpha2 = 1.6^sMax;    
    L_adapt = bilateralFilter(L,[],min(min(L)),max(max(L)),alpha2,alpha1);
end

pWhite2=pWhite*pWhite;

%Range compression
if(pLocal)
    Ld=(L.*(1+L/pWhite2))./(1+L_adapt);
else
    Ld=(L.*(1+L/pWhite2))./(1+L);
end

%Removing the old luminance
imgOut=zeros(size(img));
for i=1:3
    imgOut(:,:,i)=img(:,:,i)./lum(img).*Ld;
end

imgOut=RemoveSpecials(imgOut);

end