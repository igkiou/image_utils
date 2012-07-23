function imgOut = LischinskiTMO(img, LSC_alpha)
%
%
%      imgOut=LischinskiTMO(img)
%
%
%       Input:
%           -img: input HDR image
%           -alpha: starting exposure for tone mapping
%
%       Output:
%           -imgOut: output tone mapped image in linear domain
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

%Is alpha defined?
if(~exist('LSC_alpha')) LSC_alpha=0.5; end

%Number of zones in the image
maxL = max(max(L));
minL = min(min(L));
epsilon = 1e-6;
minLLog = log2(minL+epsilon);
Z = ceil ( log2(maxL) - minLLog);

%Chose the representative Rz for each zone
fstopMap = zeros(size(L));
Lav = logMean(L);
for i=1:Z
    indx = find(L>=2^(i+minLLog)&L<2^(minLLog+i+1));
    if(~isempty(indx))
        Rz = median(L(indx));
        %photographic operator
        Rz = (LSC_alpha * Rz) / Lav;
        f = Rz/(Rz+1);    
        fstopMap(indx) = log2(f/Rz);
    end
end

%Minimization process
fstopMap = LischinskiMinimization(log2(L+epsilon), fstopMap, 0.07*ones(size(L)));
hdrimwrite(fstopMap,'LC_after_min.pfm');

imgOut = zeros(size(img));
for i=1:3
    imgOut(:,:,i) = img(:,:,i).*2.^fstopMap;
end

end