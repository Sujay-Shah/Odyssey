//
//  FileParser.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "FileParser.h"
#include "DirectoryObject.h"

#include "dirent.h"

	
	FileParser::FileParser(void)
	{
	}

	FileParser::~FileParser(void)
	{
		for (auto it: m_obj)
		{
			if (it.second->m_objType==EDirectoryObjectType::Folder)
			{
				Folder* folder = static_cast<Folder*>(it.second);
				if (folder->IsRoot())
				{
					delete folder;
					break;
				}

			}
		}
		m_obj.clear();
	
	}

 unordered_map<uint32, DirectoryObject*> FileParser::ExploreDirectory(const string& a_path, Folder* a_parent)
	{
		Folder* root = new Folder();
		root->m_path = a_path;
		root->m_name = a_path.substr(a_path.find_last_of("/") + 1);
		root->m_nameHash = Hash(root->m_name);
		root->m_parentFolder = root->m_name;
		root->SetRoot(true);
		std::pair<uint32, Folder*> keynFolder = { root->m_nameHash, root };
		m_obj.insert(keynFolder);
		ExploreDirectoryRec(a_path,root);

		return m_obj;
	}

	const unordered_map<uint32, DirectoryObject*> FileParser::GetListOfElements(void) const
	{
		return m_obj;
	}

	void FileParser::ExploreDirectoryRec(const string& a_path, Folder* a_parent)
	{
		DIR *directory;
		struct dirent *files;
		struct stat info;
		string currLoc;
		//DirFiles currFile;

		//opening the directory
		directory = opendir(a_path.c_str());
		if (!directory)
		{
			cout << "Path " << a_path << " not found\n";

			return;
		}

		//reading the directory
		while ((files = readdir(directory)) != NULL)
		{
			if (files->d_name[0] != '.')
			{
				//get the full path
		currLoc = string(a_path) /*+ "/"*/ + string(files->d_name);
		stat(currLoc.c_str(), &info);
		//check if its folder
		if (S_ISDIR(info.st_mode))
		{
			Folder* currFolder = new Folder();
			currFolder->m_name = string(files->d_name);
			currFolder->m_nameHash = Hash(currFolder->m_name);
			currFolder->m_path = currLoc;
			currFolder->m_parentFolder = a_parent->m_name;
			currFolder->m_size = info.st_size;
			currFolder->m_timeLastAccessed = info.st_atime;
			currFolder->m_timeLastModified = info.st_mtime;

			std::pair<uint32, Folder*> keynFolder = { currFolder->m_nameHash, currFolder };
		
			m_obj.insert(keynFolder);
			if (a_parent != nullptr)
			{
				a_parent->m_Folders.insert(keynFolder);
			}
			ExploreDirectoryRec(currLoc.c_str(),currFolder);
			continue;
		}
		//if elements not folder, its a file
		//currFile.m_ElementName = std::string(files->d_name);
				//currFile.m_locHash = Hash(currFile.m_loc);

		File* currFile = new File();
		currFile->m_path= string(a_path) + "/" + string(files->d_name);
		currFile->m_name = string(files->d_name);
		currFile->m_nameHash = Hash(currFile->m_name);
		currFile->m_size= info.st_size;
		currFile->m_timeLastAccessed = info.st_atime;
		currFile->m_timeLastModified = info.st_mtime;
		//update parent fodler size 
		a_parent->m_size += currFile->m_size;
				//get the extension of the above path
				currFile->m_ext = currFile->m_path.substr(currFile->m_path.find_last_of(".") + 1);
				currFile->m_extHash = Hash(currFile->m_ext);
		currFile->m_parentFolder = a_parent->m_name;

				std::pair<uint32, File*> keynFile = { currFile->m_nameHash, currFile };

				//push them in the files
		if (a_parent != nullptr)
		{
			a_parent->m_Files.insert(keynFile);
		}
		m_obj.insert(keynFile);
			}
		}

		//closing the directory
		closedir(directory);
	}
