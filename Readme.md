this is my repo with my take on the amazing [Joey de Vries' www.learnopengl.com tutorials](https://learnopengl.com).

using these dependencies:
* [SDL2](https://www.libsdl.org/)
* [Assimp](https://assimp.org/)
* [stb\_image](https://github.com/nothings/stb)
* [GLAD](https://github.com/Dav1dde/glad)
* [GLM](https://github.com/g-truc/glm)
* ~[Soil2](https://github.com/SpartanJ/soil2)~ (do not need for now, no longer using it)

using the vcpkg package manager to automate installation of these.

vcpkg does not really work for me(yet?) on windows+gcc/clang.

I include a subrepository copy of glad and glm to make my life easier.

```
git clone git@github.com:pattakosn/stelar3D.git --recurse-submodules
git checkout --recurse-submodules
git submodule init
git submodule update
git submodule update --remote
```

Windows:
```
cmake --build .  --config Debug -j 12

.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg.exe integrate project
.\vcpkg\vcpkg.exe install --triplet=x64-windows --triplet=x64-mingw-static sdl2 glad glm assimp soil2
cmake -DCMAKE_TOOLCHAIN_FILE=../external/vcpkg/scripts/buildsystems/vcpkg.cmake ..

cmake -DCMAKE_TOOLCHAIN_FILE="D:/vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_OVERLAY_TRIPLETS="../cmake" -DVCPKG_HOST_TRIPLET="x64-windows-vs2022" -DVCPKG_TARGET_TRIPLET="x64-windows-vs2022" ../
cmake -DCMAKE_TOOLCHAIN_FILE="D:/vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_HOST_TRIPLET="x64-mingw-dynamic" -DVCPKG_TARGET_TRIPLET="x64-mingw-dynamic" ../

export VCPKG_ROOT=/d/learnopengl-mine/vcpkg

export HUNTER_ROOT=/d/hunter
```

Fedora:
```
sudo dnf install libXi-devel libXxf86vm-devel
./vcpkg/vcpkg install sdl2:x64-linux glad:x64-linux glm:x64-linux  assimp:x64-linux soil2:x64-linux
```

## Code snippets to use or delete
```
#define GLM_FORCE_RADIANS 1
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_FORCE_MESSAGES
#define GLM_FORCE_INLINE
```


## 3D graphics resources
* [SketchFab](https://sketchfab.com/)
* [SO OpenGL documentation](https://sodocumentation.net/opengl)
* [Khronos common opengl mistakes](https://www.khronos.org/opengl/wiki/Common\_Mistakes)
* [apitrace and references to other 3D profiling tools](https://apitrace.github.io/)
* [ImageDev ThermoFisher Library](https://developer.imageviz.com/)
* [Aeon Engine](https://github.com/aeon-engine) (c++14/OpenGL3 game engine)
* [Vulkan examples](https://github.com/SaschaWillems/Vulkan)
* [SIMDstring](https://github.com/RobloxResearch/SIMDString) std string compatible replacement using simd

## 3D graphics techniques list
tangent space
 * lighting maps
   - diffuse maps
   - specular maps 
 * emission maps
 * reflection maps
 * shadow volumes (stencil buffer technique for real-time shadows)
 * post-processing filter
   - Gaussian BLur
 * Super Sample Anti-Aliasing (SSAA) - render in bigger resolution and downsample to final resolution
 * MultiSample Anti-Aliasing (MSAA)
 * [OpenGL Direct State Access](https://www.khronos.org/opengl/wiki/Direct\_State\_Access)
 * (nvidia?) opengl command lists

### texture mapping techniques 
 * local maps:
   * displacement mapping
   * normal mapping
   * parallax mapping 
   * bump mapping
   * UV mapping
   * UVW mapping
   * relief mapping
   * alpha mapping
   * occlusion mapping
   * specular mapping
 * environment maps:
   * Cube mapping
   * Sphere mapping
   * Environment mapping

   
### Phong (in fragment shaders - Gouraud when in vertex shaders)
 * Ambient lighting
 * Diffuse lighting
 * Specular lighting
 
### Light casters
 * Directional light
 * Point light
 * Spot light

# Tone Mapping
**HDR** (high dynamic range) are converted to **LDR** using **tone mapping** algorithms.

Reinhard tone mapping : simplest tone mapping algorithm
automatic exposure adjustment or eye adaptation

# GPU Generations

## Nvidia
GTX 200

Fermi GTX 400
GTX 700
Maxwell GTX 900
RTX 2000
Ampere GA102 RTX 3000

Lovelace AD102 RTX 5000

## AMD ATI
* RX 6000
                | Navi 24    | Navi 23          | Navi 22       | Navi 21
Codename        | Beige Goby | Dimgrey Cavefish | Navy Flounder | Sienna Cichlid ("big navi" )
Models          | RX 6400, 6500XT | RX 6600, 6600XT | RX 6700XT | RX 6800/XT 6900XT
 | 1 raster engine, 16 shader cluster, 1024 FP32, 
PCI Express     | PCIE 4.0 x4 | PCIE 4.0 x8 | PCIE 4.0 x8 | PCIE 4.0 x8 
decode H.264/4K, H.265 | X | X | X | X 
decode AV1             |  | X | X | X
encode H.264/4K, H.265 |  | X | X | X 
Process         | TSMC 6nm | TSMC 7nm | TSMC 7nm | TSMC 7nm 
Transistor count | 5.4bil | 11.1bil | 17.2bil | 26.8bil
chipsize | 141mm^2 (38.3 mil/mm^2) | 236mm^2 (47 mil/mm^2) | 335mm^2 (51.3mil/mm^2) | 519mm^2 (51.6mil/mm^2)


