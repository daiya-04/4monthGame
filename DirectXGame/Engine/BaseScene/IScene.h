#pragma once

class SceneManager;

class IScene{
public:

	virtual void Init() = 0;
	virtual void Update() = 0;
	
	virtual void DrawModel() = 0;
	virtual void DrawBackGround() = 0;
	virtual void DrawUI() = 0;
	virtual void DrawParticle() = 0;
	virtual void DrawParticleModel() = 0;
	/// <summary>
	/// パイプライン未設定(最初に呼び出される)
	/// </summary>
	virtual void DrawNotSetPipeline() = 0;

	virtual void DebugGUI() = 0;

	virtual ~IScene() {};

};

