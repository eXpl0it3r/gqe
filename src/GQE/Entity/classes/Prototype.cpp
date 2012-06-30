/**
 * Provides the Prototype class that can be used as a template class to produce
 * Instance classes.
 *
 * @file src/GQE/Entity/classes/Prototype.cpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 * @date 20120616 - Adjustments made for new PropertyManager
 * @date 20120620 - Use Prototype to manage and delete eventually all Instances created
 * @date 20120622 - Fix issues deleting Instances at destruction
 * @date 20120630 - Add new GetInstance method to retrieve specific instance
 */
#include <GQE/Entity/classes/Prototype.hpp>
#include <GQE/Entity/classes/Instance.hpp>
#include <GQE/Entity/interfaces/ISystem.hpp>

namespace GQE
{
  Prototype::Prototype(const typePrototypeID thePrototypeID) :
    IEntity(),
    mPrototypeID(thePrototypeID)
  {
    ILOG() << "Prototype::ctor(" << mPrototypeID << ")" << std::endl;
  }

  Prototype::~Prototype()
  {
    ILOG() << "Prototype::dtor(" << mPrototypeID << ")" << std::endl;

    // Make sure we delete all created Instance classes
    std::map<const typeEntityID, Instance*>::iterator anInstanceIter;

    // Start at the beginning of the list of Instance classes
    anInstanceIter = mInstances.begin();
    while(anInstanceIter != mInstances.end())
    {
      // Get our Instance pointer
      Instance* anInstance = anInstanceIter->second;

      // Increment our iterator before deleting our Instance
      anInstanceIter++;

      // Cause the Instance to drop itself from all its registered ISystems
      anInstance->DropAllSystems();

      // Now delete the Instance
      delete anInstance;

      // Clear out value
      anInstance = NULL;
    }

    // Last of all clear our list of Instances
		mInstances.clear();
  }

  const typePrototypeID Prototype::GetID(void) const
  {
    return mPrototypeID;
  }

  Instance* Prototype::GetInstance(const typeEntityID theEntityID) const
  {
    // Default return result to NULL for now
    Instance* anResult = NULL;

    // See if we can find theEntityID in our map of Instances
    std::map<const typeEntityID, Instance*>::const_iterator anInstanceIter;
    anInstanceIter = mInstances.find(theEntityID);
    if(anInstanceIter != mInstances.end())
    {
      // Get the Instance class found
      anResult = anInstanceIter->second;
    }

    // Return anResult found above or NULL otherwise
    return anResult;
  }

  Instance* Prototype::MakeInstance()
  {
    // Try to create an Instance class right now
    Instance* anInstance = new(std::nothrow) Instance(*this);

    // If successful, clone our Prototype properties to this new Instance class
    if(anInstance != NULL)
    {
      // Clone our Prototype properties into the new Instance class
      anInstance->mProperties.Clone(mProperties);

      // Make sure the new Instance is registered with the same systems
      std::map<const typeSystemID, ISystem*>::iterator anSystemIter;
      for(anSystemIter=mSystems.begin();
          anSystemIter!=mSystems.end();
          ++anSystemIter)
      {
        ISystem* anSystem = (anSystemIter->second);
        anInstance->AddSystem(anSystem);
        anSystem->AddEntity(anInstance);
      }

      // Add this Instance to our list of instances we have created
      mInstances.insert(std::pair<const typeEntityID, Instance*>(
        anInstance->GetID(), anInstance));

      // Make note of this instance in our log file
      ILOG() << "Prototype(" << mPrototypeID << ")::MakeInstance("
        << anInstance->GetID() << ") created successfully!" << std::endl;
    }
    else
    {
      ELOG() << "Unable to create instance, out of memory!" << std::endl;
    }

    // Return the new Instance class created
    return anInstance;
  }
} // namespace GQE

/**
 * Copyright (c) 2010-2012 Jacob Dix
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
