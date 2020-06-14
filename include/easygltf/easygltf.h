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
	struct SGLTFAsset_Prop_Asset
	{
		std::string version;
		std::string generator;
		std::string minVersion;
		std::string copyright;
	};

	struct SGLTFAsset_Prop_Scene
	{
		std::vector<size_t> nodes;
	};

	struct SGLTFAsset_Prop_Node
	{
		std::vector<size_t> children;
		std::array<double, 16> matrix; // this is calculated in cases where translation, rotation and scale properties are given as seperate attribs
		size_t mesh;
		size_t skin;
		size_t camera;
		std::string name;
	};

	typedef std::map<std::string, size_t> TGLTFAsset_Prop_Mesh_Primitive_Attributes;

	struct SGLTFAsset_Prop_Mesh_Primitive
	{
		size_t mode;
		size_t indices;
		size_t material;
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
		size_t byteLength;
		std::vector<uint8_t> data;
	};

	struct SGLTFAsset_Prop_BufferView
	{
		size_t buffer;
		size_t byteOffset;
		size_t byteLength;
		size_t byteStride;
		size_t target;
	};

	struct SGLTFAsset_Prop_Accessor_Sparse
	{
		size_t count;
		size_t values; // bufferViewindex
		std::pair<size_t, size_t> indices; // bufferView, componentType
	};

	struct SGLTFAsset_Prop_Accessor
	{
		size_t bufferView;
		size_t byteOffset;
		std::string type;
		size_t componentType;
		size_t count;
		std::vector<double> min;
		std::vector<double> max;
		SGLTFAsset_Prop_Accessor_Sparse sparse;
	};
	
	struct SGLTFAsset_Prop_Material_Texture
	{
		size_t index;
		size_t texCoord; // This is the number that determines theTEXCOORD_<n>, 1 or 0. 0 is the default
	};

	struct SGLTFAsset_Prop_Material_Texture_NT
	{
		double scale;
		size_t index;
		size_t texCoord;
	};

	struct SGLTFAsset_Prop_Material_Texture_OT
	{
		double strength;
		size_t index;
		size_t texCoord;
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
		size_t source;
		size_t sampler;
	};

	enum class SGLTFAsset_Prop_Camera_Type
	{
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

	struct SGLTFAsset_Prop_Camera
	{
		SGLTFAsset_Prop_Camera_Type type;
		size_t zfar;
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
		size_t bufferView;
		std::string mimeType;
	};

	struct SGLTFAsset_Prop_Skin
	{
		size_t inverseBindMatrices;
		std::vector<size_t> joints;
		size_t skeleton;
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
		size_t node;
		EGLTFAsset_Prop_Animation_Channel_Target_Type path;
	};

	struct SGLTFAsset_Prop_Animation_Channel
	{
		SGLTFAsset_Prop_Animation_Channel_Target target;
		size_t sampler;
	};

	enum class EGLTFAsset_Prop_Animation_Sampler_Type
	{
		LINEAR,
		STEP,
		CUBICSPLINE
	};

	struct SGLTFAsset_Prop_Animation_Sampler
	{
		size_t input;
		size_t output;
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
		size_t magFiler;
		size_t minFiler;
		size_t wrapS;
		size_t wrapT;
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

		// Only works for gltf files with embedded data
		bool LoadGLTF_file(const std::string& filepath);
		bool LoadGLTF_memory(const std::vector<uint8_t>& buffer);

		bool LoadGLB_memory(const std::vector<uint8_t>& buffer);
		bool LoadGLB_file(const std::string& filepath);

	private:
		bool ParseGLTF(const rapidjson::Document& document);
		bool ParseGLB(const std::vector<uint8_t>& buffer);

		SGLTFAsset m_asset;
		std::string m_path; // For non-embedded .gltf files, also, std::optional
	};
}
