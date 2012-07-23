function PNGHDREnc(img, nameHDR, namePNG)
%
%
%       PNGHDREnc(img, name)
%
%
%       Input:
%           -img:   HDR image
%
%           -nameHDR: is the name of the HDR image if img is empty
%           -namePNG: is the name of the output PNG image
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

%remove the extension of the file
if(isempty(img))
    img = hdrread(nameHDR);
end

gamma = 2.0;
invGamma = 1.0/gamma;

%Tone mapping using Reinhard's operator
L=0.213*img(:,:,1)+0.715*img(:,:,2)+0.072*img(:,:,3);
Lwa=exp(mean(mean(log(L+1e-6))));
La = L*0.18/Lwa;
Ld = La./(La+1);

imgTMO = zeros(size(img));
for i=1:3
    imgTMO(:,:,i)=(img(:,:,i)./L).^0.2;
    imgTMO(:,:,i) = imgTMO(:,:,i).*Ld;
end
imgTMO(find(L<=0))=0;

hdrimwrite(imgTMO,'test.pfm');

%maxTMo
[n,m]=size(imgTMO);
matrix=sort(reshape(imgTMO,n*m,1));
maxTMO=matrix(round(n*m*0.999));

%Clamping
imgTMO = imgTMO/maxTMO;
imgTMO(find(imgTMO>1))=1;
imgTMO(find(imgTMO<0))=0;

%Ratio
RI = L./Ld;
RI(find(Ld<=0))=0;
RIenc = log2(RI+2^-16);
RIenc(find(RIenc>16))=16;
RIenc(find(RIenc<-16))=-16;
RIenc = (RIenc+16)/32;

imwrite(imgTMO.^invGamma,namePNG,'Alpha',RIenc.^invGamma,'Gamma',invGamma,'Comment',['Created using Banterle PNG-HDR: maxTMO: ',num2str(maxTMO)]);
end