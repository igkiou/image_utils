function [imgALogLuv, param_a, param_b] = float2ALogLuv(img, n_bits)
%
%       imgLogLuv=float2ALogLuv(img)
%
%
%        Input:
%           -img: a HDR image in RGB
%           -n_bits: number of bits for luma
%
%        Output:
%           -imgALogLuv: the HDR image in the 32-bit Adaptive LogLuv format
%           -param_a: adaptive multiplicative parameter
%           -param_b: adaptive additive parameter
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


%check n_bits
if(~exist('n_bits')) n_bits = 16; end

if(n_bits<=0)
    n_bits = 16;
end

%Conversion from RGB to XYZ
%Matrix from the original paper
mtxRGB2XYZ = [  0.497, 0.339, 0.164;...
                0.256, 0.678, 0.066;...
                0.023, 0.113, 0.864];
imgXYZ = ConvertLinearSpace(img, mtxRGB2XYZ);

%Calculating a and b
Y = imgXYZ(:,:,2);
maxY = max(max(Y));
ind_positive = find(Y>0);
min_positive_Y = min(min(Y(ind_positive)));
param_a = (2^n_bits-2)/log2(maxY/min_positive_Y);
param_b = (1/param_a) - log2(min_positive_Y);

%Encoding luminance Y
imgALogLuv = zeros(size(img));
Le = zeros(size(Y));
Le(ind_positive) = (param_a*(log2(Y(ind_positive))+param_b));
imgALogLuv(:,:,1) = ClampImg(Le,0,2^n_bits-1);

%CIE (u,v) chromaticity values
norm = (imgXYZ(:,:,1)+imgXYZ(:,:,2)+imgXYZ(:,:,3));
x = imgXYZ(:,:,1)./ norm;
y = imgXYZ(:,:,2)./ norm;

%Encoding chromaticity
norm_uv = (-2*x+12*y+3);
u_prime = 4*x./norm_uv;
v_prime = 9*y./norm_uv;

Ue = floor(410*u_prime);
imgALogLuv(:,:,2) = ClampImg(Ue,0,255);

Ve = floor(410*v_prime);
imgALogLuv(:,:,3) = ClampImg(Ve,0,255);

end