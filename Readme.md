this is my repo with my take on the amazing [Joey de Vries' www.learnopengl.com tutorials](https://learnopengl.com).

using these dependencies:
* [SDL2](https://www.libsdl.org/)
* [Assimp](https://assimp.org/)
* [stb\_image](https://github.com/nothings/stb)
* ~ [GLAD](https://github.com/Dav1dde/glad) ~
* ~ [GLM](https://github.com/g-truc/glm) ~
* ~ [Soil2](https://github.com/SpartanJ/soil2) ~
using the vcpkg package manager to automate installation of these.
vcpkg does not really work for me on windows+gcc/clang. soil2 i do not need for now so i am no longer including it. I also include a hard copy of glad and glm to make my life easier.

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
.\vcpkg\vcpkg.exe install sdl2:x64-windows
.\vcpkg\vcpkg.exe install glad:x64-windows
.\vcpkg\vcpkg.exe install glm:x64-windows
.\vcpkg\vcpkg.exe install assimp:x64-windows
.\vcpkg\vcpkg.exe install soil2:x64-windows
.\vcpkg\vcpkg.exe install --triplet=x64-windows --triplet=x64-mingw-static sdl2 glad glm assimp soil2

export VCPKG_ROOT=/d/learnopengl-mine/vcpkg
export VCPKG_FEATURE_FLAGS="-manifests"
cmake ../ -DCMAKE_TOOLCHAIN_FILE=/c/vcpkg/scripts/buildsystems/vcpkg.cmake -A x64
cmake -DVCPKG_TARGET_TRIPLET=x64-mingw-static/dynamic -DVCPKG_HOST_TRIPLET=x64-mingw-static/dynamic ..
 .\vcpkg.exe integrate project
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
[SketchFab](https://sketchfab.com/)
[SO OpenGL documentation](https://sodocumentation.net/opengl)
[Khronos common opengl mistakes](https://www.khronos.org/opengl/wiki/Common_Mistakes)
[apitrace and references to other 3D profiling tools](https://apitrace.github.io/)
[ImageDev ThermoFisher Library](https://developer.imageviz.com/)
[Aeon Engine(c++14/OpenGL3 game engine)] (https://github.com/aeon-engine)
[Vulkan examples](https://github.com/SaschaWillems/Vulkan)

## 3D graphics techniques list
 * lighting maps
     - diffuse maps
     - specular maps 
 * emission maps
 * normal maps
 * bump maps
 * reflection maps
 * shadow volumes (stencil buffer technique for real-time shadows)
 * post-processing filter
   - Gaussian BLur
 * Super Sample Anti-Aliasing (SSAA) - render in bigger resolution and downsample to final resolution
 * MultiSample Anti-Aliasing (MSAA)
 * [OpenGL Direct State Access](https://www.khronos.org/opengl/wiki/Direct_State_Access)
 * (nvidia?) opengl command lists

### Phong (in fragment shaders - Gouraud when in vertex shaders)
 * Ambient lighting
 * Diffuse lighting
 * Specular lighting
 
### Light casters
 * Directional light
 * Point light
 * Spot light

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


