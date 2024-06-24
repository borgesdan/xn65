# XN65

> XN65 is an implementation of Microsoft XNA 4.0 in C++ with DirectX 11 and the DirectXTK library.

### Adjustments and improvements

The project is still under development and the next updates will focus on the following tasks:
- [x] Finish basic classes
- [ ] Code refactoring and cleaning
- [ ] 3D support
- [ ] Implementation of missing classes and functions
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

In the root CMakeLists.txt set the vcpkg path

```
set(PROJECT_VCPKG_DIRECTORY "C:/vcpkg")
```

## 📝 License

This project is under MIT license. See [LICENSE](LICENSE.md) for more details.
