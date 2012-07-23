function [X1,Y1] = Direction2Spherical(D,r,c)

radius = sin(0.5*acos(-D(:,:,3)))./(2*sqrt(D(:,:,1).^2+D(:,:,2).^2));
u = (0.5+radius.*D(:,:,1));
v = (0.5-radius.*D(:,:,2));

X1 = round(RemoveSpecials(u)*c);
Y1 = round(RemoveSpecials(v)*r);

end

