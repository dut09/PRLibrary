%   Tao Du
%   taodu@stanford.edu
%   April 29, 2014

%   write prsc files for stereo cameras
function [  ] = write_stereo_files( fc_left, cc_left, kc_left, alpha_c_left,...
	fc_right, cc_right, kc_right, alpha_c_right, R, T, ...
    left_filename, right_filename, stereo_filename )
    %   write left camera
    write_camera_file(fc_left, cc_left, kc_left, alpha_c_left, left_filename);
    %   write right camera
    write_camera_file(fc_right, cc_right, kc_right, alpha_c_right, right_filename);
    %   write stereo camera
    fid = fopen(stereo_filename, 'wb');
    fwrite(fid, R', 'double');
    fwrite(fid, T, 'double');
    fclose(fid);
end

