function Lexp = InverseReinhard(img, eo_parameters)
%
%       Lexp = InverseReinhard(img, eo_parameters)
%
%        Input:
%           -img: input LDR image. The image is assumed to be linearized
%           -eo_parameters: is an array of two values:
%               -eo_parameters(1): is the maximum output luminance in cd/m^2
%               -eo_parameters(2: is the strecthing factor of the
%                Reinhard et al. curve
%
%        Output:
%           -Lexp: expanded luminance using inverse Reinhard et al. 2002
%           opeartor
%
%     Copyright (C) 2012  Francesco Banterle
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

%parameters extraction
LMaxOut = eo_parameters(1);
LWhite = eo_parameters(2);

%Luminance channel
L = lum(img);
maxL = max(max(L));
L = L/maxL;

%Luminance expansion
LWhite2 = LWhite^2;
Lexp = LWhite*(LMaxOut/2)*(L-1+sqrt((1-L).^2+4*L/LWhite2));
end