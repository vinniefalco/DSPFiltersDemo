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

#ifndef DSPFILTERS_DEMO_GUI_HEADER
#define DSPFILTERS_DEMO_GUI_HEADER

#include "juce_audio_utils_amalgam.h"
#include "juce_core_amalgam.h"
#include "juce_graphics_amalgam.h"
#include "juce_gui_basics_amalgam.h"
#include "juce_gui_extra_amalgam.h"

#include "../demo_core/demo_core.h"

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4100) // unreferenced formal parameter
#pragma warning (disable: 4512) // assignment operator could not be generated
#endif

#include "gui/CustomSlider.h"
#include "gui/FilterChart.h"
#include "gui/ResizableLayout.h"
#include "gui/SliderGroup.h"

#include "gui/BrickWallChart.h"
#include "gui/ContentComponentConstrainer.h"
#include "gui/CpuMeter.h"
#include "gui/FilterControls.h"
#include "gui/GainChart.h"
#include "gui/GroupDelayChart.h"
#include "gui/MainPanel.h"
#include "gui/MainWindow.h"
#include "gui/PhaseChart.h"
#include "gui/PoleZeroChart.h"
#include "gui/SettingsPanel.h"
#include "gui/SettingsWindow.h"
#include "gui/StepResponseChart.h"

#include "gui/MainApp.h"

#ifdef _MSC_VER
#pragma warning (pop)
#endif

#endif

