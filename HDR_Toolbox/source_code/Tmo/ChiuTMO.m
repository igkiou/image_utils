function imgOut=ChiuTMO(img, k, sigma, clamping, glare, glare_n, glare_width)
%
%       imgOut = ChiuTMO(img, k, sigma, clamping, glare, glare_n, glare_width)
%
%
%        Input:
%           -img: input HDR image
%           -k: scale correction
%           -sigma: local window size
%           -clamping: number of iterations for clamping and reducing
%                      halos. If it is negative, the clamping will not be
%                      calculate in the final image.
%           -glare: [0,1]. The default value is 0.8. If it is negative,
%                          the glare effect will not be calculated in the
%                          final image.
%           -glare_n: appearance (1,+Inf]. Default is 8.
%           -glare_width: size of the filter for calculating glare. Default is 121.
%
%        Output:
%           -imgOut: tone mapped image in linear space.
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

%default parameters
if(~exist('k')|~exist('sigma')|~exist('clamping')|~exist('glare')|~exist('glare_n'))
    k=8;
    [r,c,col]=size(img);
    sigma=round(16*max([r,c])/1024)+1;
    clamping=500;
    glare=0.8;
    glare_n=8;
    glare_width=121;
end

%Check parameters
if(k<=0) k=8; end
if(sigma<=0) sigma=round(16*max([r,c])/1024)+1; end
    
%Calculating S
blurred = RemoveSpecials(1./(k*GaussianFilter(L,sigma)));

%Clamping S
if(clamping>0)
    iL=RemoveSpecials(1./L);
    indx=find(blurred>=iL);
    blurred(indx)=iL(indx);

    %Smoothing S
    H2=[0.080,0.113,0.080;...
        0.113,0.227,0.113;...
        0.080,0.113,0.080];

    for i=1:clamping
        blurred = imfilter(blurred,H2,'replicate');
    end
end

%Dynamic range reduction
Ld=L.*blurred;

if(glare>0)
    %Calculation of a kernel with a Square Root shape for simulating glare
    window2=round(glare_width/2);
    [x,y]=meshgrid(-1:1/window2:1,-1:1/window2:1);
    H3=(1-glare)*(abs(sqrt(x.^2+y.^2)-1)).^glare_n;    
    H3(window2+1,window2+1)=0;

    %Circle of confusion of the kernel
    H3(find(sqrt(x.^2+y.^2)>1))=0;

    %Normalization of the kernel
    H3=H3/sum(sum(H3));
    H3(window2+1,window2+1)=glare;
   
    %Filtering
    Ld = imfilter(Ld,H3,'replicate');
end

%Removing the old luminance
imgOut=zeros(size(img));
for i=1:3
    imgOut(:,:,i)=img(:,:,i).*Ld./L;
end

imgOut=RemoveSpecials(imgOut);

end