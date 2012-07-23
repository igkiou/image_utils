function weight = WeightFunction(img, weight_type)
%
%       weight = WeightFunction(img, weight_type)
%
%
%        Input:
%           -img: input LDR image in [0,1]
%           -weight_type:
%               - 'all': weight is set to 1
%               - 'hat': hat function 1-(2x-1)^12
%               - 'Deb97': Debevec and Malik 97 weight function
%
%        Output:
%           -weight: the output weight function for a given LDR image
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

switch weight_type
    case 'all'
        weight = ones(size(img));
    case 'hat'
        weight = 1 - (2*img-1).^12;
    case 'Deb97'
        Zmin = 0/255;
        Zmax = 255/255;
        tr = (Zmin+Zmax)/2;
        indx1 = find (img<=tr);
        indx2 = find (img>tr);
        weight = zeros(size(img));
        weight(indx1) = img(indx1) - Zmin;
        weight(indx2) = Zmax - img(indx2);
        weight(find(weight<0)) = 0;
        weight = weight/max(max(max(weight)));
    otherwise 
        weight = 1;
end
end