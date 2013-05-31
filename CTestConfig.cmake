## This file should be placed in the root directory of your project.
## Then modify the CMakeLists.txt file in the root directory of your
## project to incorporate the testing dashboard.
## # The following are required to uses Dart and the Cdash dashboard
##   ENABLE_TESTING()
##   INCLUDE(CTest)
set(CTEST_PROJECT_NAME "gulSandbox")
set(CTEST_NIGHTLY_START_TIME "00:00:00 EST")

set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "build.ganymede.ch")
set(CTEST_DROP_LOCATION "/submit.php?project=gulSandbox")
set(CTEST_DROP_SITE_CDASH TRUE)

set(CTEST_CUSTOM_EXCLUDE 3rdParty
                         libs)

file(WRITE ${CMAKE_BINARY_DIR}/CTestCustom.cmake 
           "set(CTEST_CUSTOM_COVERAGE_EXCLUDE ${CTEST_CUSTOM_EXCLUDE})")

unset(CTEST_CUSTOM_EXCLUDE)
