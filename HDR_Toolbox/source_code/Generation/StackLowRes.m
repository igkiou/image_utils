function stackOut = StackLowRes (stack)
%
%
%        stackOut = StackLowRes (stack)
%
%
%        Input:
%           -stack: a stack of LDR images
%
%        Output:
%           -stackOut: a reshaped stack at low resolution
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

%stack of images
[r,c,col,n] = size(stack);
stackOut = [];

for i=1:n
    tmpStack = stack(:,:,:,i);
    tmpStack = round(imresize(tmpStack,0.01,'bilinear'));
    [r,c,col] = size(tmpStack);  
    
    if(i==1)
        stackOut = zeros(r*c,n,3);
    end
    
    for j=1:3
        stackOut(:,i,j) = reshape(tmpStack(:,:,j),r*c,1);
    end
end

end