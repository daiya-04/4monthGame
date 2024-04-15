#pragma once
#include <Camera.h>

class BaseEnemy
{
public:

	~BaseEnemy() = default;

	virtual void Initialize() = 0;

	virtual void Update() = 0;

	virtual void Draw(const Camera& camera) = 0;

protected:



};

class NormalEnemy : public BaseEnemy 
{
public:

private:


};
