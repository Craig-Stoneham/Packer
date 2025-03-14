// See LICENSE for full copyright and licensing information.

#include "test_packer.h"

PACKER_NAMESPACE_BEGIN

bool TestPacker::test_packer() {
    FileAccess::remove_all(read_path);
    FileAccess::remove_all(write_path);

    for (const auto& file_name : files) {
        FileStreamO stream;

        FileAccess::create_directories(read_path);

        stream.open(read_path + "/" + file_name, std::ios::binary);
        stream << "Hello World!";
        stream.close();
    }

    if (packer.get_ignore_file_enabled()) {
        FileStreamO stream;
        stream.open(read_path + "/" + packer.get_ignore_file_name(), std::ios::binary);
        stream.close();
    }

    packer.pack_files();

    bool write_should_exist = true;

    if (packer.get_pack_mode() == Packer::PackMode::Exclude) {
        write_should_exist = false;
    }

    if (packer.get_ignore_file_enabled() == true) {
        write_should_exist = false;
	}

    for (const auto& file_name : files) {
		String file_path = write_path + "/" + file_name;
        if (packer.get_suffix_enabled()) {
            remove_path_suffix(file_path, packer.get_suffix_string());
        }
        if (packer.get_extension_adjust() == Packer::ExtensionAdjust::Lower) {
            size_t ext_pos = file_path.find_last_of('.') + 1;
            std::transform(file_path.begin() + ext_pos, file_path.end(), file_path.begin() + ext_pos, tolower);
        } else if (packer.get_extension_adjust() == Packer::ExtensionAdjust::Upper) {
            size_t ext_pos = file_path.find_last_of('.') + 1;
            std::transform(file_path.begin() + ext_pos, file_path.end(), file_path.begin() + ext_pos, toupper);
        }
        if (FileAccess::exists(file_path) != write_should_exist) {
            return false;
        }
    }

    if (packer.get_move_files()) {
		for (const auto& file_name : files) {
			if (FileAccess::exists(read_path + "/" + file_name)) {
				return false;
			}
		}
    }

    return true;
}

TestResult TestPacker::test() {
#ifdef EXPERIMENTAL_FILESYSTEM
    normalize_path_separators(read_path);
    normalize_path_separators(write_path);
#endif // EXPERIMENTAL_FILESYSTEM

    packer.set_read_path(read_path);
    packer.set_write_path(write_path);
    packer.set_pack_mode(Packer::PackMode::Everything);
    packer.set_overwrite_files(false);
    packer.set_move_files(false);
    packer.set_suffix_string("(1)");
    packer.set_suffix_enabled(false);
    packer.set_extension_insensitive(false);
    packer.set_extension_adjust(Packer::ExtensionAdjust::Default);
    packer.set_ignore_file_name(".pkignore");
    packer.set_ignore_file_enabled(false);
#ifdef LOG_ENABLED
    packer.set_log_enabled(false);
#endif // LOG_ENABLED

    if (test_packer() == false) {
        return TEST_FAILED("Everything pack mode not working correctly.");
    }
    packer.add_extension("txt");
    packer.add_extension("TXT");

    packer.set_pack_mode(Packer::PackMode::Exclude);
    if (test_packer() == false) {
        return TEST_FAILED("Exclude pack mode not working correctly.");
    }

    packer.set_pack_mode(Packer::PackMode::Include);
    if (test_packer() == false) {
        return TEST_FAILED("Include pack mode not working correctly.");
    }

#ifdef IGNORE_FILE_ENABLED
   packer.set_ignore_file_enabled(true);
    if (test_packer() == false) {
        return TEST_FAILED("Ignore file not working correctly.");
    }
    packer.set_ignore_file_enabled(false);
#endif //PACKER_IGNORE_FILE
    
    packer.set_move_files(true);
    if (test_packer() == false) {
        return TEST_FAILED("Move files not working correctly.");
    }
    packer.set_move_files(false);

    packer.set_suffix_enabled(true);
    if (test_packer() == false) {
        return TEST_FAILED("Suffix not working correctly.");
    }
    packer.set_suffix_enabled(false);

    packer.remove_extension("TXT");
    packer.set_extension_insensitive(true);
    if (test_packer() == false) {
        return TEST_FAILED("Extension insensitive not working correctly.");
    }
    packer.set_extension_insensitive(false);
    packer.add_extension("TXT");

    packer.set_extension_adjust(Packer::ExtensionAdjust::Lower);
    if (test_packer() == false) {
        return TEST_FAILED("Extension adjust to upper not working correctly.");
    }

    packer.set_extension_adjust(Packer::ExtensionAdjust::Upper);
    if (test_packer() == false) {
        return TEST_FAILED("Extension adjust to lower not working correctly.");
    }

    FileAccess::remove_all(read_path);
    FileAccess::remove_all(write_path);

    return TEST_PASSED();
}

TestPacker::TestPacker() :
    read_path(FileAccess::current_path().string() + "/" + "Read"),
    write_path(FileAccess::current_path().string() + "/" + "Write"),
    files({ "lower_case(1).txt", "UPPER_CASE(1).TXT" }) {
    ADD_TEST("Packer", [this]() { return test(); });
}

TestPacker::~TestPacker() {
    FileAccess::remove_all(read_path);
    FileAccess::remove_all(write_path);
}

PACKER_NAMESPACE_END
