%
%       HDR Toolbox demo 1:
%	   1) Load "Bottles_Small.pfm" HDR image
%	   2) Show the image in linear mode
%	   3) Show the image in gamma mode
%	   4) Tone map and show the image using Reinhard's TMO 
%	   5) Show and Apply Color Correction to the tone mapped image
%	   6) Save the tone mapped image as PNG
%
%       Author: Francesco Banterle
%       Copyright February 2011 (c)
%
%

disp('1) Load the image Bottles_Small.pfm using hdrimread');
img = hdrimread('Bottles_Small.hdr');

disp('2) Show the image Bottles_Small.pfm in linear mode using imshow');
figure(1);
GammaTMO(img, 1.0, 0, 1);

disp('3) Show the image Bottles_Small.hdr applying gamma');
figure(2);
GammaTMO(img, 2.2, 0, 1);

disp('4) Show the image Bottles_Small.hdr applying Reinhard''s Tmo');
figure(3);
imgTMO = ReinhardTMO(img);
GammaTMO(imgTMO, 2.2, 0, 1);

disp('5) Show and Apply Color Correction to the tone mapped image');
figure(4);
imgTMO = ColorCorrection(imgTMO,0.8);
GammaTMO(imgTMO, 2.2, 0, 1);

disp('6) Save the tone mapped image as a PNG.');
hdrimwrite(GammaTMO(imgTMO, 2.2, 0, 0), 'Bottles_Small_TMO.png');



