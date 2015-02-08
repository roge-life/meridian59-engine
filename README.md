# Engine
GameEngine stack for Windows, built from Open-Source or free components.

## Main components
<table>
<tr>
  <td><b>Name</b></td>
  <td><b>Description</b></td>
  <td><b>License</b></td>
  <td><b>Website</b></td>
</tr>
<tr>
  <td>Ogre3D</td>
  <td>Render-Engine</td>
  <td><a href="http://en.wikipedia.org/wiki/MIT_License">MIT</a></td>
  <td><a href="http://www.ogre3d.org">http://www.ogre3d.org</a></td>
</tr>
<tr>
  <td>CEGUI</td>
  <td>GUI-Engine</td>
  <td><a href="http://en.wikipedia.org/wiki/MIT_License">MIT</a></td>
  <td><a href="http://cegui.org.uk">http://cegui.org.uk</a></td>
</tr>
<tr>
  <td>OIS</td>
  <td>Input-Engine</td>
  <td><a href="http://en.wikipedia.org/wiki/Zlib_License">zlib</a></td>
  <td><a href="http://sourceforge.net/projects/wgois">http://sourceforge.net/projects/wgois</a></td>
</tr>
<tr>
  <td>ParticleUniverse</td>
  <td>Particles-Engine</td>
  <td><a href="http://en.wikipedia.org/wiki/MIT_License">MIT</a></td>
  <td><a href="http://www.fxpression.com">http://www.fxpression.com</a></td>
</tr>
<tr>
  <td>irrKlang</td>
  <td>Sound-Engine</td>
  <td><a href="http://www.ambiera.com/irrklang/license.html">Proprietary **</a></td>
  <td><a href="http://www.ambiera.com/irrklang">http://www.ambiera.com/irrklang</a></td>
</tr>
</table>

 ** irrKlang is only free to use in non-commercial projects! Consult their website for further information.

## Dependencies
<table>
<tr>
  <td><b>Name</b></td>
  <td><b>Description</b></td>
  <td><b>License</b></td>
  <td><b>Website</b></td>
</tr>
<tr>
  <td>zlib</td>
  <td>ZIP-Compression</td>
  <td><a href="http://en.wikipedia.org/wiki/Zlib_License">zlib</a></td>
  <td><a href="http://www.zlib.net">http://www.zlib.net</a></td>
</tr>
<tr>
  <td>zziplib</td>
  <td>ZIP-Archives</td>
  <td><a href="http://zziplib.sourceforge.net/copying.html">LGPL/MPL</a></td>
  <td><a href="http://zziplib.sourceforge.net">http://zziplib.sourceforge.net</a></td>
</tr>
<tr>
  <td>pcre</td>
  <td>Regular expressions</td>
  <td><a href="http://www.pcre.org/licence.txt">BSD</a></td>
  <td><a href="http://www.pcre.org">http://www.pcre.org</a></td>
</tr>
<tr>
  <td>OpenEXR</td>
  <td>EXR-Fileformat</td>
  <td><a href="http://www.openexr.com/license.html">Modified BSD</a></td>
  <td><a href="http://www.openexr.com">http://www.openexr.com</a></td>
</tr>
<tr>
  <td>LibTIFF</td>
  <td>TIF-Fileformat</td>
  <td><a href="http://en.wikipedia.org/wiki/BSD_licenses">BSD-like</a></td>
  <td><a href="http://www.remotesensing.org/libtiff">http://www.remotesensing.org/libtiff</a></td>
</tr>
<tr>
  <td>LibRAW</td>
  <td>RAW-Fileformat</td>
  <td><a href="http://www.libraw.org/libraw-lite">LGPL</a></td>
  <td><a href="http://www.libraw.org">http://www.libraw.org</a></td>
</tr>
<tr>
  <td>LibPNG</td>
  <td>PNG-Fileformat</td>
  <td><a href="http://www.libpng.org/pub/png/src/libpng-LICENSE.txt">Custom</a></td>
  <td><a href="http://www.libpng.org/pub/png/libpng.html">http://www.libpng.org/pub/png/libpng.html</a></td>
</tr>
<tr>
  <td>LibOpenJPEG</td>
  <td>JPG-Fileformat</td>
  <td><a href="http://en.wikipedia.org/wiki/BSD_licenses">BSD</a></td>
  <td><a href="http://www.openjpeg.org">http://www.openjpeg.org</a></td>
</tr>
<tr>
  <td>LibJPEG</td>
  <td>JPG-Fileformat</td>
  <td><a href="http://www.gnu.org/licenses/gpl-2.0.html">GPLv2</a></td>
  <td><a href="http://sourceforge.net/projects/libjpeg">http://sourceforge.net/projects/libjpeg</a></td>
</tr>
<tr>
  <td>FreeImage</td>
  <td>Image-Codecs</td>
  <td><a href="http://freeimage.sourceforge.net/license.html">GPL</a></td>
  <td><a href="http://freeimage.sourceforge.net">http://freeimage.sourceforge.net</a></td>
</tr>
<tr>
  <td>FreeType</td>
  <td>Font-Codecs</td>
  <td><a href="http://www.freetype.org/license.html">GPL/FTL</a></td>
  <td><a href="http://www.freetype.org">http://www.freetype.org</a></td>
</tr>
<tr>
  <td>CG</td>
  <td>Shaders</td>
  <td><a href="http://developer.download.nvidia.com/cg/Cg_3.0/license.pdf">Custom</a></td>
  <td><a href="https://developer.nvidia.com/cg-toolkit">https://developer.nvidia.com/cg-toolkit</a></td>
</tr>
<tr>
  <td>boost</td>
  <td>C++ framework</td>
  <td><a href="http://www.boost.org/users/license.html">Custom</a></td>
  <td><a href="http://www.boost.org">http://www.boost.org</a></td>
</tr>
</table>

## Compiling
 * Requires Visual Studio 2013.
 * Use included <a href="https://github.com/cyberjunk/Engine/blob/master/src/Engine.sln">Engine.sln</a> solution file.
 * Almost all libraries are statical linked.
 * All 32-Bit builds use the v120_xp toolset and are compatible with Windows XP.
 * Engine can also be built in 64-Bit.
 * Do not upgrade any projects/solutions when opening the solution the first time and being asked so. It's because of the older .NET4 and the older toolsets, which you probably want to keep for XP compatibility.
 * Release-builds have high optimizations, SSE2 enabled and no debuginfo. Debug-builds the opposite.
 * This package works with CLR (C++ CLI) enabled application (see TEST app).
 * You maybe have to set your Debugging *WorkFolder* to *$(OutDir)* (in project properties) to run the TEST app from VS directly. Otherwise try starting the TEST.exe directly in case of errors.
 * The sources-structure (buildfiles, content) has been heavily adjusted from its original packages/downloads/checkouts. All dependencies have been ripped of any documentations and additional, unnecessary files.
 * The TEST application is an mini executable to test whether everything links in OK. It's the only executable project in the solution.
 * All solution projects can be built independently from each other (due to the fully statical linking), except for the TEST application, which depends on every single other one.
 * Make sure to have a look at the */src/README/* subfolder
