// Copyright (C) 2020 livvv2k <ml.smiley3@gmail.com>
//
// This file is part of EasyGLTF.
//
// EasyGLTF is free software : you can redistribute itand /or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// EasyGLTF is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with EasyGLTF.If not, see < https://www.gnu.org/licenses/>.

#include "easygltf.h"

#include "rapidjson/document.h"

#include <fstream>
#include <numeric>

// fuck me... PR please. Will insta merge that shit. Not hard to make it nicer than this
static std::array<double, 16> MatxMat(const std::array<double, 16>& matA, const std::array<double, 16>& matB)
{
	return std::array<double, 16>{
		(matA[0] * matB[0] + matA[1] * matB[4] + matA[2] * matB[8] + matA[3] * matB[12]),
		(matA[0] * matB[1] + matA[1] * matB[5] + matA[2] * matB[9] + matA[3] * matB[13]),
		(matA[0] * matB[2] + matA[1] * matB[6] + matA[2] * matB[10] + matA[3] * matB[14]),
		(matA[0] * matB[3] + matA[1] * matB[7] + matA[2] * matB[11] + matA[3] * matB[15]),

		(matA[4] * matB[0] + matA[5] * matB[4] + matA[6] * matB[8] + matA[7] * matB[12]),
		(matA[4] * matB[1] + matA[5] * matB[5] + matA[6] * matB[9] + matA[7] * matB[13]),
		(matA[4] * matB[2] + matA[5] * matB[6] + matA[6] * matB[10] + matA[7] * matB[14]),
		(matA[4] * matB[3] + matA[5] * matB[7] + matA[6] * matB[11] + matA[7] * matB[15]),

		(matA[8] * matB[0] + matA[9] * matB[4] + matA[10] * matB[8] + matA[11] * matB[12]),
		(matA[8] * matB[1] + matA[9] * matB[5] + matA[10] * matB[9] + matA[11] * matB[13]),
		(matA[8] * matB[2] + matA[9] * matB[6] + matA[10] * matB[10] + matA[11] * matB[14]),
		(matA[8] * matB[3] + matA[9] * matB[7] + matA[10] * matB[11] + matA[11] * matB[15]),

		(matA[12] * matB[0] + matA[13] * matB[4] + matA[14] * matB[8] + matA[15] * matB[12]),
		(matA[12] * matB[1] + matA[13] * matB[5] + matA[14] * matB[9] + matA[15] * matB[13]),
		(matA[12] * matB[2] + matA[13] * matB[6] + matA[14] * matB[10] + matA[15] * matB[14]),
		(matA[12] * matB[3] + matA[13] * matB[7] + matA[14] * matB[11] + matA[15] * matB[15]),
	};
}

