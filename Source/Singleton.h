// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

// Singleton class
template<typename T>
class Singleton
{

public:
	// Create a new instance of type T as singleton
	static void CreateInstance()
	{
		if (mMyInstance == nullptr)			// Create instance if it is null
			mMyInstance = new T;			// Create a new instance of type T on the heap
	}
	// Delete current instance of the singleton 
	static void DestroyInstance()
	{
		delete mMyInstance;					// Clear memory of instance
		mMyInstance = nullptr;				// Set it to null
	}

	// Return current istance of the singleton 
	static T* GetInstance()
	{
		return mMyInstance;					// Return instance of 
	}
protected:
	Singleton() {}
	~Singleton() {}

private:
	static T* mMyInstance;					// Singleton instance of type T
};