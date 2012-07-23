function [imgOut,samples]=ImportanceSampling(img, falloff, nSamples)
%
%
%        [imgOut,samples]=ImportanceSampling(img, falloff, nSamples)
%
%
%        Input:
%           -img: an environment map in the latitude-longitude mapping
%           -nSamples: the number of samples to generate
%           -falloff: a flag. If it is set 1, it means that fall-off will
%                     be taken into account
%
%        Output:
%           -imgOut: an image with sampled points (only where they are
%           placed)
%           -samples: a list of the sampled points of img
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

%falloff compensation
if(falloff)
    img=FallOffEnvMap(img);
end

%Luminance channel
L = lum(img);
[r,c]=size(L);

%Creation of 1D distributions for sampling
cDistr=[];
values=zeros(c,1);
for i=1:c
    %1D Distribution
    tmpDistr = Create1DDistribution(L(:,i));
    cDistr=[cDistr,tmpDistr];
    values(i)=tmpDistr.maxCDF;
end
rDistr = Create1DDistribution(values);

%Sampling
samples = [];
imgOut = zeros(size(L));
pi22 = 2*pi^2;
for i=1:nSamples
    %random values in [0,1]
    u=rand(2,1);
    
    %sampling the rDistr
    [val1,pdf1]=Sampling1DDistribution(rDistr,u(1));

    %sampling the cDistr
    [val2,pdf2]=Sampling1DDistribution(cDistr(val1),u(2));
    
    phi = pi*2*val1/c;
    theta = pi*val2/r;
    vec=PolarVec3(theta, phi);
    pdf = (pdf1*pdf2)/(pi22*abs(sin(theta)));
    samples = [samples,struct('dir',vec,'color',img(val2,val1,:),'pdf',pdf)];
    
    imgOut(val2,val1)=imgOut(val2,val1)+1;

end

end