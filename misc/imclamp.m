function Y = imclamp(X, min, max)

Y = X;
Y(X < min) = min;
Y(X > max) = max;
