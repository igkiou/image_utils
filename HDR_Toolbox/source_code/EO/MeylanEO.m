function imgOut = MeylanEO(img, Meylan_Max, Meylan_lambda, gammaRemoval)
%
%       imgOut = MeylanEO(img, Meylan_Max, Meylan_lambda, gammaRemoval)
%
%
%        Input:
%           -img: input LDR image
%           -Meylan_Max: this value defines the 
%           -Meylan_lambda: threshold for applying the iTMO
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

%default parameters
if(~exist('Meylan_Max')|~exist('Meylan_lambda')|~exist('gammaRemoval'))
    %Maximum value of DR-37P: 3000 cd/m^2 
    Meylan_Max=3000;   
    %standard diffuse part range
    Meylan_lambda=0.67;
    %no gamma removal
    gammaRemoval=-1;
end

%Gamma removal
if(gammaRemoval>0.0)
    img=img.^gammaRemoval;
end

%Luminance channel
l=lum(img);
lmax=max(max(l));

%Filtering with a box filter of size m+1
vS=max(size(l)); %max(width,height)
kSize=round(vS/50)+1;
h = fspecial('average',kSize);
Lfiltered=imfilter(l,h);
t1=max(max(Lfiltered));

%Filtering with a box filter of size 2m+1
kSize=round(vS/25)+1;
h = fspecial('average',kSize);
Lfiltered=imfilter(l,h);
t2=max(max(Lfiltered));

%Thresholding the image luminance channel with threshold t1
mask=zeros(size(l));
indx=find(l>t1);
mask(indx)=1;

%Removing single pixels
mask=bwmorph(mask,'clean');

%n step Erosion and Dilatation
H_iter=[1,1,1;1,0,1;1,1,1];
n=3;
for i=1:n
    %Mask2
    tmpMask2=imfilter(mask,H_iter);
    Mask2=ones(size(l));
    Mask2(find(mask==0))=0;
    Mask2(find(tmpMask2<1))=0;

    %Mask3
    tmpMask2=imfilter(Mask2,H_iter);
    Mask3=zeros(size(l));
    Mask3(find(Mask2==1))=1;
    Mask3(find(Mask2>3))=1;
    Mask3(find(l>t2))=1;

    mask=Mask3;
end

itD=find(mask==0); %Diffuse part
itS=find(mask==1); %Specular part

%Calculation of the curve constants
omega=min(min(l(mask==1)));
s1=Meylan_Max*Meylan_lambda/omega;
s2=Meylan_Max*(1.0-Meylan_lambda)/(lmax-omega);

L=zeros(size(l));
L(itD)=l(itD)*s1; 				%Diffuse expansion
L(itS)=omega*s1+(l(itS)-omega)*s2;	%Specular expansion

%Filtered luminance
h5 = fspecial('average',5);
LFiltered=imfilter(L,h5);

%Smoothing mask
smask=zeros(size(l));
smask(l>omega)=1;

tmpSmask2=imfilter(smask,H_iter);
smask2=smask;
smask2(find(tmpSmask2>1))=1;
smask3=imfilter(smask2,h5);

%The expanded part and its filtered version are blended using the mask 
Lfinal=L.*(1-smask3)+smask3.*LFiltered;

%Removing the old luminance
imgOut=zeros(size(img));
for i=1:3
    imgOut(:,:,i)=img(:,:,i).*Lfinal./l;
end

imgOut=RemoveSpecials(imgOut);

end
