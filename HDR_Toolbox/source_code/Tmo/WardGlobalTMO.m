function imgOut=WardGlobalTMO(img,Ld_Max)
%
%       imgOut=Ward1TMO(img,Ld_Max)
%
%
%       Input:
%           -img: input HDR image
%           -Ld_Max: Maximum LDR luminance
%
%       Output
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

if(~exist('Ld_Max'))
    Ld_Max=100;
end

%harmonic mean
Lwa=logMean(L);

%contrast scale
m=(((1.219+(Ld_Max/2)^0.4)/(1.219+Lwa^0.4))^2.5)/Ld_Max;

imgOut=img*m;

end