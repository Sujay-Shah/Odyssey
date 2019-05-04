//
//  Singleton.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

	template <class T>
	class Singleton : public NonCopyable
	{
	public:
		static bool Exists(void)
		{
			return m_self != nullptr;
		}

		static T* GetInstance(void)
		{
			if (Exists())
			{
				return static_cast<T*>(m_self);
			}
			else
			{
				cout << STRINGIFY(T)<<" doesn't exist, allocate memory to that object\n";
			}
			return nullptr;
		}

	protected:
		Singleton(void)
		{
			if (Exists())
			{
				cout << STRINGIFY(T) << " already exists!\n";
			}
			else
			{
				m_self = static_cast<T*>(this);
			}
		}

		~Singleton(void)
		{
			if (Exists())
			{
				m_self = nullptr;
			}
		}

	public:
		static void* m_self;
	};

	template <class T>
	void* Singleton<T>::m_self = nullptr;

