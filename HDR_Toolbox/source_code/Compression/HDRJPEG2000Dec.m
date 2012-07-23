function imgRec = HDRJPEG2000Dec(name)
%
%
%       imgRec=HDRJPEG2000Dec(name)
%
%
%       Input:
%           -name: the prefix of the compressed HDR images using JPEG HDR
%
%       Output:
%           -imgRec: the reconstructed HDR image
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

%Read tone mapping data
fid = fopen([name,'_data.txt'],'r');
xMax = zeros(3,1);
xMin = zeros(3,1);
for i = 1:3
    fscanf(fid,'%s',1);
    xMax(i) = fscanf(fid,'%g',1);
    fscanf(fid,'%s',1);
    xMin(i) = fscanf(fid,'%g',1);
end
fclose(fid);

%Decompression
nBit = 16;
delta = 1e-6;
imgRec = double(imread([name,'_comp.jp2']))/(2^nBit-1);
for i = 1:3
    imgRec(:,:,i) = exp(imgRec(:,:,i)*(xMax(i)-xMin(i))+xMin(i))-delta;
end

imgRec(find(imgRec<0.0))=0;

end