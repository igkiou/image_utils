function imgOut = KrawczykTMO(img)
%
%
%       imgOut = KrawczykTMO(img)
%
%
%       Input:
%          -img: input HDR image
%
%       Output:
%          -imgOut: tone mapped image
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

%Calculate the histrogram of the HDR image in Log10 space
[histo,bound,haverage]=HistogramHDR(img,256,'log10',0);

%Determine how many K clusters (number of zones)
C=bound(1):1:bound(2);
K=length(C);
if(C(K)<bound(2))
    C=[C,bound(2)];
    K=length(C);
end

%Calculation of luminance and log luminance
delta=1e-6;
L=lum(img);
LLog10=log10(L+delta);

%Init K-means
totPixels=zeros(size(C));
oldK=K;
oldC=C;
iter=100; %mximum number of iterations
histoValue=(bound(2)-bound(1))*(0:(length(histo)-1))/(length(histo)-1)+bound(1);
histoValue=histoValue';

%K-means loop
for p=1:iter
    belongC=-ones(size(histo));
    distance=100*oldK*ones(size(histo));
    %Calculate the distance of each bin in the histogram from centroids C
    for i=1:K
        tmpDistance=abs(C(i)-histoValue);
        tmpDistance=min(tmpDistance,distance);
        indx=find(tmpDistance<distance);
        if(~isempty(indx))
            belongC(indx)=i;
            distance=tmpDistance;
        end
    end

    %Calculate the new centroids C
    C=zeros(size(C));
    totPixels=zeros(size(C));
    full=zeros(size(C));
    for i=1:K
        indx=find(belongC==i);
        if(~isempty(indx))
            full(i)=1;
            totHisto=sum(histo(indx));
            totPixels(i)=totHisto;
            C(i)=sum((histoValue(indx).*histo(indx))/totHisto);
        end
    end
    
    %Remove empty frameworks
    C=C(find(full==1));
    totPixels=totPixels(find(full==1));
    K=length(C);
    
    %is a fix point reached?
    if(K==oldK)
        if(sum(oldC-C)<=0)
            break
        end
    end
    oldC=C;
    oldK=K;
end

%Merging frameworks
iter=K*10;
for p=1:iter
    for i=1:(K-1)
        %Distance between frameworks has to be <= than 1
        if(abs(C(i)-C(i+1))<1)
            tmp=totPixels(i)+totPixels(i+1);
            C(i)=(C(i)*totPixels(i)+C(i+1)*totPixels(i+1))/tmp;
            totPixels(i)=tmp;

            %Removing not necessary frameworks
            C(i+1)=[];
            totPixels(i+1)=[];          
            K=length(C);
            break
        end
    end
end

%Calculating the minimum distance 
%(sigma) between frameworks
sigma=100*K;
for i=1:K
    for j=(i+1):K
        sigma=min(sigma,abs(C(i)-C(j)));
    end
end

%Partitioning in frameworks
framework=-ones(size(L));
distance=100*K*ones(size(L));
for i=1:K
    tmpDistance=abs(C(i)-LLog10);
    tmpDistance=min(distance,tmpDistance);
    indx=find(tmpDistance<distance);
    if(~isempty(indx))
        %assign the right framework
        framework(indx)=i;

        %updating distance
        distance=tmpDistance;
    end
end

%Normalization
sigma2=2*sigma^2;
tot=zeros(size(L));
A=zeros(K,1);
sigmaArticulation2=2*0.33^2;
for i=1:K
    %Articulatin of the framework
    indx=find(framework==i);
    maxY=max(LLog10(indx));
    minY=min(LLog10(indx));
    A(i)=1-exp(-(maxY-minY)^2/sigmaArticulation2);
    %The sum of Probability Maps for normalisation
    tot=tot+exp(-(C(i)-LLog10).^2/sigma2)*A(i);
end

%Calculating probability maps
Y=LLog10;
[height,width,col] = size(img);
for i=1:K
    indx=find(framework==i);
    if(~isempty(indx))
        %Probability map
        P=exp(-(C(i)-LLog10).^2/sigma2);
        P=RemoveSpecials(P./tot);
        P = bilateralFilter(P,[],0,1,min([height,width])/2,0.4);
        figure(i)
        imshow(P);
        %Anchoring
        W=MaxQuart(LLog10(indx),0.95);
        Y=Y-W*A(i)*P;
    end
end

%Clamp in the range [-2,0]
Ld=ClampImg(Y,-2,0);

%Remap values in [0,1]
Ld=(10.^(Ld+2))/100;

%Removing the old luminance
imgOut=zeros(size(img));
for i=1:3
    imgOut(:,:,i)=img(:,:,i).*Ld./L;
end

imgOut=RemoveSpecials(imgOut);

end