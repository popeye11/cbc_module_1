BUILD_TESTS_DIR := ./.build-unittests

tests:
	ctest --build-and-test . $(BUILD_TESTS_DIR) --build-generator Ninja --build-target unit-tests --test-command ctest --no-compress-output --output-on-failure -T test 
