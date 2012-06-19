/**
 * Provides the IEntity interface class for all game entities in a game.
 *
 * @file src/GQE/Entity/interfaces/IEntity.cpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 * @date 20120609 - Move property methods to new PropertyManager class
 * @date 20120618 - Moved ID from Instance class to this base class
 */
#include <GQE/Entity/interfaces/IEntity.hpp>
#include <GQE/Entity/interfaces/ISystem.hpp>
namespace GQE
{
	typeEntityID IEntity::mNextID = 0;

  IEntity::IEntity() :
    mEntityID(UseNextID())
  {
    ILOG() << "IEntity::ctor(" << mEntityID << ")" << std::endl;
  }

  IEntity::~IEntity()
  {
    ILOG() << "IEntity::dtor(" << mEntityID << ")" << std::endl;
  }

  const typeEntityID IEntity::GetID() const
  {
    return mEntityID;
  }

	typeEntityID IEntity::UseNextID()
	{
		return ++mNextID;
	}

	void IEntity::AddSystem(ISystem* theSystem)
	{
		if(mSystemList.find(theSystem->GetID())==mSystemList.end())
    {
      mSystemList[theSystem->GetID()]=theSystem;
    }
    else
    {
      WLOG() << "IEntity:AddSystem() system(" << theSystem->GetID()
        << ") is already controlling this entity." << std::endl;
    }
	}
	bool IEntity::HasSystem(typeSystemID theSystemID)
	{
		bool anResult=false;
		if(mSystemList.find(theSystemID)!=mSystemList.end())
		{
			anResult=true;
		}
		return anResult;
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
