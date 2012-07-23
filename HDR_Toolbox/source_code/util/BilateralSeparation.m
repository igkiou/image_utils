function [Base,Detail]=BilateralSeparation(img, simga_s, simga_r)
%
%
%       [Base,Detail]=BilateralSeparation(img)
%
%
%       Input:
%           -img: input image
%	    -sigma_r: range sigma
%	    -sigma_s: spatial sigma
%
%       Output:
%           -Base: the low frequency image
%           -Detail: the high frequency image
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

%default parameters
if(~exist('sigma_s')|~exist('sigma_r'))
    sigma_r=4;
    sigma_s=0.5;
end

%Base Layer
tmp=log2(img+1);

try
    imgFil = bilateralFilter(tmp,[],min(min(tmp)),max(max(tmp)),sigma_s,sigma_r);
catch exception
    imgFil = bilateralFilter(tmp);
end

Base=2.^(imgFil)-1;

%Removing 0
Base(find(Base<0))=0;

%Detail Layer
Detail=RemoveSpecials(img./Base);

end