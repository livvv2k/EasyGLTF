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

#pragma once

#include "rapidjson/fwd.h"

#include <string>
#include <vector>
#include <array>
#include <map>

namespace EGLTF
{
	struct SGLB_HEADER
	{
		uint32_t magic;
		uint32_t version;
		uint32_t length;
	};

	struct SGLB_CHUNK
	{
		uint32_t chunkLength;
		uint32_t chunkType;
		std::vector<uint8_t> chunkData;
	};

	struct SGLTFAsset_Prop_Asset
	{
		std::string version;
		std::string generator;
		std::string minVersion;
		std::string copyright;
	};

	struct SGLTFAsset_Prop_Scene
	{
		std::vector<int32_t> nodes;
	};

	struct SGLTFAsset_Prop_Node
	{
		std::vector<int32_t> children;
		std::array<double, 16> matrix; // this is calculated in cases where translation, rotation and scale properties are given as seperate attribs
		int32_t mesh = -1;
		int32_t skin = -1;
		int32_t camera = -1;
		std::string name;
	};

	typedef std::map<std::string, int32_t> TGLTFAsset_Prop_Mesh_Primitive_Attributes;

	struct SGLTFAsset_Prop_Mesh_Primitive
	{
		int32_t mode = -1;
		int32_t indices = -1;
		int32_t material = -1;
		TGLTFAsset_Prop_Mesh_Primitive_Attributes attributes; // the specs were not clear on what these are or how many there can be...
		std::vector<TGLTFAsset_Prop_Mesh_Primitive_Attributes> targets;
	};

	struct SGLTFAsset_Prop_Mesh
	{
		std::string name;
		std::vector<SGLTFAsset_Prop_Mesh_Primitive> primitives;
		std::vector<double> weights;
	};

	struct SGLTFAsset_Prop_Buffer
	{
		int32_t byteLength = -1;
		std::vector<uint8_t> data;
	};

	struct SGLTFAsset_Prop_BufferView
	{
		int32_t buffer = -1;
		int32_t byteOffset = -1;
		int32_t byteLength = -1;
		int32_t byteStride = -1;
		int32_t target = -1;
	};

	struct SGLTFAsset_Prop_Accessor_Sparse
	{
		int32_t count = -1;
		int32_t values = -1; // bufferViewindex
		std::pair<int32_t, int32_t> indices; // bufferView, componentType
	};

	struct SGLTFAsset_Prop_Accessor
	{
		int32_t bufferView = -1;
		int32_t byteOffset = -1;
		std::string type;
		int32_t componentType = -1;
		int32_t count = -1;
		std::vector<double> min;
		std::vector<double> max;
		SGLTFAsset_Prop_Accessor_Sparse sparse;
	};
	
	struct SGLTFAsset_Prop_Material_Texture
	{
		int32_t index = -1;
		int32_t texCoord = -1; // This is the number that determines theTEXCOORD_<n>, 1 or 0. 0 is the default
	};

	struct SGLTFAsset_Prop_Material_Texture_NT
	{
		double scale;
		int32_t index = -1;
		int32_t texCoord = -1;
	};

	struct SGLTFAsset_Prop_Material_Texture_OT
	{
		double strength;
		int32_t index = -1;
		int32_t texCoord = -1;
	};

	// GLTF uses the Metallic-Roughness Model from Physically-based rendering
	struct SGLTFAsset_Prop_Material_MRM
	{
		std::array<double, 4> baseColorFactor; // RGBA
		SGLTFAsset_Prop_Material_Texture baseColorTexture;
		SGLTFAsset_Prop_Material_Texture metallicRoughnessTexture;
		double metallicFactor;
		double roughnessFactor;
	};

	struct SGLTFAsset_Prop_Material
	{
		std::string name;
		SGLTFAsset_Prop_Material_MRM pbrMetallicRoughness;
		std::array<double, 3> emissiveFactor;
		SGLTFAsset_Prop_Material_Texture_OT occlusionTexture;
		SGLTFAsset_Prop_Material_Texture_NT normalTexture;
		SGLTFAsset_Prop_Material_Texture emissiveTexture;
	};

