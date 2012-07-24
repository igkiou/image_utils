function img = pfmread(filename)
% TODO: Handle color properly
%
%
%       img = read_pfm(filename)
%
%        Input:
%           -filename: the name of the file to open
%
%        Output:
%           -img: the opened image
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
fid = fopen(filename, 'r');

%reading the header
fscanf(fid, '%c', 3);
m = fscanf(fid, '%d', 1);
fscanf(fid, '%c', 1);
n = fscanf(fid, '%d', 1);
fscanf(fid, '%c', 1);
fscanf(fid, '%f', 1);
fscanf(fid, '%c', 1);

% img = zeros(m, n);
img = fread(fid, n * m, 'float');
img = reshape(img, [m n]);
img = img';
img = img(end:-1:1, :);

fclose(fid);

