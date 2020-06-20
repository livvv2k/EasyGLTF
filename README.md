# EasyGLTF
[![Build Status](https://travis-ci.org/livvv2k/EasyGLTF.svg?branch=master)](https://travis-ci.org/livvv2k/EasyGLTF)

C++11 library for loading and parsing glTF assets.

## Dependencies
The only dependency used is a header only library, [rapidjson](https://github.com/Tencent/rapidjson), that need not be present unless you are building the library.

## Building
### Linux
```
mkdir out && cd out/
cmake ../
make
```
### Windows
If you have cmake tools installed with Visual Studio (You can use the installer to get it, `Tools->Get Tools and Features`), you can just open the folder with Visual Studio and carry on as if it were a sln file.
You could also use cmake to generate an sln file.

After building, copy out the `include` and `lib` folder in `output/` which would have the needed header and static library file.

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
