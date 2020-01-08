# code-test-c--1
c++ code test assignment

Configuring

- Clone project from git repository https://github.com/alexey7733/code-test-c-1.git

- Launch CMamke gui and choose Visual Studio 2014 (has to be installed earlier) and native compiler settings

- Use CMake gui utility to configure the project (tested on Windows 10)
 - navigate to the source directory of the project in the CMake gui
 - setup build directory in the CMake gui
 - for CMake to fin Qt binaries CMAKE_PREFIX_PATH entry need to be added
 example: "C:/Qt/Qt5.12.6/5.12.6/msvc2015_64/lib/cmake"
 - press "Configure"
 - pres "Generate"
 
 Build and run
- There will be Visual Studio project solution generated in the "build" directory (defined earlier in CMake gui)
- Open Symbio-CodeTest solution file
- Select and right-click the "symbioDemo" module and select "Set as StartUp Project"
- Press F5 to build and run the application

Using
- In the program window press "New" to create new account
- There will be popup shown to input data for the new account
- To delete existing account click account id under the "User id" column and press "Delete"
- To view account details click account id under the "User id" column and press "Details"

Unit test for database module
- There is a macro definition in db.h file
- To activate unit test uncomment on line 8 in db.h file

//#define UNIT_TEST_DB

- After that build and run program normally
- In the console you will see otput of the unit test

Documentation
- There is HTML documentation in the doc/doxygen/html folder ad the project root
 - click index.html file to see documented classes and interfaces

