
# SLRhythm

OSU! mania-lile game on 128x64 LCD in the PIC18F4550 plaftorm (VUT UMEL graphickit)
- runs at 20fps (LCD limitation)
- fixed-point math
- slightly vibecoded
- 4k gameplay
- adjustable AR, OD
- map stored in flash and streamed to RAM


## Structure

| Path                        | Purpose                                                                                                                             |
|-----------------------------|-------------------------------------------------------------------------------------------------------------------------------------|
| _build                      | The [CMake build tree](https://cmake.org/cmake/help/latest/manual/cmake.1.html#introduction-to-cmake-buildsystems), can be deleted. |
| cmake                       | Generated [CMake](https://cmake.org/) files. May be deleted if user.cmake has not been added                                        |
| .vscode                     | See [VSCode](https://code.visualstudio.com/docs/getstarted/settings)                                                                |
| .vscode/settings.json       | Workspace specific settings                                                                                                         |
| .vscode/SLRhythm.mplab.json | The MPLAB project file, should not be deleted                                                                                       |
| out                         | Final build artifacts                                                                                                               |
