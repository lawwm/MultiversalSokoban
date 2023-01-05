# Installation Guide #

## Download MultiversalSokoban ##
`git clone 'https://github.com/lawwm/MultiversalSokoban.git'`

### Download required SDL dependencies from `https://github.com/libsdl-org/SDL/releases/tag/release-2.26.1` or `https://www.libsdl.org/release/`
1) SDL2-devel-2.26.1-VC.zip: `https://www.libsdl.org/release/SDL2-devel-2.26.1-VC.zip` 
2) SDL2_ttf-devel-2.20.1-VC.zip: `https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.20.1-VC.zip`
3) SDL2_mixer-devel-2.6.2-VC.zip: `https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.6.2-VC.zip`
4) SDL2_image-devel-2.6.2-VC.zip: `https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.6.2-VC.zip`

### Extract all downloads
1) Extract all downloads
2) From SDL2 folder, navigate to the include folder and copy all files, and paste in jumper folder of MultiversalSokoban folder
3) From SDL2_ttf folder, navigate to the include folder and copy SDL_ttf.h, and paste in jumper folder of MultiversalSokoban folder
4) From SDL2_image folder, navigate to the include folder and copy SDL_image.h, and paste in jumper folder of MultiversalSokoban folder


### Running and debugging
1) Right Click jumper.sln in the main directory > open with File Explorer > paste the .dll files here
    1b) get the full path of the library folders from the extracted SDL folders:
    1c) SDL2-2.26.1\lib\x64
    1d) SDL2_ttf-2.20.1\lib\x64
    1e) SDL2_image-2.6.2\lib\x64
2) under Project tab > click Properties > C/C++ > Additional Include Directories, select `<Edit..>`
    2b) select new line and add the path of the jumper folder, click ok
3) under Project tab > click Properties > Linker > General > Additional Library Directories, select `<Edit..>`
    3b) get the full path of the library folders from the extracted SDL folders, see 1b - 1e
4) select ok, click apply and ok
5) ensure `SDL2.lib;SDL2main.lib;SDL2test.lib;SDL2_image.lib;SDL2_ttf.lib` in Linker > Input > Additional Dependencies, else add to it.