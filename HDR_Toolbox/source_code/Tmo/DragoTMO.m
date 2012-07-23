function [imgOut,Drago_LMax]=DragoTMO(img, Drago_Ld_Max, Drago_b,Drago_LMax)
%
%
%        imgOut=DragoTMO(img, Drago_Ld_Max, Drago_b)
%
%
%        Input:
%           -img: input HDR image
%           -Drago_Ld_Max: maximum output luminance of the LDR display
%           -Drago_b: bias parameter (0,1]. 
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

%Is it a three color channels image?
check3Color(img);

%Luminance channel
L=lum(img);

if(~exist('Drago_Ld_Max'))
    Drago_Ld_Max=100;
end

if(~exist('Drago_b'))   
    Drago_b=0.95;
end

if(~exist('Drago_LMax'))   
    Drago_LMax=max(max(L));
end
%Max luminance
LMax=Drago_LMax*0.5+0.5*max(max(L));
Drago_LMax = LMax;

constant=log(Drago_b)/log(0.5);
costant2=(Drago_Ld_Max/100)/(log10(1+LMax));

Ld=costant2*log(1+L)./log(2+8*((L/LMax).^constant));

%Removing the old luminance
imgOut=zeros(size(img));
for i=1:3
    imgOut(:,:,i)=img(:,:,i).*Ld./L;
end

imgOut=RemoveSpecials(imgOut);

end
