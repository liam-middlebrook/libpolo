<a href='http://www.itba.edu.ar'><img src='http://libpolo.googlecode.com/files/itbalogo.png' align='right' alt='ITBA Instituto Tecnológico de Buenos Aires' /></a>

## libpolo ##

libpolo is a lightweight graphics, sound and network library for educational environments.

It is aimed towards students learning the C and C++ language, and is very suitable for high school and first-year university courses. Its functionality is built around the GLUT library, providing a streamlined interface and full access to OpenGL. A feature list:

  * 2D primitives (point, line, rect, quad, triangle, circle and a rounded rect)
  * pen stroke and solid/gradient/texture fill for all primitives
  * texture uploading from BMP files (RGB, RGBA formats)
  * simple mouse and keyboard handling
  * RGB and HSV color spaces

The network module supports unlimited TCP/IP connections, both in client and server mode.

The sound module is capable of playing back WAV, AIFF and OGG files.

It is cross-platform, and works under Linux, Mac OS X and Windows.

It is very easy to integrate in a project. Just add polo.c and polo.h in your project, and link with the GLUT library.

For libpolonet, add polonet.c and polonet.h. For libpolosnd, add polosnd.c and polosnd.h.

## Help & Discussion ##

You can go to the [libpolo group](http://groups.google.com/group/libpolo) for help!

## Screenshots ##

A screenshot of the mouse painter demo:

![http://libpolo.googlecode.com/files/demo_paint.jpg](http://libpolo.googlecode.com/files/demo_paint.jpg)

A screenshot of the OpenGL demo:

![http://libpolo.googlecode.com/files/demo_3d.jpg](http://libpolo.googlecode.com/files/demo_3d.jpg)