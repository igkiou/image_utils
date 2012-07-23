 -Author: Francesco Banterle
-Contact e-mail: support@advancedhdrbook.com

-License: This software is distributed under GPL v3 license (see license.txt)

-Year: Fall 2010-2011

-Title: HDR Toolbox for Matlab

-Version: 1.0.4

-How To Install:
1) Unzip the file HDRToolbox.zip in a FOLDER on your PC/MAC;
2) Run Matlab
3) Set the FOLDER as current directory
4) Write the command installHDRToolbox in the Command Window, and wait
for the installation process.


-How to Use the Toolbox:
Run the demos to understand how to use I/O function, visualize images, tone Map Images, etc.

-Update Report:

06/07/2012: -Partial rewritten of the ChangeMapping.m function and its auxiliary functions
            -Fixed BanterleExpandMap.m to be closer to reference. Fixed a bug in the samples
             clamping, and added a Density Estimation through image splatting (see imSplat.m function).
             Note that this version is not suitable for videos but only for still images.
            -Added a batch function for converting HDR images into LDR images using tone mapping
            -Fixed an issue in Create1DDistribution.m. Thanks to Joel Kronander.
18/06/2012: -Fixed a bug in float2LogLuv.m. Thanks to Vassilios Solachidis
14/06/2012: -Added FalseColor.m function for the visualization of HDR images in false colors 
	    -Added imSameHeight.m utility function for adjusting the height
	    of an image to match the height of a target one
12/06/2012: -Improved TumblinRushmeierTMO.m, added maximum display Luminance parameter
	    -Added clamping to MertensTMO.m
07/06/2012: -Fixed a minor bug in GenerateExposureBracketing.m
	    -Made BuildHDR.m and CombineLDR.m functions more flexible and general,
	    allowing stack to be used as input
28/01/2012: -Fixed a bug in the WardGlobalTMO. Thanks to kirkt from the hdrlabs.com forum.
             
            -CompoCon was made Octave compliant (explicit cast to logical values).

27/01/2012: -Removed some case sensitive problems for Octave. Thanks to kirkt from hdrlabs.com forum.
25/01/2012: -Improved read_rgbe.m function; now RLE compressed streams can be opened.
            -Minor improvements in write_rgbe.m function (RLE compression still missing).
17/12/2011: -Fixed PoissonSolver, it now works not only for squared images

	    -Added a first attempt to retinex
18/11/2011: -Fixed some bugs in the LogLuv encoding and added the Adaptive LogLuv encoding
25/10/2011: -Demo images are compressed into RGBE format to reduce download time.
11/10/2011: -Added ReinhardBilTMO.m
	    -Renamed the main util folder due to compability with Linux. Thanks to Shanmuga Raman.
11/09/2011: -Fixed a bug in the MeylanEO.m file
08/03/2011: -Fixed some problems in ExponentialTMO.m. Thanks to Alessandro Artusi.


-Known issues: -In ChangeMapping.m the Spherical mapping is not yet supported for output.

-Other: - Read license.txt
        - bilateralFilter.m is an implementation by Jiawen Chen under MIT License.
        - BanterleExpandMap.m is an implementation of Banterle et al. 2008 (SCCG 2008) for
          images only not videos. It is not an implementation of Banterle et al. 2006 (Graphite) or
          Banterle et al. 2007 (The Visual Computer). If you are not sure about parameters to
          be used please contact the author of the HDR Toolbox at any time.
