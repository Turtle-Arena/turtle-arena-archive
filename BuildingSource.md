# Introduction #

This is the main page for how to compile and run Turtle Arena. It may seem a little complex at first, but it should be fairly easy.

# All Platforms #

For all platforms the first step is to get the source code.

## Get the Source ##
### Latest Development ###
You will need a git client, such as [TortoiseGit](https://code.google.com/p/tortoisegit/) (Windows), [RabbitVCS](http://www.rabbitvcs.org) (Linux), or the command line program "git"

Next you will need to do a source checkout, this varies by program.

Code and data are split up, you're recommended to name code repo "engine" and data repo "baseturtle".

  * https://github.com/Turtle-Arena/turtle-arena-code.git
  * https://github.com/Turtle-Arena/turtle-arena-data.git

### Released Versions ###
#### Archives ####
There are source tarballs on the download page for Turtle Arena 0.4.x, 0.5.x, and 0.6.x versions. Earlier versions included a zip of the source in main download.

Notes;
  * 0.4.x does not include data files or data source files in the tarball, they are in subversion.
  * 0.5.x/0.6 does not include data files in the tarball, they are in subversion.

The data files are zipped and included in the game downloads (see `base/assets*.pk3`).

### Subversion ###
You will need a subversion client, such as [TortoiseSVN](http://tortoisesvn.tigris.org/) (Windows), [RabbitVCS](http://www.rabbitvcs.org) (Linux), or the command line program "svn"

Next you will need to do a source checkout, this varies by program.

Tags for latest stable release versions
  * http://turtlearena.googlecode.com/svn/tags/0.6.1/
  * http://turtlearena.googlecode.com/svn/tags/0.4.3/
  * http://turtlearena.googlecode.com/svn/tags/0.2/

# Windows #

See [Building source on Windows](BuildingSourceOnWindows.md).

# Linux #
Should also work on other Unix-like operating systems, like FreeBSD, but have not been tested.

See [Building source on Linux](BuildingSourceOnLinux.md).

# Mac OS X #

See [Building source on Mac OS X](BuildingSourceOnMacOSX.md).