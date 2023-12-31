#include "ModelLoader.hpp"
#include "Age/Resource/File.hpp"
#include "Age/Other/Logger.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Age/Other/print_math.hpp"
#include "Age/Resource/ResourceLoader.hpp"
#include "Age/Transform/Transform.hpp"
#include "Age/Math/Math.hpp"

namespace a_game_engine
{
	namespace
	{
		struct Vertex
		{
			vec3 pos, normal;
			vec2 uv;
		};
		struct TextureInfo
		{
			std::string name;
			aiTextureType type;
		};

		std::unique_ptr<ModelResource::Mesh> handleAiMesh(aiMesh* mesh, 
			const std::vector<MaterialProps>& materials, bool tangentSpace)
		{
			try
			{
				std::vector<Vertex> vertices;
				vertices.resize(mesh->mNumVertices);
				for (uint i = 0; i < mesh->mNumVertices; i++)
				{
					vertices[i].pos.x = mesh->mVertices[i].x;
					vertices[i].pos.y = mesh->mVertices[i].y;
					vertices[i].pos.z = mesh->mVertices[i].z;
					if (mesh->HasNormals())
					{
						vertices[i].normal.x = mesh->mNormals[i].x;
						vertices[i].normal.y = mesh->mNormals[i].y;
						vertices[i].normal.z = mesh->mNormals[i].z;
					}
					if (mesh->mTextureCoords[0])
					{
						vertices[i].uv.x = mesh->mTextureCoords[0][i].x;
						vertices[i].uv.y = mesh->mTextureCoords[0][i].y;
					}
				}
				std::vector<uint> indices;
				for (uint i = 0; i < mesh->mNumFaces; i++)
				{
					auto& face = mesh->mFaces[i];
					for (uint j = 0; j < face.mNumIndices; j++)
						indices.push_back(face.mIndices[j]);
				}

				VertexBuffer buffer;
				buffer.addFloatBuffer((float*)vertices.data(), vertices.size() * 8, { 3, 3, 2 });
				buffer.setIndexBuffer(indices);
				if (tangentSpace and mesh->mTangents)
				{
					std::vector<vec3> tangents;
					tangents.resize(mesh->mNumVertices);
					for (uint i = 0; i < mesh->mNumVertices; i++)
					{
						tangents[i].x = mesh->mTangents[i].x;
						tangents[i].y = mesh->mTangents[i].y;
						tangents[i].z = mesh->mTangents[i].z;
					}
					buffer.addFloatBuffer((float*)tangents.data(), tangents.size() * 3, 
						VertexBuffer::Attributes(3, 3, 3, 0));
				}

				auto result = std::make_unique<ModelResource::Mesh>();
				result->buffer = std::move(buffer);
				result->props = materials[mesh->mMaterialIndex];

				return result;
			}
			catch (...)
			{
				return nullptr;
			}
		}

		std::unique_ptr<ModelResource::Node> handleAiNode(aiNode* node, 
			const ModelResource& model, const ModelResource::Node* parent)
		{
			auto result = std::make_unique<ModelResource::Node>();

			result->ownerModel = &model;
			result->parent = parent;
			result->name = node->mName.C_Str();
			
			for (uint i = 0; i < node->mNumMeshes; i++)
			{
				uint meshIndex = node->mMeshes[i];
				const auto& mesh = model.meshes[meshIndex];
				result->meshes.push_back(mesh.get());
			}
			std::memcpy(&result->transform, &node->mTransformation, sizeof(mat4));
			result->transform.reflect();

			for (uint i = 0; i < node->mNumChildren; i++)
			{
				result->children.push_back(handleAiNode(node->mChildren[i], model, result.get()));
			}

			return result;
		}

		std::vector<Texture2D*> handleMaterialOfType(aiMaterial* mat, aiTextureType type,
			TextureLoader& textureLoader, const std::filesystem::path& path, bool srgb)
		{
			std::vector<Texture2D*> result;

			TextureLoader::Settings settings;
			settings.srgb = srgb;

			for (uint i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString filename;
				mat->GetTexture(type, i, &filename);
				result.push_back(&textureLoader.load(path / filename.C_Str(), settings));
			}

			return result;
		}

		bool has(const MaterialProps& props, const std::string& element)
		{
			for (int i = 0; i < props.size(); i++)
			{
				if (props[i].name == element)
					return true;
			}
			return false;
		}
		int find(const MaterialProps& props, const Texture2D* tex)
		{
			for (int i = 0; i < props.size(); i++)
			{
				if (props[i].property.getType() == ShaderProperty::Type::Texture2D &&
					std::get<ShaderProperty::Texture2DProp>(props[i].property.value).texture == tex)
					return i;
			}
			return -1;
		}

