# EasyGLTF
[![Build Status](https://travis-ci.org/livvv2k/EasyGLTF.svg?branch=master)](https://travis-ci.org/livvv2k/EasyGLTF)

C++11 library for loading and parsing glTF assets.

## Dependencies
The only dependency used is a header only library, [rapidjson](https://github.com/Tencent/rapidjson), that need not be present unless you are building the library.

## Usage
The interface supports loading GLTF and GLB files from either memory or file path.
```
EGLTF::CEasyGLTF* easygltf = new EGLTF::CEasyGLTF();
if (!easygltf->LoadGLB_file("Monster/glTF-Binary/Monster.glb"))
{
  return false;
}

// The SGLTFAsset is a higher level representation of a gltf asset from the loaded file(s).
// All the information of the loaded asset can be accessed from this struct.
EGLTF::SGLTFAsset asset = easyglt->GetAssetInstance(); // returns a const reference
```
