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

#include <easygltf/easygltf.h>

int main(int argc, char** argv)
{
	EGLTF::CEasyGLTF* easygltf = new EGLTF::CEasyGLTF();

	if (!easygltf->LoadGLB_file("Monster/glTF-Binary/Monster.glb"))
	{
		return 1;
	}

	return 0;
}
