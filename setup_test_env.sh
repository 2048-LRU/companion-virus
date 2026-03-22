BUILD_DIR="build/release"
TEST_ENV="${BUILD_DIR}/home/user1"
IMAGE_FOLDER="${TEST_ENV}/images"

mkdir -p "$TEST_ENV"
mkdir -p "$IMAGE_FOLDER"
cp assets/* "$IMAGE_FOLDER" 2>/dev/null

cp "${BUILD_DIR}/src/virus/mediaplayer" "$TEST_ENV/"
cp "${BUILD_DIR}/src/utils/base_converter" "$TEST_ENV/"
cp "${BUILD_DIR}/src/utils/caesar_encrypt" "$TEST_ENV/"
cp "${BUILD_DIR}/src/utils/file_hasher" "$TEST_ENV/"
cp "${BUILD_DIR}/src/utils/img_inverter" "$TEST_ENV/"
cp "${BUILD_DIR}/src/utils/num_generator" "$TEST_ENV/"
cp "${BUILD_DIR}/src/utils/temp_converter" "$TEST_ENV/"

chmod 770 "$TEST_ENV"/*