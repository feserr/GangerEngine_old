GangerEngine
=======

- Windows [![Build status](https://ci.appveyor.com/api/projects/status/pta0jukudrmqkwt7/branch/master?svg=true)](https://ci.appveyor.com/project/feserr/gangerengine-hxaa4/branch/master)
- Linux and OSX [![Build Status](https://travis-ci.org/Reisor/GangerEngine.svg)](https://travis-ci.org/feserr/GangerEngine)

# Index

- [About](#about)
- [What's New?](#whats-new)
- [How to Build](#how-to-build)
- [Requirements](#requirements)
- [Dependencies](#dependencies)
- [Future features](#future)
- [Contributing](#contributing)
- [Bugs?](#bugs)
- [License](#license)

<a name="about"></a>
# GangerEngine 0.0.4

GangerEngine is an open source game framework for desktop games. It is based in a fork of  [Bengine](https://github.com/Barnold1953/GraphicsTutorials), by Benjamin Arnold.

Version: 0.0.4 - Released: 04th July 2016

<a name="whats-new"></a>
## What's new in 0.0.4?

* Added class interface for common classes.
* Added a debug renderer.
* Added GUI system based in [CEGUI](http://cegui.org.uk/).
* Added a screen list class to manage different rooms.
* A 2D side-scrolling with level editor demo.

<a name="how-to-build"></a>
## How to Build

You need to install [cmake](https://cmake.org/) version 2.8 or above.

Then launch one of the following scripts depending in with SO you are using:
* BuildLinux.sh for Linux.
* BuildMac.sh for MacOSX.
* BuildVisualStudio for Windows with Visual Studio.

<a name="requirements"></a>
## Requirements

Games created with GangerEngine require a graphic card compatible with OpenGL 3.3 or newer.

<a name="dependencies"></a>
## Dependencies

* [Boost 1.61](http://www.boost.org/)
* [CEGUI 0.87](http://cegui.org.uk/)
* [SDL2](https://www.libsdl.org/index.php)
* [GLEW](http://glew.sourceforge.net/)
* [GLM](http://glm.g-truc.net/0.9.7/index.html)
* [UTF8-CPP](http://utfcpp.sourceforge.net/)
* [Box2D](http://box2d.org/) (only for the ninja demo).

<a name="future"></a>
## Future features

* CMake build script for the demos.
* Collision system inside the engine.

<a name="contributing"></a>
## Contributing

- If you find a bug then please report it on [GitHub Issues][issues].

- If you have a feature request, or have written a game or demo that shows GangerEngine in use, then please get in touch. We'd love to hear from you!

- If you issue a Pull Request for GangerEngine, please only do so against the `dev` branch and **not** against the `master` branch.

<a name="bugs"></a>
## Bugs?

Please add them to the [Issue Tracker][issues] with as much info as possible, especially source code demonstrating the issue.

<a name="license"></a>
## License

GangerEngine is released under the [Apache v2 license](http://www.apache.org/licenses/LICENSE-2.0.html).

[issues]: https://github.com/Reisor/GangerEngine/issues