		MaterialProps handleAiMaterial(aiMaterial* mat, TextureLoader& loader, 
			const std::filesystem::path& path, bool disableSrgb)
		{
			TextureInfo colorTypes[] = { 
				{"baseColorMap", aiTextureType_DIFFUSE},
				{"baseColorMap", aiTextureType_BASE_COLOR}};
			TextureInfo infoTypes[] = { 
				{"normalMap", aiTextureType_NORMALS},
				{"roughnessMap", aiTextureType_DIFFUSE_ROUGHNESS},
				{"metalnessMap", aiTextureType_METALNESS},
				{"aoMap", aiTextureType_AMBIENT_OCCLUSION}};

			aiTextureType types[] = { aiTextureType_NORMALS,
			aiTextureType_DIFFUSE_ROUGHNESS,
			aiTextureType_METALNESS,
			aiTextureType_AMBIENT_OCCLUSION,
			aiTextureType_UNKNOWN };

			int slot = 0;
			MaterialProps result;
			for (const TextureInfo& texInfo : colorTypes)
			{
				auto textures = handleMaterialOfType(mat, texInfo.type, loader, path, !disableSrgb);
				for (auto* tex : textures)
				{
					if (not has(result, texInfo.name))
					{
						int texIndex = find(result, tex);
						if (texIndex == -1)
						{
							ShaderProperty::Texture2DProp texProp(*tex, slot);
							result.push_back({ texInfo.name, texProp });
							slot++;
						}
						else
						{
							auto& prop = result[texIndex];
							result.push_back({ texInfo.name, prop.property });
						}
					}
				}
			}
			for (const TextureInfo& texInfo : infoTypes)
			{
				auto textures = handleMaterialOfType(mat, texInfo.type, loader, path, false);
				for (auto* tex : textures)
				{
					if (not has(result, texInfo.name))
					{
						int texIndex = find(result, tex);
						if (texIndex == -1)
						{
							ShaderProperty::Texture2DProp texProp(*tex, slot);
							result.push_back({ texInfo.name, texProp });
							slot++;
						}
						else
						{
							auto& prop = result[texIndex];
							result.push_back({ texInfo.name, prop.property });
						}
					}
				}
			}

			return result;
		}
	}

	std::unique_ptr<ModelResource> ModelLoader::defModel = nullptr;

	ModelLoader::ModelLoader()
	{
		textureLoader.logOnLoad = textureLoader.logOnUnload = false;
	}

	ModelLoader::~ModelLoader()
	{
		for (const auto& m : resources)
		{
			if (m.second)
				Logger::logInfo("Model " + m.first.string() + " was unloaded");
		}
	}

	ModelResource& ModelLoader::load(const std::filesystem::path& path, const Settings& s)
	{
		return ResourceLoader::defaultLoad<ModelResource, std::filesystem::path>(resources, path, 
			[&](const std::filesystem::path& path)
			{
				return readFromFile(path, s);
			}, getDefault);
	}
	std::unique_ptr<ModelResource> ModelLoader::readFromFile(const std::filesystem::path& path, const Settings& s)
	{
		Assimp::Importer importer;
		uint flags = aiProcess_Triangulate;
		if (s.inverseUV)
			flags |= aiProcess_FlipUVs;
		if (s.withNormalMap)
			flags |= aiProcess_CalcTangentSpace;
		auto* scene = importer.ReadFile(path.string(), flags);

		if (not scene ||
			scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
			not scene->mRootNode)
		{
			Logger::logError(importer.GetErrorString());
			return nullptr;
		}

		auto result = std::make_unique<ModelResource>();

		auto parentPath = path.parent_path();
		std::vector<MaterialProps> materials;
		materials.resize(scene->mNumMaterials);
		for (uint i = 0; i < scene->mNumMaterials; i++)
		{
			auto* mat = scene->mMaterials[i];
			materials[i] = handleAiMaterial(mat, textureLoader, parentPath, s.disableSrgb);
		}

		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
			auto mesh = scene->mMeshes[i];

			auto ageMesh = handleAiMesh(mesh, materials, s.withNormalMap);
			if (not ageMesh)
			{
				Logger::logError("Cannot load model " + path.string());
				return nullptr;
			}
			result->meshes.push_back(std::move(ageMesh));
		}

		result->rootNode = handleAiNode(scene->mRootNode, *result, nullptr);
		result->rootNode->transform = s.rootNodeTransform;
		Logger::logInfo("Model " + path.string() + " was loaded");

		return result;
	}
	ModelResource& ModelLoader::getDefault()
	{
		if (defModel == nullptr)
		{
			defModel = std::make_unique<ModelResource>();
		}
		return *defModel;
	}
	ModelLoader::Settings::Settings(const mat4& mat, bool inverseUv, bool withNormalMap,
		bool disableSrgb)
		: rootNodeTransform(mat), inverseUV(inverseUv), withNormalMap(withNormalMap), disableSrgb(disableSrgb)
	{}
	ModelLoader::Settings::Settings(const vec3& scale, bool rotate, bool inverseUv, bool withNormalMap,
		bool disableSrgb)
		: inverseUV(inverseUv), withNormalMap(withNormalMap), disableSrgb(disableSrgb)
	{
		rootNodeTransform.setModelMatrix(
			{ 0.f }, 
			rotate ? vec3{glm::radians(90.f), 0.f, 0.f} : vec3{ 0.f },
			scale);
	}
}
