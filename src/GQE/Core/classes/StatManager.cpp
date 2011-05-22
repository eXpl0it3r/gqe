/**
 * Provides the StatManager class in the GQE namespace which is responsible
 * for collecting and providing Statistical information about the application.
 * This information includes, the current Updates per second and Frames per
 * second and other statistics.
 *
 * @file src/GQE/Core/classes/StatManager.hpp
 * @author Ryan Lindeman
 * @date 20110128 - Initial Release
 * @date 20110128 - Moved to GQE Core library and src directory
 * @date 20110218 - Change to system include style
 */
 
#include <assert.h>
#include <sstream>
#include <GQE/Core/classes/StatManager.hpp>
#include <GQE/Core/classes/App.hpp>
 
namespace GQE
{
  StatManager::StatManager() :
    mApp(NULL),
    mShow(false),
    mFrames(0),
    mUpdates(0),
    mFPS(),
    mUPS()
  {
  }
 
  StatManager::~StatManager()
  {
    // Output to log file
    if(NULL != mApp)
    {
      mApp->mLog << "StatManager::~StatManager() dtor called" << std::endl;
    }
 
    // Clear pointers we don't need anymore
    mApp = NULL;
  }

  void StatManager::DoInit(void)
  {
    // Reset our counters
    mFrames = 0;
    mUpdates = 0;

    // Reset our clocks
    mFrameClock.Reset();
    mUpdateClock.Reset();

    // Position and color for the FPS/UPS string
    mFPS.SetColor(sf::Color(255,255,255,128));
    mFPS.Move(0,0);
    mUPS.SetColor(sf::Color(255,255,255,128));
    mUPS.Move(0,30);

#if (SFML_VERSION_MAJOR < 2)
    // Default strings to display for Frames/Updates per second
    mFPS.SetText("");
    mUPS.SetText("");
#else
    // Default strings to display for Frames/Updates per second
    mFPS.SetString("");
    mUPS.SetString("");
#endif
  }

  void StatManager::DeInit(void)
  {
  }

  bool StatManager::IsShowing(void) const
  {
    return mShow;
  }

  void StatManager::SetShow(bool theShow)
  {
    mShow = theShow;
  }

  Uint32 StatManager::GetUpdates(void) const
  {
    return mUpdates;
  }

  Uint32 StatManager::GetFrames(void) const
  {
    return mFrames;
  }

  void StatManager::RegisterApp(App* theApp)
  {
    // Check that our pointer is good
    assert(NULL != theApp && "StatManager::RegisterApp() theApp pointer provided is bad");
 
    // Make a note of the pointer
    assert(NULL == mApp && "StatManager::RegisterApp() theApp pointer was already registered");
    mApp = theApp;
  }

  void StatManager::Update(void)
  {
    // Check our App pointer
    assert(NULL != mApp && "StatManager::Update() bad app pointer");

    // Increment our update counter
    mUpdates++;
    if(mUpdateClock.GetElapsedTime() > 1.0f)
    {
      // Updates string stream
      std::ostringstream updates;

      // Update our UPS string to be displayed
      updates.precision(2);
      updates.width(7);
      updates << "UPS: " << std::fixed << (float)mUpdates / mUpdateClock.GetElapsedTime();
#if (SFML_VERSION_MAJOR < 2)
      mUPS.SetText(updates.str());
#else
      mUPS.SetString(updates.str());
#endif

      // Reset our Update clock and update counter
      mUpdates = 0;
      mUpdateClock.Reset();
    }
  }

  void StatManager::Draw(void)
  {
    // Check our mApp pointer
    assert(NULL != mApp && "StatManager::Draw() invalid app pointer provided");

    // Increment our frame counter
    mFrames++;
    if(mFrameClock.GetElapsedTime() > 1.0f)
    {
      // Frames string stream
      std::ostringstream frames;

      // Get our FramesPerSecond value
      frames.precision(2);
      frames.width(7);
      frames << "FPS: " << std::fixed << (float)mFrames / mFrameClock.GetElapsedTime();
#if (SFML_VERSION_MAJOR < 2)
      mFPS.SetText(frames.str());
#else
      mFPS.SetString(frames.str());
#endif

      // Reset our Frames clock and frame counter
      mFrames = 0;
      mFrameClock.Reset();
    }

    // Are we showing the current statistics?
    if(mShow)
    {
      // Draw the Frames Per Second debug value on the screen
      mApp->mWindow.Draw(mFPS);

      // Draw the Updates Per Second debug value on the screen
      mApp->mWindow.Draw(mUPS);
    }
  }
}; // namespace GQE

/**
 * Copyright (c) 2010-2011 Ryan Lindeman
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
