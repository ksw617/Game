#pragma once
#include "Singleton.h"
#include "GameObject.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Component.h"


class Resources : public Singleton<Resources>
{
	friend class Singleton<Resources>;
private:
	Resources() = default;
	virtual ~Resources() = default;
private:
	using KeyObjMap = map<wstring, shared_ptr<Object>>;
	array<KeyObjMap, OBJECT_TYPE_COUNT> resources;
public:
	template<typename T>
	OBJECT_TYPE GetObjectType();

	template<typename T>
	shared_ptr<T> Load(const wstring& key, const wstring& path);

	template<typename T>
	bool Add(const wstring& key, shared_ptr<T> object);

	template<typename T>
	shared_ptr<T> GetResource(const wstring& key);

public:
	shared_ptr<Mesh> LoadCubeMesh();
	//±¸ ·Îµå
	shared_ptr<Mesh> LoadSphereMesh();
};

template<typename T>
inline OBJECT_TYPE Resources::GetObjectType()
{
	if (std::is_same_v<T, GameObject>)
		return OBJECT_TYPE::GAMEOBJECT;
	if (is_same_v<T, Material>)
		return OBJECT_TYPE::MATERIAL;
	if (is_same_v<T, Mesh>)
		return OBJECT_TYPE::MESH;
	if (is_same_v<T, Shader>)
		return OBJECT_TYPE::SHADER;
	if (is_same_v<T, Texture>)
		return OBJECT_TYPE::TEXTURE;
	if (is_same_v<T, Component>)
		return OBJECT_TYPE::COMPONENT;
	else
		return OBJECT_TYPE::NONE;
	
}

template<typename T>
inline shared_ptr<T> Resources::Load(const wstring& key, const wstring& path)
{
	OBJECT_TYPE objectType = GetObjectType<T>();
	KeyObjMap& keyObjMap = resources[static_cast<UINT8>(objectType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	shared_ptr<T> object = make_shared<T>();
	object->Load(path);
	KeyObjMap[key] = object;

	return object;

}

template<typename T>
inline bool Resources::Add(const wstring& key, shared_ptr<T> object)
{
	OBJECT_TYPE objectType = GetObjectType<T>();
	KeyObjMap& keyObjMap = resources[static_cast<UINT8>(objectType)];
	
	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return false; 

	keyObjMap[key] = object;

	return true;

}

template<typename T>
inline shared_ptr<T> Resources::GetResource(const wstring& key)
{
	OBJECT_TYPE objectType = GetObjectType<T>();
	KeyObjMap& keyObjMap = resources[static_cast<UINT8>(objectType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

		return nullptr;
}
