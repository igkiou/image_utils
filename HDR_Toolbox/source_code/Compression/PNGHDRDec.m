function imgRec = PNGHDRDec(name)
%
%
%       imgRec = PNGHDRDec(img)
%
%
%       Input:
%
%           -name:  is the name of an PNGHDR image.
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
%Reading PNGHDR file
[img, map, alpha] = imread(name);
%imshow(alpha);

%Removing gamma
gamma = 2.0;
img = (double(img)/255).^gamma;
alpha = (double(alpha)/255).^gamma;

%Ratio decoding
RI = 2.^(alpha*32-16)-2^-16;
RI(find(RI<0))=0;

L=0.213*img(:,:,1)+0.715*img(:,:,2)+0.072*img(:,:,3);

imgRec = zeros(size(img));
for i=1:3
    imgRecCol = (img(:,:,i)./L).^5.0;
    imgRec(:,:,i) = imgRecCol.*L.*RI;
end

end