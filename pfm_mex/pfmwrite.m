function ret = pfmwrite(img,filename)
% TODO: Handle color properly
%
%       ret = write_pfm(img,filename)
%
%
%        Input:
%           -img: the image to write on the hard disk
%           -filename: the name of the image to write
%
%        Output:
%           -ret: a boolean value, it is set to 1 if the write succeed, 0 otherwise
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

ret = 0;
%open the file
fid = fopen(filename,'w');

[n,m,c]=size(img);

fprintf(fid,'PF%c%d %d%c-1.000000%c',10,m,n,10,10);

tot = n*m;
data=zeros(tot*3,1);

img = img(end:-1:1, :);
img = img';

for i=1:c
    indx = i:3:(tot*3);
    data(indx) = reshape(img(:,:,i),tot,1);
end

for i=c:3
    indx = i:3:(tot*3);
    data(indx) = reshape(img(:,:,c),tot,1);
end

fwrite(fid,data,'float');

fclose(fid);

end
