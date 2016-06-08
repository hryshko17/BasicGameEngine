
#ifndef CASSETMANAGER_H
#define CASSETMANAGER_H

#include "../../Game/Core/stdafx.h"

class CAssetManager
{
private:

	static CAssetManager* m_pcInstance;

	//this map is all the textures that have been loaded. the string(key) is the filepath
	std::unordered_map<std::wstring, ID3D11ShaderResourceView*> m_umTextureList;

	//helper functions
	void SearchFilesByPath(std::string szFolderPath, std::string szFileExtension, std::vector<std::string>& vszFilePaths);
	bool AddTexture(std::wstring szTexturePath, ID3D11ShaderResourceView* d3dSRV);

public:

	CAssetManager() = default;
	~CAssetManager() = default;


	static CAssetManager* GetInstance();
	static void DeleteInstance();

	bool Initialize();
	void Shutdown();

	bool CheckTextureExists(std::wstring szTexturePath);
	bool LoadTexture(const wchar_t* chTexturePath, ID3D11ShaderResourceView* d3dSRV);

	//accessors
	std::unordered_map<std::wstring, ID3D11ShaderResourceView*> GetTextureList() { return m_umTextureList; };
	ID3D11ShaderResourceView* GetTextureFromList(std::wstring szFilePath) { return m_umTextureList[szFilePath]; };

};

#endif