function imgOut=AshikhminTMO(img, LdMax, pLocal)
%
%
%      imgOut=AshikhminTMO(img, LdMax, pLocal)
%
%
%       Input:
%           -img: input HDR image
%           -LdMax: maximum output luminance of the LDR display
%           -pLocal: boolean value. If it is true a local version is used
%                   otherwise a global version.
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

if(~exist('pLocal'))
    pLocal=1;
end

if(~exist('LdMax'))
    LdMax=100;
end

%Local calculation?
if(pLocal)
    %precompute 10 filtered images
    sMax=10;    %sMax should be one degree of visual angle, the value is set as in the original paper
    [r,c]=size(L);
    Lfiltered=zeros(r,c,sMax); %filtered images
    LC=zeros(r,c,sMax);
    for i=1:sMax
       Lfiltered(:,:,i)=GaussianFilterWindow(L,i+1); 
       %normalized difference of Gaussian levels
       LC(:,:,i)=RemoveSpecials(abs(Lfiltered(:,:,i)-GaussianFilterWindow(L,(i+1)*2))./Lfiltered(:,:,i)); 
    end  
    
    %threshold is a constant for solving the band-limited local contrast LC at a given
    %image location. This is kept as in the original paper
    threshold=0.5;
    
    %adaptation image
    L_adapt=-ones(size(L));
    for i=1:sMax
        ind=find(LC(:,:,i)<threshold);
        L_adapt(ind)=Lfiltered(r*c*(i-1)+ind);
    end
    
    %set the maximum level
    ind=find(L_adapt<0);
    L_adapt(ind)=Lfiltered(r*c*(sMax-1)+ind);
    
    %Remove the detail layer
    Ldetail=RemoveSpecials(L./L_adapt);
    L=L_adapt;
end

%Roboust maximum and minimum
maxL=MaxQuart(L,0.9995);
minL=MaxQuart(L,0.0005);

%Range compression
maxL_TVI=TVI_Ashikhmin(maxL);
minL_TVI=TVI_Ashikhmin(minL);

Ld=LdMax*(TVI_Ashikhmin(L)-minL_TVI)/(maxL_TVI-minL_TVI);

%Local Recombination
if(pLocal)
    Ld=Ld.*Ldetail;
end

%Removing the old luminance
imgOut=zeros(size(img));
for i=1:3
    imgOut(:,:,i)=img(:,:,i)./lum(img).*Ld;
end

imgOut=RemoveSpecials(imgOut);

end