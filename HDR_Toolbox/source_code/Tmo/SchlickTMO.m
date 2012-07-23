function imgOut=SchlickTMO(img, schlick_mode, schlick_p, schlick_bit, schlick_dL0, schlick_k)
%
%       imgOut=SchlickTMO(img, schlick_mode, schlick_p, schlick_bit,schlick_dL0)
%
%
%       Input:
%           -img: input HDR image
%           -schlick_p: in [1,+inf]
%           -schlick_bit: number of bit for the quantisation
%           -schlick_dL0: 
%           -schlick_k: in [0,1]
%           -Mode = { 'standard', 'calib', 'nonuniform' }
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

if(~exist('schlick_mode')|~exist('schlick_p')|~exist('schlick_bit')|~exist('schlick_dL0')|~exist('schlick_k'))
    schlick_mode='standard';
    schlick_p=1/0.005;
end

%Luminance channel
L=lum(img);

%Max Luminance value 
LMax=max(max(L));

%Min Luminance value 
LMin=min(min(L));
if(LMin<=0.0)
     ind=find(LMin>0.0);
     LMin=min(min(L(ind)));
end

%Mode selection
switch schlick_mode
    case 'standard'
        p=schlick_p;        
        if(p<1)
            p=1;
        end
        
    case 'calib'
        p=schlick_dL0*LMax/(2^schlick_bit*LMin);
        
    case 'nonuniform'
        p=schlick_dL0*LMax/(2^schlick_bit*LMin);
        p=p*(1-schlick_k+schlick_k*L/sqrt(LMax*LMin));
end

%Dynamic Range Reduction
Ld=p.*L./((p-1).*L+LMax);

%Removing the old luminance
imgOut=zeros(size(img));
for i=1:3
    imgOut(:,:,i)=(img(:,:,i).*Ld)./L;
end
imgOut=RemoveSpecials(imgOut);

end