# CAP4730_Final_Wang_Nguyen
Compiling Instructions:
1. Go to the source code project solution and open with VS2019.
3. Go to project solution properties.
4. Change Configuration to All Configurations and Platfrom to Active(x64).
5. You should be able to run the project at this point. However if you still can't follow the steps below.
6. Configuration properties -> C/C++ -> General -> Additional Include Directories -> Paste: $(SolutionDir)\glfw-3.3.6.bin.WIN64\include;$(SolutionDir)\assimp\include;$(SolutionDir)\glew-2.1.0\include;$(SolutionDir)\glm;%(AdditionalIncludeDirectories)
7. Configuration properties -> Linker -> General -> Additional Library Directories -> Paste: $(SolutionDir)\glew-2.1.0\lib\Release\x64;$(SolutionDir)\glfw-3.3.6.bin.WIN64\lib-vc2019;$(SolutionDir)\assimp\lib;%(AdditionalLibraryDirectories)
8. Configuration properties -> Linker -> Input -> Additional Dependencies -> Paste: opengl32.lib;glew32s.lib;glfw3.lib;assimp-vc140-mt.lib;%(AdditionalDependencies)
9. Now you should be able to build and compile the project.
