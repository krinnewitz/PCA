cmake_minimum_required (VERSION 2.6)
project (PCA) 

FIND_PACKAGE( OpenCV REQUIRED )

add_executable (pca Main.cpp PCA.cpp Texture.cpp) 

TARGET_LINK_LIBRARIES( pca ${OpenCV_LIBS} )

