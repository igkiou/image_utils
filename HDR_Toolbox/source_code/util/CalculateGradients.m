function [fx,fy] = CalculateGradients(I)

kernelX = [0,0,0;-1,0,1;0,0,0];
kernelY = [0,1,0;0,0,0;0,-1,0];

fx = imfilter(I,kernelX,'same')/2;
fy = imfilter(I,kernelY,'same')/2;

end