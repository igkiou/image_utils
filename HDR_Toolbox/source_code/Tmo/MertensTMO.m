function imgOut = MertensTMO( img, format, wE, wS, wC )
%
%
%        imgOut = MertensTMO( img, format, wE, wS, wC )
%
%
%        Input:
%           -img: input HDR image
%           -format: the format of LDR images ('bmp', 'jpg', etc) in case
%                    img=[] and the tone mapped images is built from a sequence of
%                    images in the current directory
%           -wE: the weight for the well exposedness in [0,1]. Well exposed
%                pixels are taken more into account if the wE is near 1
%                otherwise they are not taken into account.
%           -wS: the weight for the saturation in [0,1]. Saturated
%                pixels are taken more into account if the wS is near 1
%                otherwise they are not taken into account.
%           -wC: the weight for the contrast in [0,1]. Strong edgese are 
%                taken more into account if the wE is near 1
%                otherwise they are not taken into account.
%
%        Output:
%           -imgOut: tone mapped image
%
%        Note: Gamma correction is not needed because it works on gamma
%        corrected images.
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

%default parameters if they are missing
if(~exist('wE'))
    wE=0.75;
end

if(~exist('wS'))
    wS=0.5;
end

if(~exist('wC'))
    wC=0.5;
end

%stack generation
stack=[];

[r,c,col]=size(img);
if((r*c)>0)
    %Convert the HDR image into a stack
    [stack,stack_exposure] = GenerateExposureBracketing(img,2);
else
    %load images from the current directory
    images=dir(['*.',format]);
    n = length(images);
    for i=1:n
        stack(:,:,:,i) = double(imread(images(i).name))/255;
    end
end

%number of images in the stack
[r,c,col,n]=size(stack);

%Computation of weights for each image
total=zeros(r,c);
weight=zeros(r,c,n);
for i=1:n
    %calculation of the weights
    L=lum(stack(:,:,:,i));    
    weightE=MertensWellExposedness(stack(:,:,:,i));
    weightS=MertensSaturation(stack(:,:,:,i));
    weightC=MertensContrast(L);
    %final weight
    weight(:,:,i)=(weightE.^wE).*(weightC.^wC).*(weightS.^wS);
    total=total+weight(:,:,i);
end

%Normalisation of weights
for i=1:n
    weight(:,:,i)=RemoveSpecials(weight(:,:,i)./total);
end

%empty pyramid
tf=[];
for i=1:n
    %Laplacian pyramid: image
    pyrImg=pyrImg3(stack(:,:,:,i),@pyrLapGen);
    %Gaussian pyramid: weight   
    pyrW=pyrGaussGen(weight(:,:,i));

    %Multiplication image times weights
    tmpVal=pyrLstS2OP(pyrImg,pyrW,@pyrMul);
   
    if(i==1)
        tf=tmpVal;
    else
        %accumulation
        tf=pyrLst2OP(tf,tmpVal,@pyrAdd);    
    end
end

%Evaluation of Laplacian/Gaussian Pyramids
imgOut=zeros(r,c,col);
for i=1:3
    imgOut(:,:,i) = pyrVal(tf(i));
end

%Clamping
imgOut = ClampImg(imgOut,0,1);

disp('This algorithm outputs images with gamma encoding. Inverse gamma is not required to be applied!');

end