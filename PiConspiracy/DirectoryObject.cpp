//
//  DirectoryObject.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "DirectoryObject.h"

	DirectoryObject::DirectoryObject(EDirectoryObjectType a_obj)
		: m_path(""),
		m_name(""),
		m_timeLastAccessed(),
		m_timeLastModified(),
		m_nameHash(),
		m_size(),
		m_objType(a_obj)
	{
	}

	DirectoryObject::~DirectoryObject()
	{
	}

	File::File() :DirectoryObject(DirectoryObjectType::File)
	{
	}


	File::~File()
	{
	}

	Folder::Folder() :DirectoryObject(DirectoryObjectType::Folder)
	{
	}

	Folder::~Folder()
	{
		for (auto it : m_Files)
		{
			delete it.second;
			it.second = nullptr;
		}
		m_Files.clear();

		for (auto it : m_Folders)
		{
			delete it.second;
			it.second = nullptr;
		}
		m_Folders.clear();

	}
	bool Folder::IsRoot(void) const
	{
		return m_isRoot;
	}
	void Folder::SetRoot(bool a_val)
	{
		m_isRoot = a_val;
	}
