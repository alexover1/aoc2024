/* ========================================================================
   $File: nob.c $
   $Date: 12/8/24 $
   $Revision: $
   $Creator: Alex Overstreet $
   $Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
   ======================================================================== */

#include <time.h>

#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#include "day.h"

bool build_aoc(Cmd *cmd)
{
    bool result = true;

    if (!mkdir_if_not_exists(".build")) return_defer(false);

    Procs procs = {0};
    String_Builder sb = {0};
    File_Paths file_paths = {0};

    for (size_t i = 0; i < DAY; i += 1) {
        da_append(&file_paths, temp_sprintf(".build/day%02zu.o", i + 1));
    }

    const char *header_paths[] = { "aoc.h", "utils.h" };

    for (size_t i = 0; i < DAY; i += 1) {
        const char *input_path = temp_sprintf("code/day%02zu.cpp", i + 1);
        const char *output_path = file_paths.items[i];

        const char *input_paths[] = { "aoc.h", "utils.h", input_path };
        if (needs_rebuild(output_path, input_paths, ARRAY_LEN(input_paths))) {
            cmd_append(cmd, "clang++", "-std=c++11", "-I.");
            cmd_append(cmd, "-c", input_path);
            cmd_append(cmd, "-o", output_path);

            Proc proc = cmd_run_async_and_reset(cmd);
            da_append(&procs, proc);
        }
    }

    if (!procs_wait_and_reset(&procs)) return_defer(false);

    sb_append_cstr(&sb, "#pragma once\n\n");
    for (size_t i = 0; i < DAY; i += 1) {
        sb_append_cstr(&sb, temp_sprintf("extern solution Solution%02zu;\n", i + 1));
    }
    sb_append_cstr(&sb, "\ninternal solution Solutions[] =\n{\n");
    for (size_t i = 0; i < DAY; i += 1) {
        sb_append_cstr(&sb, temp_sprintf("    Solution%02zu,\n", i + 1));
    }
    sb_append_cstr(&sb, "};\n");

    if (!write_entire_file("solutions.h", sb.items, sb.count)) return_defer(false);

    size_t object_files_count = file_paths.count;
    da_append(&file_paths, "aoc.cpp");
    da_append_many(&file_paths, header_paths, ARRAY_LEN(header_paths));

    const char *binary_path = "./aoc";

    if (needs_rebuild(binary_path, file_paths.items, file_paths.count)) {
        cmd_append(cmd, "clang++", "-std=c++11");
        cmd_append(cmd, "aoc.cpp", "-o", binary_path);
        da_append_many(cmd, file_paths.items, object_files_count);

        if (!cmd_run_sync_and_reset(cmd)) return_defer(false);
    }

defer:
    sb_free(sb);
    da_free(procs);
    da_free(file_paths);

    return result;
}

bool save_next_day_header(const char *file_path, size_t day)
{
    bool result = true;
    String_Builder sb = {0};

    sb_append_cstr(&sb, temp_sprintf("#define DAY %zu\n", day));
    if (!write_entire_file(file_path, sb.items, sb.count)) return_defer(false);

    nob_log(NOB_INFO, "Updated %s", file_path);

defer:
    sb_free(sb);
    return result;
}

bool save_next_day_template(size_t day)
{
    bool result = true;
    String_Builder sb = {0};

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char date_string[64];
    assert(strftime(date_string, sizeof(date_string), "%D", tm));

    sb_append_cstr(&sb, "/* ========================================================================\n");
    sb_append_cstr(&sb, temp_sprintf("$File: day%02zu.cpp $\n", day));
    sb_append_cstr(&sb, temp_sprintf("$Date: %s $\n", date_string));
    sb_append_cstr(&sb, "$Revision: $\n");
    sb_append_cstr(&sb, "$Creator: Alex Overstreet $\n");
    sb_append_cstr(&sb, "$Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $\n");
    sb_append_cstr(&sb, "======================================================================== */\n");

    sb_append_cstr(&sb, "\n#include \"aoc.h\"\n");

    sb_append_cstr(&sb, "\ninternal u64\n");
    sb_append_cstr(&sb, "SolvePartOne(memory_arena *Arena, string Input)\n");
    sb_append_cstr(&sb, "{\n");
    sb_append_cstr(&sb, "    u64 Result = 0;\n");
    sb_append_cstr(&sb, "    return(Result);\n");
    sb_append_cstr(&sb, "}\n");
    sb_append_cstr(&sb, "\ninternal u64\n");
    sb_append_cstr(&sb, "SolvePartTwo(memory_arena *Arena, string Input)\n");
    sb_append_cstr(&sb, "{\n");
    sb_append_cstr(&sb, "    u64 Result = 0;\n");
    sb_append_cstr(&sb, "    return(Result);\n");
    sb_append_cstr(&sb, "}\n");

    sb_append_cstr(&sb, temp_sprintf("\nsolution Solution%02zu =\n", day));
    sb_append_cstr(&sb, "{\n    SolvePartOne,\n    SolvePartTwo,\n};\n");

    const char *file_path = temp_sprintf("code/day%02zu.cpp", day);

    if (!write_entire_file(file_path, sb.items, sb.count)) return_defer(false);

    nob_log(NOB_INFO, "Created %s", file_path);

defer:
    sb_free(sb);
    return result;
}

bool save_test_template(size_t day)
{
    bool result = true;
    String_Builder sb = {0};

    sb_append_cstr(&sb, ":i part_one 0\n");
    sb_append_cstr(&sb, ":i part_two 0\n");
    sb_append_cstr(&sb, ":b input 0\n");

    const char *file_path = temp_sprintf("tests/%02zu_sample.txt", day);

    if (!write_entire_file(file_path, sb.items, sb.count)) return_defer(false);

    nob_log(NOB_INFO, "Created %s", file_path);

defer:
    sb_free(sb);
    return result;
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    bool result = 0;
    const char *program = shift_args(&argc, &argv);
    Cmd cmd = {0};

    if (argc > 0 && strcmp(*argv, "next_day") == 0) {
        size_t day = DAY + 1;

        if (!save_next_day_header("day.h", day)) return_defer(1);
        if (!save_next_day_template(day)) return_defer(1);
        if (!save_test_template(day)) return_defer(1);
    } else {
        if (!build_aoc(&cmd)) return 1;

        if (argc > 0) {
            cmd_append(&cmd, "./aoc");
            da_append_many(&cmd, argv, argc);

            if (!cmd_run_sync_and_reset(&cmd)) return_defer(1);
        }
    }

defer:
    cmd_free(cmd);
    return 0;
}
