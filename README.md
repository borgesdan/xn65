﻿# XN65

> XN65 is an implementation of Microsoft XNA 4.0 in C++ with DirectX 11 and the DirectXTK library.

### Adjustments and improvements

The project is still under development and the next updates will focus on the following tasks:
- [ ] Finish basic classes
- [ ] Code refactoring and cleaning
- [ ] 3D support
- [ ] Content Pipeline
- [ ] Multiplatform support

## 💻 Requirements
- `DirectX 11`: Currently X65 only supports DirectX 11
- `vcpkg`: C/C++ dependency manager from Microsoft 
- `DirectXTK`: The DirectX Tool Kit

## 🚀 DirectXTK and VCPKG

Use the following command in vcpkg to install DirectXTK:

```
.\vcpkg install directxtk[tools,spectre,xaudio2-9]
```

In the root CMakeLists.txt set the vcpkg CMAKE_TOOLCHAIN_FILE path

```
include("C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
```

## 📝 Licença

Esse projeto está sob licença. Veja o arquivo [LICENÇA](LICENSE.md) para mais detalhes.