
#include "AssetManager.h"
#include "DDSTextureLoader.h"
#include "../Renderer/Renderer.h"

CAssetManager* CAssetManager::m_pcInstance = nullptr;

CAssetManager* CAssetManager::GetInstance()
{
	if (m_pcInstance == nullptr)
		m_pcInstance = new CAssetManager();

	return m_pcInstance;
}

void CAssetManager::DeleteInstance()
{
	if (m_pcInstance != nullptr)
		SAFE_DELETE(m_pcInstance);
}


bool CAssetManager::Initialize()
{
	//load in all textures from the Assets::Art::2D::Textures Folder
	std::vector<std::string> vszTexturePaths;
	SearchFilesByPath("../Assets/Art/2D/Textures/", ".dds", vszTexturePaths);

	for (unsigned int i = 0; i < vszTexturePaths.size(); i++)
	{
		std::wstring szPath(vszTexturePaths[i].begin(), vszTexturePaths[i].end());
		if (m_umTextureList.find(szPath) == m_umTextureList.end())
		{
			ID3D11ShaderResourceView* d3dNewSRV = nullptr;
			if (LoadTexture(szPath.c_str(), d3dNewSRV) == false)
			{
				std::string szFailed = "Failed to Load Texture during Asset Manager Initialization!\nTexture File: ";
				szFailed += vszTexturePaths[i].c_str();
				szFailed += "\n";
				DEBUG_OUTPUT(szFailed);
				return false;
			}
		}
		else 
			continue;
	}

	return true;
}

void CAssetManager::Shutdown()
{
	MAP_RELEASE_ALL(m_umTextureList);
}

bool CAssetManager::CheckTextureExists(std::wstring szTexturePath)
{
	std::unordered_map<std::wstring, ID3D11ShaderResourceView*>::iterator iter = m_umTextureList.find(szTexturePath);
	if (iter != m_umTextureList.end())
		return true;//it exists

	return false;
}

bool CAssetManager::LoadTexture(const wchar_t* chTexturePath, ID3D11ShaderResourceView* d3dSRV)
{
	if (chTexturePath == nullptr)
	{
		std::wstring szFailed = L"Texture Failed to Load due to nullptr. File: ";
		szFailed += std::wstring(chTexturePath) + L"\n";
		DEBUG_OUTPUTW(szFailed);
		return false;
	}

	if (CheckTextureExists(std::wstring(chTexturePath)) == false)
	{
		//The texture does not exists. Load it in and add it to the texture list
		HRESULT error = CreateDDSTextureFromFile(CRenderer::GetInstance()->GetDevice(), chTexturePath, nullptr, &d3dSRV);
		if (FAILED(error))
		{
			std::wstring szFailed = L"Texture Failed to Load from File. File: ";
			szFailed += std::wstring(chTexturePath) + L"\n";
			DEBUG_OUTPUTW(szFailed);
			return false;
		}

		if (AddTexture(std::wstring(chTexturePath), d3dSRV) == false)
		{
			std::wstring szFailed = L"Failed to Add to Texture List. File: ";
			szFailed += std::wstring(chTexturePath) + L"\n";
			DEBUG_OUTPUTW(szFailed);
			return false;
		}
		else
		{
			std::wstring szSuccess = L"Texture Loaded Successfully. File: ";
			szSuccess += std::wstring(chTexturePath) + L"\n";
			DEBUG_OUTPUTW(szSuccess);
		}
	}
	else//texture has alrdy been loaded in
	{
		std::wstring szSuccess = L"Texture has already been loaded. File: ";
		szSuccess += std::wstring(chTexturePath) + L"\n";
		DEBUG_OUTPUTW(szSuccess);
	}

	return true;
}


/*	Private Functions	*/
void CAssetManager::SearchFilesByPath(std::string szFolderPath, std::string szFileExtension, std::vector<std::string>& vszFilePaths)
{
	std::string szSearchPath = szFolderPath + "*" + szFileExtension;

	WIN32_FIND_DATAA fdSearch;
	HANDLE hFind = ::FindFirstFileA(szSearchPath.c_str(), &fdSearch);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(fdSearch.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				std::string path = szFolderPath + fdSearch.cFileName;
				vszFilePaths.push_back(path);
			}
		} while (::FindNextFileA(hFind, &fdSearch));
		::FindClose(hFind);
	}
}

bool CAssetManager::AddTexture(std::wstring szTexturePath, ID3D11ShaderResourceView* d3dSRV)
{
	if (d3dSRV != nullptr && CheckTextureExists(szTexturePath) == false)
	{
		m_umTextureList.insert(std::pair<std::wstring, ID3D11ShaderResourceView*>(szTexturePath, d3dSRV));
		return true;
	}

	return false;
}
