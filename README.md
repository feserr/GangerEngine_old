GangerEngine
=======

- Windows [![Build status](https://ci.appveyor.com/api/projects/status/m1rc8c2peak0cmxr/branch/master?svg=true)](https://ci.appveyor.com/project/feserr/gangerengine-hxaa4/branch/master)
- Linux and OSX [![Build Status](https://travis-ci.org/feserr/GangerEngine.svg?branch=master)](https://travis-ci.org/feserr/GangerEngine)

# Index

- [About](#about)
- [What's New?](#whats-new)
- [How to Build](#how-to-build)
- [Requirements](#requirements)
- [Dependencies](#dependencies)
- [Demos](#demos)
- [Future features](#future)
- [Contributing](#contributing)
- [Bugs?](#bugs)
- [Honorable mentions?](#mentions)
- [License](#license)

<a name="about"></a>
# GangerEngine 0.0.4.1

GangerEngine is an open source game framework for desktop games.

Version: 0.0.4.1 - Released: 06th November 2016

<a name="whats-new"></a>
## What's new in 0.0.4.1?

* Replace boost filesystem for [Wenzel Jakob](https://github.com/wjakob) solution.

<a name="how-to-build"></a>
## How to Build

You need to install [cmake](https://cmake.org/) version 2.8 or above.

Then launch one of the following scripts depending in with SO you are using:
* BuildLinux.sh for Linux.
* BuildMac.sh for MacOSX.
* BuildVisualStudio for Windows with Visual Studio.

<a name="requirements"></a>
## Requirements

Games created with GangerEngine require a graphic card compatible with OpenGL 3.3 or newer and C++ compiler compatible with C++14.

<a name="dependencies"></a>
## Dependencies

* Included:
  * [GLM](http://glm.g-truc.net/0.9.7/index.html)
  * [UTF8-CPP](http://utfcpp.sourceforge.net/)
  * [filesystem](https://github.com/wjakob/filesystem)
  * [Lua](https://www.lua.org)
  * [LuaBridge](https://github.com/vinniefalco/LuaBridge)

* External:
  * [CEGUI 0.87](http://cegui.org.uk/)
  * [SDL2](https://www.libsdl.org/index.php)
  * [GLEW](http://glew.sourceforge.net/)
  * [Box2D](http://box2d.org/) (only for the simple platformer demo).


I do not include the libraries for the external dependecies.

<a name="demos"></a>
## Demos

* Massive ball collision simulator.
* Top-Down zombie shooter.
* Pong.
* Simple platformer with editor.

<a name="future"></a>
## Future features

* CMake build script for the demos.

<a name="contributing"></a>
## Contributing

- If you find a bug then please report it on [GitHub Issues][issues].

- If you have a feature request, or have written a game or demo that shows GangerEngine in use, then please get in touch. We'd love to hear from you!

- If you issue a Pull Request for GangerEngine, please only do so against the `dev` branch and **not** against the `master` branch.

<a name="bugs"></a>
## Bugs?

Please add them to the [Issue Tracker][issues] with as much info as possible, especially source code demonstrating the issue.

<a name="mentions"></a>
## Honorable mentions

* [Wenzel Jakob](https://github.com/wjakob) for the filesystem solution.
* [Benjamin Arnold](https://github.com/Barnold1953) for the awesome 2D game engine tutorial, without it this engine will never exist.

<a name="license"></a>
## License

GangerEngine is released under the [Apache v2 license](http://www.apache.org/licenses/LICENSE-2.0.html).

[issues]: https://github.com/Reisor/GangerEngine/issues
