function imgOut=FerwerdaTMO(img, LdMax, Lda)
%
%       imgOut=FerwerdaTMO(img, Lda, LdMax)
%
%
%        Input:
%           -img: input HDR image
%           -LdMax: maximum luminance of the display in cd/m^2
%           -Lda: adaptation luminance in cd/m^2
%
%        Output:
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
%          -imgOut: tone mapped image

%is it a three color channels image?
check3Color(img);

%Luminance channel
L=lum(img);

if(~exist('LdMax')|~exist('Lda'))
    LdMax=100;
    Lda=30;
end

if(Lda<0)
    Lda=LdMax/2;
end

%Luminance world adaptation
Lwa=mean(mean(L));

%Contrast reduction
mR=TpFerwerda(Lda)/TpFerwerda(Lwa);
mC=TsFerwerda(Lda)/TsFerwerda(Lwa);
k=ClampImg((1-(Lwa/2-0.01)/(10-0.01))^2,0,1);

%Removing the old luminance
imgOut=zeros(size(img));
vec=[1.05,0.97,1.27];
for i=1:3
    imgOut(:,:,i)=(mC*img(:,:,i)+vec(i)*mR*k*L);
end

imgOut=imgOut/LdMax;

%imgOut=imgOut/mean(mean(mean(imgOut)));

end