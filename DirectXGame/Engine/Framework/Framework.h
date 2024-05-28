#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"
#include "SceneManager.h"
#include <memory>

class DSFramework{
public:

	virtual ~DSFramework() = default;

	virtual void Init();

	virtual void Update();

	virtual void Draw() = 0;

	virtual void Finalize();

	void Run();

};

