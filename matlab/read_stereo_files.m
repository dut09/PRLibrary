%   Tao Du
%   taodu@stanford.edu
%   April 29, 2014

%   write prsc files for stereo cameras
function [ fc_left, cc_left, kc_left, alpha_c_left,...
	fc_right, cc_right, kc_right, alpha_c_right, R, T ]...
        = read_stereo_files( left_filename, right_filename, stereo_filename )
    %   write left camera
    [~, ~, fc_left, cc_left, kc_left, alpha_c_left] = read_camera_file(left_filename);
    %   write right camera
    [~, ~, fc_right, cc_right, kc_right, alpha_c_right] = read_camera_file(right_filename);
    %   write stereo camera
    fid = fopen(stereo_filename, 'rb');
    R = fread(fid, [3 3], 'double', 'l')';
    T = fread(fid, [3 1], 'double', 'l');
    fclose(fid);
end

