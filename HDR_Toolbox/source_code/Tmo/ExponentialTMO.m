function imgOut=ExponentialTMO(img, q_exponential, k_exponential)
%
%       imgOut=ExponentialTMO(img,Exp_scale)   
%
%
%       Input:
%           -img: input HDR image
%           -q_exponential: appearance value (1,+inf)
%           -k_exponential: appearance value (1,+inf)
%
%       Output
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

if(~exist('q_exponential')||~exist('k_exponential'))
    q_exponential=1;
    k_exponential=1;
end

%check for q_exponential>=1
if(q_exponential<1)
    q_exponential=1;
end

%check for q_exponential>=1
if(k_exponential<1)
    k_exponential=1;
end

%Logarithmic mean calculation
Lwa=logMean(L);

%Dynamic Range Reduction
Ld=1-exp(-(L*q_exponential)/(Lwa*k_exponential));

%Removing the old luminance
imgOut=zeros(size(img));
for i=1:3
    imgOut(:,:,i)=img(:,:,i).*Ld./L;
end

imgOut=RemoveSpecials(imgOut);

end