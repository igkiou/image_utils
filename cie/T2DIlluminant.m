function [illum, wavelengths] = T2DIlluminant(T)

if ((nargin < 1) || isempty(T)),
	T = 6504;
end;

xy = T2xy(T);

M = 0.0241 + 0.2562 * xy(1) - 0.7341 * xy(2);
M1 = (-1.3515 - 1.7703 * xy(1) + 5.9114 * xy(2)) / M;
M2 = (0.0300 - 31.4424 * xy(1) + 30.0717 * xy(2)) / M;

tempMat = csvread('ds.csv');
wavelengths = tempMat(:, 1);
S0 = tempMat(:, 2);
S1 = tempMat(:, 3);
S2 = tempMat(:, 4);

illum = S0 + M1 * S1 + M2 * S2;