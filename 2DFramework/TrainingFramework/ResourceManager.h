#pragma once

class ResourceManager {
private:
	static ResourceManager *s_instance;
	char **m_listModelFile;
	char **m_listTextureFile;
	char **m_listCubeFile;
	char **m_listShader;
public:
	ResourceManager();
	~ResourceManager();
	void Init();
	void CleanUp();
	static ResourceManager* GetInstance();
};