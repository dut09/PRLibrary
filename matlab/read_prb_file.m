%   Tao Du
%   taodu@stanford.edu
%   April 29, 2014

%   read PRB file
%   input: 
%       fileName
%       type: 'rgb', 'depth'
%   output:
%       data
%   this function has been tested
function [ data ] = read_prb_file( filename, type )
fid = fopen(filename, 'rb');
%   skip the byte code
fread(fid, 1, 'int8', 'l');
height = fread(fid, 1, 'int32', 'l');
width = fread(fid, 1, 'int32', 'l');
if strcmp(type, 'rgb')
    image = fread(fid, [width * 3, height], 'double', 'l');
    image = image';
    data = zeros(height, width, 3);
    data(:, :, 1) = image(:, 1 : 3 : end);
    data(:, :, 2) = image(:, 2 : 3 : end);
    data(:, :, 3) = image(:, 3 : 3 : end);
elseif strcmp(type, 'depth')
    data = fread(fid, [width, height], 'double', 'l');
    data = data';
else
    disp('invalid type');
    data = 0;
end
fclose(fid);
end

