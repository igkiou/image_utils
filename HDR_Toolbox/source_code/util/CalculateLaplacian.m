function [dx,dy,divG] = CalculateLaplacian(I,fx,fy)
%
%        [dx,dy,divG] = CalculateLaplacian(I)
%
%

if(~exist('fx')||~exist('fy'))
    [fx,fy] = CalculateGradients(I);
end

kernelX = [0,0,0;-1,1,0;0,0,0];
kernelY = [0,0,0;0,1,0;0,-1,0];
dx = imfilter(fx,kernelX,'same');
dy = imfilter(fy,kernelY,'same');

divG = RemoveSpecials(dx+dy);

end