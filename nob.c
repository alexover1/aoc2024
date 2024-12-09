#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#define _(x) do { if(!(x)) return(1); } while(0)

#define DAYS_COUNT 6

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    _(mkdir_if_not_exists(".build"));

    Cmd cmd = {0};
    Procs procs = {0};
    String_Builder sb = {0};
    File_Paths file_paths = {0};

    for (size_t i = 0; i < DAYS_COUNT; i += 1) {
        da_append(&file_paths, temp_sprintf(".build/day%02zu.o", i + 1));
    }

    const char *header_paths[] = { "aoc.h", "aoc_lib.h" };

    for (size_t i = 0; i < DAYS_COUNT; i += 1) {
        const char *input_path = temp_sprintf("code/day%02zu.cpp", i + 1);
        const char *output_path = file_paths.items[i];

        const char *input_paths[] = { "aoc.h", "aoc_lib.h", input_path };
        if (needs_rebuild(output_path, input_paths, ARRAY_LEN(input_paths))) {
            cmd_append(&cmd, "clang++", "-std=c++11", "-I.");
            cmd_append(&cmd, "-c", input_path);
            cmd_append(&cmd, "-o", output_path);

            Proc proc = cmd_run_async_and_reset(&cmd);
            da_append(&procs, proc);
        }
    }

    _(procs_wait_and_reset(&procs));

    sb_append_cstr(&sb, "#pragma once\n\n");
    for (size_t i = 0; i < DAYS_COUNT; i += 1) {
        sb_append_cstr(&sb, temp_sprintf("extern solution Solution%02zu;\n", i + 1));
    }
    sb_append_cstr(&sb, "\ninternal solution Solutions[] =\n{\n");
    for (size_t i = 0; i < DAYS_COUNT; i += 1) {
        sb_append_cstr(&sb, temp_sprintf("    Solution%02zu,\n", i + 1));
    }
    sb_append_cstr(&sb, "};\n");

    _(write_entire_file("solutions.h", sb.items, sb.count));

    size_t object_files_count = file_paths.count;
    da_append(&file_paths, "aoc.cpp");
    da_append_many(&file_paths, header_paths, ARRAY_LEN(header_paths));

    if (needs_rebuild("./aoc", file_paths.items, file_paths.count)) {
        cmd_append(&cmd, "clang++", "-std=c++11");
        cmd_append(&cmd, "aoc.cpp", "-o", "./aoc");
        da_append_many(&cmd, file_paths.items, object_files_count);

        _(cmd_run_sync_and_reset(&cmd));
    }

    return 0;
}
