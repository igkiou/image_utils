function imgOut=WardHistAdjTMO(img,nBin)
%
%        imgOut=WardHistAdjTMO(img,nBin)
%
%
%        Input:
%           -img: input HDR image
%           -nBin: number of bins for calculating the histogram (1,+Inf)
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

if(~exist('nBin'))
    nBin=256;
end

if(nBin<1)
    nBin=256;
end

%The image is downsampled
[n,m]=size(L);
maxCoord = max([n,m]);
viewAngleWidth = 2*atan(m/(2*maxCoord*0.75));
viewAngleHeight = 2*atan(n/(2*maxCoord*0.75));
fScaleX = (2*tan(viewAngleWidth/2)/0.01745);
fScaleY = (2*tan(viewAngleHeight/2)/0.01745);

L2=imresize(L,[round(fScaleY), round(fScaleX)],'bilinear');
LMax=max(max(L2));
LMin=min(min(L2));

if(LMin<=0.0)
     LMin=min(L2(find(L2>0.0)));
end

%Log space
Llog=log(L2);
LlMax=log(LMax);
LlMin=log(LMin);

%Display characteristics in cd/m^2
LdMax=100;    LldMax=log(LdMax);
LdMin=1;      LldMin=log(LdMin);

%function P
p=zeros(nBin,1);
delta=(LlMax-LlMin)/nBin;
for i=1:nBin
    indx=find(Llog>(delta*(i-1)+LlMin)&Llog<=(delta*i+LlMin));
    p(i)=numel(indx);
end

%Histogram ceiling   
p=histogram_ceiling(p,delta/(LldMax-LldMin));
plot(p);

%Calculation of P(x) 
Pcum=cumsum(p);
Pcum=Pcum/max(Pcum);

%Calculate tone mapped luminance
x=(LlMin:(LlMax-LlMin)/(nBin-1):LlMax)';
pps=spline(x,Pcum);      
Ld=exp(LldMin+(LldMax-LldMin)*ppval(pps,log(L)));
Ld=(Ld-LdMin)/(LdMax-LdMin);
%Removing the old luminance
imgOut=zeros(size(img));
for i=1:3
    imgOut(:,:,i)=img(:,:,i).*Ld./L;
end
imgOut=RemoveSpecials(imgOut);

end