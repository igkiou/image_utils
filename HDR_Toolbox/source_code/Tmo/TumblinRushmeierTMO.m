function imgOut=TumblinRushmeierTMO(img, Lda, LdMax, CMax, Lwa)
%
%        imgOut=TumblinRushmeierTMO(img, Lwa, Lda, CMax)   
%
%
%        Input:
%           -img: input HDR image
%           -Lda: adaptation display luminance in [10,30] cd/m^2
%           -LdMax: maximum display luminance in [80-180] cd/m^2
%           -CMax: maximum LDR monitor contrast typically between 30 to 100
%           -Lwa: adaptation world luminance cd/m^2
%
%        Output:
%           -imgOut: tone mapped image in [0,1]
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
L = lum(img);

%default parameters
if(~exist('Lda'))
    Lda = 20;
end

if(~exist('LdMax'))
    LdMax = 100;
end

if(~exist('CMax'))
    CMax = 100;
end

if(~exist('Lwa'))
    tmp = log(L+2.3*1e-5);
    Lwa = exp(mean(tmp(:)));
end

%Range compression
gamma_w  = gammaTumRushTMO(Lwa);
gamma_d  = gammaTumRushTMO(Lda);
gamma_wd = gamma_w./(1.855+0.4*log(Lda));
mLwa     = sqrt(CMax).^(gamma_wd-1);
Ld = mLwa.*Lda*((L./Lwa).^(gamma_w./gamma_d));

%Removing the old luminance
imgOut=zeros(size(img));
for i=1:3
    imgOut(:,:,i)=img(:,:,i).*Ld./L;
end

imgOut = imgOut / LdMax;

end