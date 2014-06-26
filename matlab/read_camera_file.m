%   Tao Du
%   taodu@stanford.edu
%   June 25, 2014

function [ height, width, fc, cc, kc, alpha_c ] = read_camera_file( filename )
    %   write left camera
    fid = fopen(filename, 'rb');
    %   compute the height and width
    height = fread(fid, 1, 'int32', 'l');
    width = fread(fid, 1, 'int32', 'l');
    fc = fread(fid, 2, 'double', 'l');
    cc = fread(fid, 2, 'double', 'l');
    kc = fread(fid, 5, 'double', 'l');
    alpha_c = fread(fid, 1, 'double', 'l');
    fclose(fid);
end



