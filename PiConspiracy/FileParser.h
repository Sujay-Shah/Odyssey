//
//  FileParser.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once


	struct DirectoryObject;
	struct Folder;

	class FileParser : Singleton<FileParser>
	{
	public:
		
		FileParser(void);
		~FileParser(void);

		//function to explore what files are stored in a directory
		unordered_map<uint32, DirectoryObject*> ExploreDirectory(const string& a_path, Folder* a_parent);
		const unordered_map<uint32, DirectoryObject*> GetListOfElements(void) const;
		
	private:
		void ExploreDirectoryRec(const string& a_path, Folder* a_parent);

		//all files/folders obj found in the directory go here
		unordered_map<uint32, DirectoryObject*> m_obj;
	};

