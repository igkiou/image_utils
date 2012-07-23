function [imgOut,La]=YeeTMO(img, nLayer, CMax, Lda)
%
%
%       imgOut=YeeTMO(img, nLayer)
%
%
%       Input:
%           -img: HDR image
%           -nLayer: number of layer
%           -CMax: CMax parameter of Tumblin-Rushmeier TMO
%           -Lda: Lda parameter of Tumblin-Rushmeier TMO
%
%       Output:
%           -imgOut: tone mapped image
%           -La: Adaptation luminance
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

if(~exist('nLayer')|~exist('CMax')|~exist('Lda'))
    nLayer=64;
    CMax=100;
    Lda=80;
end

%calculation of the adaptation
Llog=log10(L+1e-6);
% Removing noise using the bilateral filter
minLLog = min(min(Llog));
maxLLog = max(max(Llog));
Llog = bilateralFilter(Llog,[],minLLog,maxLLog,4,0.02);
LLoge = log(L+2.5*1e-5);
bin_size1=1;
bin_size2=0.5;
La=zeros(size(L));
for i=0:(nLayer-1)
    bin_size=bin_size1+(bin_size2-bin_size1)*i/(nLayer-1);    
    segments=round((Llog-minLLog)/bin_size)+1; 
    %Calculation of layers
    [imgLabel]=CompoCon(segments,8);    
    labels = unique(imgLabel);
    for p=1:length(labels);
        %Group adaptation
        indx=find(imgLabel==labels(p));
        La(indx)=La(indx)+mean(mean(LLoge(indx)));
    end
end
La=exp(La/nLayer);
La(find(La<0))=0;

%Dynamic Range Reduction
imgOut = TumblinRushmeierTMO(img, Lda, CMax, La);

end