// null terminated at eof
static void LoadFile(const std::string& filepath, std::vector<uint8_t>& out)
{
	std::ifstream fh(filepath, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
	size_t sz = fh.tellg();

	if (sz < 1)
		return;

	fh.seekg(0, std::ios::beg);

	out.clear();
	out.resize(sz + 1); // null char

	if (!fh.is_open())
		return;

	fh.read((char*) out.data(), sz);

	fh.close();
}

EGLTF::CEasyGLTF::CEasyGLTF() {}

EGLTF::CEasyGLTF::~CEasyGLTF() {}

bool EGLTF::CEasyGLTF::LoadGLTF_file(const std::string& filepath)
{
	std::vector<uint8_t> buffer;

	LoadFile(filepath, buffer);

	if (buffer.size() < 1)
	{
		return false;
	}

	fwrite((char*) buffer.data(), buffer.size(), 1, stdout);

	rapidjson::Document document;
	document.Parse((char*) buffer.data());

	if (document.HasParseError())
		return false;

	return ParseGLTF(document);
}

// Only works for gltf files with embedded data
bool EGLTF::CEasyGLTF::LoadGLTF_memory(const std::vector<uint8_t>& buffer)
{
	rapidjson::Document document;
	document.Parse((char*)buffer.data());

	if (document.HasParseError())
		return false;

	return ParseGLTF(document);
}

bool EGLTF::CEasyGLTF::LoadGLB_memory(const std::vector<uint8_t>& buffer)
{
	return ParseGLB(buffer);
}

bool EGLTF::CEasyGLTF::LoadGLB_file(const std::string& filepath)
{
	std::vector<uint8_t> buffer;
	LoadFile(filepath, buffer);

	if (buffer.size() < 1)
		return false;

	return ParseGLB(buffer);
}

bool EGLTF::CEasyGLTF::ParseGLTF(const rapidjson::Document& document)
{
	// For some weird reason, GCC still doesnt "support" this pragma. Ironically, to support this, all it needs to do is ignore it.
	// Even llvm supports this stuff...

#pragma region Asset

	if (document.HasMember("asset"))
	{
		SGLTFAsset_Prop_Asset asset;
		if (document["asset"].HasMember("version") && document["asset"]["version"].IsString())
		{
			asset.version = document["asset"]["version"].GetString();
		}
		else
			return false;

		if (document["asset"].HasMember("minVersion") && document["asset"]["minVersion"].IsString())
			asset.minVersion = document["asset"]["minVersion"].GetString();

		if (document["asset"].HasMember("generator") && document["asset"]["generator"].IsString())
			asset.generator = document["asset"]["generator"].GetString();

		if (document["asset"].HasMember("copyright") && document["asset"]["copyright"].IsString())
			asset.copyright = document["asset"]["copyright"].GetString();

		// The specs kinda allows to have whatever metadata you want in here, but we wont bother with more since these are the 4 the specs actually mention
		m_asset.asset = asset;
	}
	else
		return false; // This is the only top level field the specs actually require to be present

#pragma endregion Asset

#pragma region Buffers
	if (document.HasMember("buffers") && document["buffers"].IsArray())
	{
		for (const auto& v : document["buffers"].GetArray())
		{
			if (!v.HasMember("byteLength") || !v.HasMember("uri"))
				return false;

			SGLTFAsset_Prop_Buffer buffer;

			buffer.byteLength = v["byteLength"].GetInt();

			std::vector<uint8_t> data;

			std::string value = v["uri"].GetString();

			static std::string bufferMIMEType = "data:application/octet-stream;base64";
			size_t needlePos = value.find(bufferMIMEType);
			if (needlePos != std::string::npos)
			{
				data.resize(value.size() - bufferMIMEType.size());
				memcpy_s(data.data(), data.size(), &value[bufferMIMEType.size()], data.size());
			}
			else
			{
				std::vector<uint8_t> out;
				LoadFile(value, out);
				data.resize(out.size() - 1); // strip the null termination
				memcpy_s(data.data(), data.size(), out.data(), out.size() - 1);
			}

			buffer.data = data;

			m_asset.buffers.push_back(buffer);
		}
	}

#pragma endregion Buffers

#pragma region BufferViews

	if (document.HasMember("bufferViews") && document["bufferViews"].IsArray())
	{
		for (const auto& v : document["bufferViews"].GetArray())
		{
			if (!v.HasMember("buffer") || !v.HasMember("byteOffset") || !v.HasMember("byteLength"))
				return false;

			SGLTFAsset_Prop_BufferView bv;

			bv.buffer = v["buffer"].GetInt();
			bv.byteLength = v["bufferLength"].GetInt();
			bv.byteOffset = v["byteOffset"].GetInt();

			if (v.HasMember("byteStride"))
				bv.byteStride = v["byteStride"].GetInt();
			if (v.HasMember("target"))
				bv.target = v["target"].GetInt();

			m_asset.bufferViews.push_back(bv);
		}
	}

#pragma endregion BufferViews

#pragma region Accessors

	if (document.HasMember("accessors") && document["accessors"].IsArray())
	{
		for (const auto& v : document["accessors"].GetArray())
		{
			if (!v.HasMember("bufferView") || !v.HasMember("type") || !v.HasMember("componentType") || !v.HasMember("count") || !v.HasMember("min") || !v.HasMember("max"))
				return false;

			SGLTFAsset_Prop_Accessor accessor;

			accessor.bufferView = v["bufferView"].GetInt();
			accessor.componentType = v["componentType"].GetInt();
			accessor.count = v["count"].GetInt();
			accessor.type = v["type"].GetString();
			
			if (!v["min"].IsArray() || !v["max"].IsArray())
				return false;

			for (const auto& vv : v["min"].GetArray())
			{
				accessor.min.push_back(vv.GetDouble());
			}

			for (const auto& vv : v["max"].GetArray())
			{
				accessor.max.push_back(vv.GetDouble());
			}

			if (v.HasMember("byteOffset"))
				accessor.byteOffset = v["byteOffset"].GetInt();

			if (v.HasMember("sparse"))
			{
				const auto& vv = v["sparse"];

				if (!vv.HasMember("count") || !vv.HasMember("indices") ||
				    !vv["indices"].HasMember("bufferView") || !vv["indices"].HasMember("componentType") ||
				    !vv.HasMember("values") || !vv["values"].HasMember("bufferView"))
					return false;

				SGLTFAsset_Prop_Accessor_Sparse as;
				as.count = vv["count"].GetInt();
				as.values = vv["values"]["bufferView"].GetInt();
				as.indices = std::make_pair(vv["indices"]["bufferView"].GetInt(), vv["indices"]["componentType"].GetInt());

				accessor.sparse = as;
			}

			m_asset.accessors.push_back(accessor);
		}
	}

#pragma endregion Accessors

#pragma region Materials

	if (document.HasMember("materials") && document["materials"].IsArray())
	{
		for (const auto& v : document["materials"].GetArray())
		{
			SGLTFAsset_Prop_Material mat;

			// In a move that suprised literally no one, the standard doesnt really say which fields are actually necessary...
			// So, I am going to allow the model to omit anything they want. Dont blame me, blame the standards.

			// This is not necessary from what I can tell and it surely is not needed to use a mat
			if (v.HasMember("name") && v["name"].IsString())
				mat.name = v["name"].GetString();

			// I think this is actually needed but wth
			if (v.HasMember("pbrMetallicRoughness"))
			{
				SGLTFAsset_Prop_Material_MRM pbrMRM = {};

				if (v["pbrMetallicRoughness"].HasMember("baseColorTexture"))
				{
					SGLTFAsset_Prop_Material_Texture tex;

					if (!v["pbrMetallicRoughness"]["baseColorTexture"].HasMember("index"))
						return false;
					tex.index = v["pbrMetallicRoughness"]["baseColorTexture"]["index"].GetInt();

					if (v["pbrMetallicRoughness"]["baseColorTexture"].HasMember("texCoord"))
					{
						size_t tc = v["pbrMetallicRoughness"]["baseColorTexture"]["texCoord"].GetInt();
						if (tc == 0 || tc == 1)
							tex.texCoord = tc;
						else
							return false;
					}
					else
						tex.texCoord = 0;

					pbrMRM.baseColorTexture = tex;
				}

				if (v["pbrMetallicRoughness"].HasMember("baseColorFactor"))
				{
					if (!v["pbrMetallicRoughness"]["baseColorFactor"].IsArray())
						return false;

					const auto& vv = v["pbrMetallicRoughness"]["baseColorFactor"].GetArray();
					if (vv.Size() != 4)
						return false;

					for (rapidjson::SizeType i = 0; i < 4; ++i)
					{
						double val = vv[i].GetDouble();
						if (val > 1.0f || val < 0.0f)
							return false;
						pbrMRM.baseColorFactor[i] = val;
					}
				}
				
				if (v["pbrMetallicRoughness"].HasMember("metallicRoughnessTexture"))
				{
					SGLTFAsset_Prop_Material_Texture tex;

					if (!v["pbrMetallicRoughness"]["metallicRoughnessTexture"].HasMember("index"))
						return false;
					tex.index = v["pbrMetallicRoughness"]["metallicRoughnessTexture"]["index"].GetInt();

					if (v["pbrMetallicRoughness"]["metallicRoughnessTexture"].HasMember("texCoord"))
					{
						size_t tc = v["pbrMetallicRoughness"]["metallicRoughnessTexture"]["texCoord"].GetInt();
						if (tc == 0 || tc == 1)
							tex.texCoord = tc;
						else
							return false;
					}
					else
						tex.texCoord = 0;

					pbrMRM.metallicRoughnessTexture = tex;
				}

				if (v["pbrMetallicRoughness"].HasMember("metallicFactor"))
				{
					if (!v["pbrMetallicRoughness"]["metallicFactor"].IsDouble())
						return false;

					// Maybe these were supposed to be between 0.0 and 1.0?
					pbrMRM.metallicFactor = v["pbrMetallicRoughness"]["metallicFactor"].GetDouble();
				}

				if (v["pbrMetallicRoughness"].HasMember("roughnessFactor"))
				{
					if (!v["pbrMetallicRoughness"]["roughnessFactor"].IsDouble())
						return false;

					// Maybe these were supposed to be between 0.0 and 1.0?
					pbrMRM.roughnessFactor = v["pbrMetallicRoughness"]["roughnessFactor"].GetDouble();
				}

				mat.pbrMetallicRoughness = pbrMRM;
			}

			if (v.HasMember("normalTexture"))
			{
				if (!v["normalTexture"].HasMember("scale") || !v["normalTexture"].HasMember("index"))
					return false;

				if (!v["normalTexture"]["scale"].IsDouble() || !v["normalTexture"]["index"].IsInt())
					return false;

				SGLTFAsset_Prop_Material_Texture_NT tex;
				// Again, are these supposed to be between 0.0 and 1.0?
				tex.scale = v["normalTexture"]["scale"].GetDouble();
				tex.index = v["normalTexture"]["index"].GetInt();

				if (v["normalTexture"].HasMember("texCoord") && v["normalTexture"]["texCoord"].IsInt())
				{
					size_t tc = v["normalTexture"]["texCoord"].GetInt();
					if (tc == 0 || tc == 1)
						tex.texCoord = tc;
					else
						return false;
				}
				else
					tex.texCoord = 0;

				mat.normalTexture = tex;
			}

			if (v.HasMember("occlusionTexture"))
			{
				if (!v["occlusionTexture"].HasMember("strength") || !v["occlusionTexture"].HasMember("index"))
					return false;

				if (!v["occlusionTexture"]["strength"].IsDouble() || !v["occlusionTexture"]["index"].IsInt())
					return false;

				SGLTFAsset_Prop_Material_Texture_OT tex;
				// Again, are these supposed to be between 0.0 and 1.0?
				tex.strength = v["occlusionTexture"]["strength"].GetDouble();
				tex.index = v["occlusionTexture"]["index"].GetInt();

				if (v["occlusionTexture"].HasMember("texCoord") && v["occlusionTexture"]["texCoord"].IsInt())
				{
					size_t tc = v["occlusionTexture"]["texCoord"].GetInt();
					if (tc == 0 || tc == 1)
						tex.texCoord = tc;
					else
						return false;
				}
				else
					tex.texCoord = 0;

				mat.occlusionTexture = tex;
			}

			if (v.HasMember("emissiveTexture"))
			{
				SGLTFAsset_Prop_Material_Texture tex;

				if (!v["emissiveTexture"].HasMember("index"))
					return false;
				tex.index = v["emissiveTexture"]["index"].GetInt();

				if (v["emissiveTexture"].HasMember("texCoord"))
				{
					size_t tc = v["emissiveTexture"]["texCoord"].GetInt();
					if (tc == 0 || tc == 1)
						tex.texCoord = tc;
					else
						return false;
				}
				else
					tex.texCoord = 0;

				mat.emissiveTexture = tex;
			}

			if (v.HasMember("emissiveFactor"))
			{
				if (!v["emissiveFactor"].IsArray())
					return false;

				const auto& vv = v["emissiveFactor"].GetArray();
				if (vv.Size() != 3)
					return false;

				for (rapidjson::SizeType i = 0; i < 3; ++i)
				{
					double val = vv[i].GetDouble();
					if (val > 1.0f || val < 0.0f)
						return false;
					mat.emissiveFactor[i] = val;
				}
			}

			m_asset.materials.push_back(mat);
		}
	}

#pragma endregion Materials

#pragma region Textures

	// Pretty sure this is needed but whatever
	if (document.HasMember("textures") && document["textures"].IsArray())
	{
		for (const auto& v : document["textures"].GetArray())
		{
			if (!v.HasMember("source") || v.HasMember("sampler"))
				return false;

			SGLTFAsset_Prop_Texture tex;
			tex.source = v["source"].GetInt();
			tex.sampler = v["sampler"].GetInt();

			m_asset.textures.push_back(tex);
		}
	}

#pragma endregion Textures

#pragma region Images

	if (document.HasMember("images") && document["images"].IsArray())
	{
		for (const auto& v : document["images"].GetArray())
		{
			SGLTFAsset_Prop_Image image;
			if (v.HasMember("uri"))
			{
				std::string value = v["uri"].GetString();
				static std::string jpegMIMEType = "data:image/jpeg;base64";
				size_t needlePosJpeg = value.find(jpegMIMEType);
				static std::string pngMIMEType = "data:image/png;base64";
				size_t needlePosPng = value.find(pngMIMEType);

				if (needlePosJpeg != std::string::npos)
				{
					image.data.resize(value.size() - jpegMIMEType.size());
					memcpy_s(image.data.data(), image.data.size(), &value[jpegMIMEType.size()], image.data.size());
				}
				else if (needlePosPng != std::string::npos)
				{
					image.data.resize(value.size() - pngMIMEType.size());
					memcpy_s(image.data.data(), image.data.size(), &value[pngMIMEType.size()], image.data.size());
				}
				else
				{
					std::vector<uint8_t> out;
					LoadFile(value, out);
					image.data.resize(out.size() - 1); // strip the null termination
					memcpy_s(image.data.data(), image.data.size(), out.data(), out.size() - 1);
				}
			}
			else
			{
				if (!v.HasMember("bufferView") || !v.HasMember("mimeType"))
					return false;

				image.bufferView = v["bufferView"].GetInt();
				image.mimeType = v["mimeType"].GetString();
			}

			m_asset.images.push_back(image);
		}
	}

#pragma endregion Images

#pragma region Samplers
	
	if (document.HasMember("samplers") && document["samplers"].IsArray())
	{
		for (const auto& v : document["samplers"].GetArray())
		{
			if (!v.HasMember("magFilter") || !v.HasMember("minFilter") || !v.HasMember("wrapT") || !v.HasMember("wrapS"))
				return false;

			SGLTFAsset_Prop_Sampler sampler;
			sampler.magFiler = v["magFilter"].GetInt();
			sampler.magFiler = v["minFilter"].GetInt();
			sampler.magFiler = v["wrapT"].GetInt();
			sampler.magFiler = v["wrapS"].GetInt();

			m_asset.samplers.push_back(sampler);
		}
	}

#pragma endregion Samplers

#pragma region Meshes

	if (document.HasMember("meshes") && document["meshes"].IsArray())
	{
		for (const auto& v : document["meshes"].GetArray())
		{
			if (!v.HasMember("name") || !v.HasMember("primitives") || !v["primitives"].IsArray())
				return false;

			SGLTFAsset_Prop_Mesh mesh;
			
			for (const auto& vv : v["primitives"].GetArray())
			{
				if (!vv.HasMember("mode") || !vv.HasMember("indices") || !vv.HasMember("attributes") || !vv.HasMember("material"))
					return false;

				SGLTFAsset_Prop_Mesh_Primitive meshPrimitive;

				meshPrimitive.mode = vv["mode"].GetInt();
				meshPrimitive.indices = vv["indices"].GetInt();
				meshPrimitive.material = vv["material"].GetInt();

				for (auto& iter = vv["attributes"].MemberBegin(); iter != vv["attributes"].MemberEnd(); ++iter)
					meshPrimitive.attributes.emplace(iter->name.GetString(), iter->value.GetInt());

				if (vv.HasMember("targets") && vv["targets"].IsArray())
				{
					for (const auto& vvv : vv["targets"].GetArray())
					{
						TGLTFAsset_Prop_Mesh_Primitive_Attributes attr;

						for (auto& iter = vvv.MemberBegin(); iter != vvv.MemberEnd(); ++iter)
							attr.emplace(iter->name.GetString(), iter->value.GetInt());

						meshPrimitive.targets.push_back(attr);
					}
				}

				mesh.primitives.push_back(meshPrimitive);
			}

			if (v.HasMember("weights") && v["weights"].IsArray())
			{
				for (const auto& vv : v["weights"].GetArray())
					mesh.weights.push_back(vv.GetDouble());
			}

			m_asset.meshes.push_back(mesh);
		}
	}

#pragma endregion Meshes

#pragma region Nodes
	
	if (document.HasMember("nodes") && document["nodes"].IsArray())
	{
		for (const auto& v : document["nodes"].GetArray())
		{
			SGLTFAsset_Prop_Node node;

			if (v.HasMember("children"))
			{
				if (v["children"].IsArray())
					return false;

				for (const auto& vv : v["children"].GetArray())
					node.children.push_back(vv.GetInt());
			}

			if (v.HasMember("matrix"))
			{
				if (!v["matrix"].IsArray())
					return false;

				for (rapidjson::SizeType i = 0; i < v["matrix"].Capacity(); ++i)
					node.matrix[i] = v["matrix"][i].GetDouble();
			}
			else if (v.HasMember("translation") && v.HasMember("scale") && v.HasMember("rotation"))
			{
				if (!v["translation"].IsArray() || !v["scale"].IsArray() || !v["rotation"].IsArray())
					return false;

				const auto& translation = v["translation"].GetArray();
				const auto& scale = v["scale"].GetArray();
				const auto& rotation = v["rotation"].GetArray();

				static const std::array<double, 16> identityMatrix = {
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				};

				std::array<double, 16> translationMatrix = identityMatrix;
				translationMatrix[3] = translation[0].GetDouble();
				translationMatrix[7] = translation[1].GetDouble();
				translationMatrix[1] = translation[2].GetDouble();

				std::array<double, 16> scaleMatrix = identityMatrix;
				scaleMatrix[0] = scale[0].GetDouble();
				scaleMatrix[5] = scale[1].GetDouble();
				scaleMatrix[10] = scale[2].GetDouble();

				// rotation quarternion
				double qx = rotation[0].GetDouble();
				double qy = rotation[1].GetDouble();
				double qz = rotation[2].GetDouble();
				double qw = rotation[3].GetDouble();

#define SQR(x) (x * x)

				const double normalizationVal = 1.0f / std::sqrt(SQR(qx) + SQR(qy) + SQR(qz) + SQR(qw));
				qx *= normalizationVal;
				qy *= normalizationVal;
				qz *= normalizationVal;
				qw *= normalizationVal;

				std::array<double, 16> rotationMatrix = {
					1.0f - 2.0f * qy * qy - 2.0f * qz * qz, 2.0f * qx * qy - 2.0f * qz * qw, 2.0f * qx * qz + 2.0f * qy * qw, 0.0f,
					2.0f * qx * qy + 2.0f * qz * qw, 1.0f - 2.0f * qx * qx - 2.0f * qz * qz, 2.0f * qy * qz - 2.0f * qx * qw, 0.0f,
					2.0f * qx * qz - 2.0f * qy * qw, 2.0f * qy * qz + 2.0f * qx * qw, 1.0f - 2.0f * qx * qx - 2.0f * qy * qy, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f,
				};

				// M = T * R * S
				std::array<double, 16> TR = MatxMat(translationMatrix, rotationMatrix);
				node.matrix = MatxMat(TR, scaleMatrix);

				// No regrets
			}

			if (v.HasMember("mesh"))
				node.mesh = v["mesh"].GetInt();

			if (v.HasMember("camera"))
				node.camera = v["camera"].GetInt();

			if (v.HasMember("skin"))
				node.skin = v["skin"].GetInt();

			if (v.HasMember("name"))
				node.name = v["name"].GetInt();

			m_asset.nodes.push_back(node);
		}
	}

#pragma endregion Nodes

	return true;
}

bool EGLTF::CEasyGLTF::ParseGLB(const std::vector<uint8_t>& buffer)
{
	return true;
}
