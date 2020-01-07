# code-test-c--1
c++ code test assignment

Configuring

- Use CMake gui utility to configure the project
 - for CMake to fin Qt binaries CMAKE_PREFIX_PATH entry need to be added
 example: "C:/Qt/Qt5.12.6/5.12.6/msvc2015_64/lib/cmake"
 - Press "Configure"
 - Pres "Generate"
 
 Building
- There will be Visual Studio project solution generated in the "build" directory (defined earlier in CMake gui)
- Open Symbio-CodeTest solution file
- Select and right-click the "symbioDemo" module and select "Set as StartUp Project"
- Press F5 to build and run the application

Using
- In the program window press "New" to create new account
- There will be popup shown to input data for the new account
- To delete existing account click account id under the "User id" column and press "Delete"
- To view account details click account id under the "User id" column and press "Details"
