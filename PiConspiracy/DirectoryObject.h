//
//  DirectoryObject.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

	struct DirectoryObjectType
	{
		enum Enum
		{
			File,
			Folder
		};
	};

	TYPEDEF_ENUM(DirectoryObjectType);

	struct DirectoryObject
	{
		DirectoryObject(EDirectoryObjectType a_obj);
		virtual ~DirectoryObject(void);
		string m_path;
		string m_name;
		time_t m_timeLastModified;
		time_t m_timeLastAccessed;
		uint32 m_nameHash;
		int32 m_size;
		EDirectoryObjectType m_objType;
	};

	struct File : public DirectoryObject
	{
	public:
		File(void);
		~File(void);

		string m_parentFolder;
		string m_ext;
		uint32 m_extHash;
	};

	struct Folder : public DirectoryObject
	{
	public:
		Folder(void);
		~Folder(void);
	
		string m_parentFolder;
	
		unordered_map<uint32, File*> m_Files;
		unordered_map<uint32, Folder*> m_Folders;

	bool IsRoot(void) const;
	void SetRoot(bool a_val);
	private:
	bool m_isRoot = false;

	
	};