	struct SGLTFAsset_Prop_Texture
	{
		int32_t source = -1;
		int32_t sampler = -1;
	};

	enum class SGLTFAsset_Prop_Camera_Type
	{
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

	struct SGLTFAsset_Prop_Camera
	{
		SGLTFAsset_Prop_Camera_Type type;
		int32_t zfar = -1;
		double znear;

		// TODO: std::optional
		
		// Meaning of these two variables change according to type
		             // orth	|	perspective

		double val0; // xmag	|	aspectRatio
		double val1; // ymag	|	yfov
	};

	struct SGLTFAsset_Prop_Image
	{
		std::vector<uint8_t> data;
		// Or
		int32_t bufferView = -1;
		std::string mimeType;
	};

	struct SGLTFAsset_Prop_Skin
	{
		int32_t inverseBindMatrices = -1;
		std::vector<int32_t> joints;
		int32_t skeleton = -1;
		std::string name;
	};

	enum class EGLTFAsset_Prop_Animation_Channel_Target_Type
	{
		TRANSLATION,
		ROTATION,
		SCALE,
		WEIGHTS
	};

	struct SGLTFAsset_Prop_Animation_Channel_Target
	{
		int32_t node = -1;
		EGLTFAsset_Prop_Animation_Channel_Target_Type path;
	};

	struct SGLTFAsset_Prop_Animation_Channel
	{
		SGLTFAsset_Prop_Animation_Channel_Target target;
		int32_t sampler = -1;
	};

	enum class EGLTFAsset_Prop_Animation_Sampler_Type
	{
		LINEAR,
		STEP,
		CUBICSPLINE
	};

	struct SGLTFAsset_Prop_Animation_Sampler
	{
		int32_t input = -1;
		int32_t output = -1;
		EGLTFAsset_Prop_Animation_Sampler_Type interpolation;
	};

	struct SGLTFAsset_Prop_Animation
	{
		std::string name;
		std::vector<SGLTFAsset_Prop_Animation_Channel> channels;
		std::vector<SGLTFAsset_Prop_Animation_Sampler> samplers;
	};

	struct SGLTFAsset_Prop_Sampler
	{
		int32_t magFiler = -1;
		int32_t minFiler = -1;
		int32_t wrapS = -1;
		int32_t wrapT = -1;
	};

	struct SGLTFAsset
	{
		SGLTFAsset_Prop_Asset asset;
		std::string scene; // default scene
		std::vector<SGLTFAsset_Prop_Scene> scenes;
		std::vector<SGLTFAsset_Prop_Mesh> meshes;
		std::vector<SGLTFAsset_Prop_Buffer> buffers;
		std::vector<SGLTFAsset_Prop_BufferView> bufferViews;
		std::vector<SGLTFAsset_Prop_Accessor> accessors;
		std::vector<SGLTFAsset_Prop_Material> materials;
		std::vector<SGLTFAsset_Prop_Texture> textures;
		std::vector<SGLTFAsset_Prop_Camera> cameras;
		std::vector<SGLTFAsset_Prop_Image> images;
		std::vector<SGLTFAsset_Prop_Skin> skins;
		std::vector<SGLTFAsset_Prop_Animation> animations;
		std::vector<SGLTFAsset_Prop_Sampler> samplers;
		std::vector<SGLTFAsset_Prop_Node> nodes;
	};

	class CEasyGLTF
	{
	public:
		CEasyGLTF();
		~CEasyGLTF();

		bool LoadGLTF_file(const std::string& filepath);
		bool LoadGLTF_memory(const std::vector<uint8_t>& buffer);

		bool LoadGLB_memory(const std::vector<uint8_t>& buffer);
		bool LoadGLB_file(const std::string& filepath);

	private:
		bool ParseGLTF(const rapidjson::Document& document);
		bool ParseGLB(const std::vector<uint8_t>& buffer);

		SGLTFAsset m_asset;

		std::string m_path; // For non-embedded .gltf files, also, std::optional

		std::vector<uint8_t> m_binaryBuffer; // For glb file's binary chunk, also, std::optional here as well
	};
}
