// Copyright notice: See COPYRIGHT.md for full copyright and licensing information.

#include "test_config_file.h"
#include "test_crypto.h"
#include "test_variant.h"
#include "test_packer.h"

USING_NAMESPACE_PACKER

int main() {
    TestCrypto test_crypto;
    TestVariant test_variant;
    TestConfigFile test_config_file;
    TestPacker test_packer;

    return TestSuite::run_tests(true);
}
