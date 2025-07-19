include(FetchContent)

FetchContent_Declare(
  tinygltf
  GIT_REPOSITORY https://github.com/syoyo/tinygltf.git
  GIT_TAG v2.9.6
)

FetchContent_MakeAvailable(tinygltf)

