include(FetchContent)
FetchContent_Declare(
        assimp
        GIT_REPOSITORY	https://github.com/assimp/assimp.git
        GIT_TAG 	    master
)
FetchContent_MakeAvailable(assimp)
