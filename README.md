# JUCE Amalgamation

A distribution of the [JUCE][1] library in amalgamated source code form.

## What's an amalgamation?

An amalgamation is simply a collection of header and source files that have been
concatenated together to form one or more very large files. In this form, they
are easy to add to your existing project as source files (rather than linking
as a library). They are also easier to redistribute if you are making an open
source application and don't want to have any external dependencies.

## What is JUCE?

JUCE (Jules' Utility Class Extensions) is an all-encompassing C++ class library
for developing cross-platform software. It contains pretty much everything
you're likely to need to create most applications, and is particularly
well-suited for building highly-customised GUIs, and for handling graphics
and sound. JUCE can target the following platforms:

**Mac OSX**: Applications and VST/AudioUnit/RTAS/NPAPI plugins can be compiled
             Xcode for OSX 10.4 or later.

**iOS**: Native iPhone and iPad apps can be built with Xcode.

**Windows**: Applications and VST/RTAS/NPAPI/ActiveX plugins can be built using
             MS Visual Studio. The results are all fully compatible with
             Windows XP, or Win7.

**Linux**: Applications and plugins can be built for any kernel 2.6 or later.

**Android**: Android apps can now be built using Ant and Eclipse, with the
             Android NDK v5 or later.

For all the platforms above, the code that you write is the same, and you don't
need to worry about any platform-specific details. If your C++ is portable, then
you should be able to simply re-compile your app to run it on other OSes.

## How do I use this?

JUCE is broken up into modules, with each module coming with its own pair of
amalgamated source and header files. Add the desired amalgamated module sources
to your existing project. For Macintosh and iOS targets you will use the ".mm"
files. For all others, add the ".cpp" files. Include the corresponding header
in your source code. You will need to create a file called "AppConfig.h" and
configure your include paths so that it is visible to the JUCE amalgamated
sources.

The [JUCE Amalgmation][7] was built using the [Amalgamate Templates][3].

## License

Copyright 2004-11 by [Raw Material Software Ltd.][5] <br>
JUCE is released under the [GNU Public Licence][6], which means it can be freely
copied and distributed, and costs nothing to use in open-source applications. If
you'd like to release a closed-source application that uses JUCE, [paid
commercial licences][2] are available.

[1]: http://rawmaterialsoftware.com/juce.php "The JUCE Library"
[2]: http://rawmaterialsoftware.com/jucelicense.php "JUCE Commercial Licensing"
[3]: https://github.com/vinniefalco/Amalgams/ "Amalgamate Templates"
[4]: https://github.com/vinniefalco/Amalgamate/ "Amalgamate Tool"
[5]: http://rawmaterialsoftware.com "Raw Material Software, Ltd."
[6]: http://www.gnu.org/licenses/gpl-2.0.html "GNU GPL Version 2"
[7]: https://github.com/vinniefalco/JUCEAmalgam "JUCE Amalgamation"
