cmake_minimum_required(VERSION 3.6)
project(Render)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(    SOURCE_FILES
        main.cpp
        ServerAWS.cpp
        ServerAWS.h
        Cluster.cpp
        Cluster.h
        json.hpp
        Util.h
        Util.cpp
        RenderJob.cpp
        RenderJob.h
        Animations/Animation.cpp
        Animations/Animation.h
        Animations/FractParam.cpp
        Animations/FractParam.h
        Animations/Light.cpp
        Animations/Light.h
        Animations/Camera.cpp
        Animations/Camera.h
        Animations/Paths/Path.cpp
        Animations/Paths/Path.h
        Animations/Paths/Path.cpp
        Animations/Color.cpp
        Animations/Color.h
        Animations/Plotter.cpp
        Animations/Plotter.h
        Animations/Catmul.cpp
        Animations/Catmul.h
        Libs/muparser-2.2.5/src/muParser.cpp
        Libs/muparser-2.2.5/src/muParserBase.cpp
        Libs/muparser-2.2.5/src/muParserBytecode.cpp
        Libs/muparser-2.2.5/src/muParserCallback.cpp
        Libs/muparser-2.2.5/src/muParserError.cpp
        Libs/muparser-2.2.5/src/muParserTokenReader.cpp
        Libs/muparser-2.2.5/include/muParser.h
        Libs/muparser-2.2.5/include/muParserBase.h
        Libs/muparser-2.2.5/include/muParserBytecode.h
        Libs/muparser-2.2.5/include/muParserCallback.h
        Libs/muparser-2.2.5/include/muParserDef.h
        Libs/muparser-2.2.5/include/muParserError.h
        Libs/muparser-2.2.5/include/muParserFixes.h
        Libs/muparser-2.2.5/include/muParserStack.h
        Libs/muparser-2.2.5/include/muParserToken.h
        Libs/muparser-2.2.5/include/muParserTokenReader.h
        Animations/Filters/Filter1f.cpp Animations/Filters/Filter1f.h Animations/Paths/BasicPath.cpp Animations/Paths/BasicPath.h Animations/Filters/Filter2f.cpp Animations/Filters/Filter2f.h Animations/Filters/Filter3f.cpp Animations/Filters/Filter3f.h Animations/Paths/CompoundPath.cpp Animations/Paths/CompoundPath.h Animations/Paths/SplinePath.cpp Animations/Paths/SplinePath.h Animations/Filters/CompoundFilter.cpp Animations/Filters/CompoundFilter.h Animations/Paths/PathAnalyzer.cpp Animations/Paths/PathAnalyzer.h Animations/Paths/MixedPath.cpp Animations/Paths/MixedPath.h Animations/Paths/PathFactory.cpp Animations/Paths/PathFactory.h Animations/Filters/MixedFilter.cpp Animations/Filters/MixedFilter.h Animations/IFS.cpp Animations/IFS.h Animations/SymmetryVector.cpp Animations/SymmetryVector.h Animations/Filters/PiecewiseFilter.cpp Animations/Filters/PiecewiseFilter.h)

add_executable(Render ${SOURCE_FILES})

set(OpenCV_DIR "/home/lance/Libraries/opencv-3.0.0/build/")
find_package(OpenCV REQUIRED)
target_link_libraries(Render ${OpenCV_LIBS})