/*******************************************************************************

"A Collection of Useful C++ Classes for Digital Signal Processing"
 By Vinnie Falco

Official project location:
https://github.com/vinniefalco/DSPFilters

See Documentation.cpp for contact information, notes, and bibliography.

--------------------------------------------------------------------------------

License: MIT License (http://www.opensource.org/licenses/mit-license.php)
Copyright (c) 2009 by Vinnie Falco

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*******************************************************************************/

#include "demo_gui.h"

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4100) // unreferenced formal parameter
#pragma warning (disable: 4127) // conditional expression is constant
#pragma warning (disable: 4355) // 'this' used in base member initializer list
#pragma warning (disable: 4512) // assignment operator could not be generated
#endif

#include "gui/BrickWallChart.cpp"
#include "gui/CpuMeter.cpp"
#include "gui/FilterChart.cpp"
#include "gui/FilterControls.cpp"
#include "gui/GainChart.cpp"
#include "gui/GroupDelayChart.cpp"
#include "gui/MainApp.cpp"
#include "gui/MainPanel.cpp"
#include "gui/MainWindow.cpp"
#include "gui/PhaseChart.cpp"
#include "gui/PoleZeroChart.cpp"
#include "gui/ResizableLayout.cpp"
#include "gui/SettingsPanel.cpp"
#include "gui/SettingsWindow.cpp"
#include "gui/SliderGroup.cpp"
#include "gui/StepResponseChart.cpp"

#ifdef _MSC_VER
#pragma warning (pop)
#endif
