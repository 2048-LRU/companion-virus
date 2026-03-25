BUILD_DIR="build/release"
TEST_ENV="home/user1"
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

# rwx pour pouvoir renommer les fichiers
chmod 775 "$TEST_ENV"

# droits de l'énoncé sur les fichiers, l'user exécute / lit uniquement, le groupe lit, écrit exécute
chmod 570 "$TEST_ENV"/*

# on doit pouvoir lire les images, en ajouter dans le dossier ...
chmod 775 "$IMAGE_FOLDER"
chmod 664 "$IMAGE_FOLDER"/*