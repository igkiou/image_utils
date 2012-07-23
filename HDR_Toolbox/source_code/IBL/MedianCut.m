function [imgOut,lights]=MedianCut(img,nlights,falloff)
%
%
%        lights=MedianCut(img,nlights,falloff)
%
%
%        Input:
%           -img: an environment map in the latitude-longitude mapping
%           -nlights: the number of samples to generate
%           -falloff: a flag. If it is set 1, it means that fall-off will
%                     be taken into account
%
%        Output:
%           -imgOut: an image with sampled points
%           -lights: a list of directional lights
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

global L;
global imgWork;
global limitSize;
global nLights;
global lights;

%falloff compensation
if(falloff)
    img=FallOffEnvMap(img);
end

%Global variables initialization
L=lum(img);
imgWork=img;
nLights=round(log2(nlights));

[r,c]=size(L);
limitSize=2;%limitSize=max([c,r])/2^nluce;

lights=[];

if(c>r)
    MedianCutAux(1,c,1,r,0,1);
else
    MedianCutAux(1,c,1,r,0,0);
end

imgOut = GenerateLightMap(lights,c,r);

end