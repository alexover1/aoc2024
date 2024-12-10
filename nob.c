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

#define _(x) do { if(!(x)) return_defer(false); } while(0)
bool build_aoc(Cmd *cmd)
{
    bool result = true;

    _(mkdir_if_not_exists(".build"));

    Procs procs = {0};
    String_Builder sb = {0};
    File_Paths file_paths = {0};

    for (size_t i = 0; i < DAY; i += 1) {
        da_append(&file_paths, temp_sprintf(".build/day%02zu.o", i + 1));
    }

    const char *header_paths[] = { "aoc.h", "aoc_lib.h" };

    for (size_t i = 0; i < DAY; i += 1) {
        const char *input_path = temp_sprintf("code/day%02zu.cpp", i + 1);
        const char *output_path = file_paths.items[i];

        const char *input_paths[] = { "aoc.h", "aoc_lib.h", input_path };
        if (needs_rebuild(output_path, input_paths, ARRAY_LEN(input_paths))) {
            cmd_append(cmd, "clang++", "-std=c++11", "-I.");
            cmd_append(cmd, "-c", input_path);
            cmd_append(cmd, "-o", output_path);

            Proc proc = cmd_run_async_and_reset(cmd);
            da_append(&procs, proc);
        }
    }

    _(procs_wait_and_reset(&procs));

    sb_append_cstr(&sb, "#pragma once\n\n");
    for (size_t i = 0; i < DAY; i += 1) {
        sb_append_cstr(&sb, temp_sprintf("extern solution Solution%02zu;\n", i + 1));
    }
    sb_append_cstr(&sb, "\ninternal solution Solutions[] =\n{\n");
    for (size_t i = 0; i < DAY; i += 1) {
        sb_append_cstr(&sb, temp_sprintf("    Solution%02zu,\n", i + 1));
    }
    sb_append_cstr(&sb, "};\n");

    _(write_entire_file("solutions.h", sb.items, sb.count));

    size_t object_files_count = file_paths.count;
    da_append(&file_paths, "aoc.cpp");
    da_append_many(&file_paths, header_paths, ARRAY_LEN(header_paths));

    const char *binary_path = "./aoc";

    if (needs_rebuild(binary_path, file_paths.items, file_paths.count)) {
        cmd_append(cmd, "clang++", "-std=c++11");
        cmd_append(cmd, "aoc.cpp", "-o", binary_path);
        da_append_many(cmd, file_paths.items, object_files_count);

        _(cmd_run_sync_and_reset(cmd));
    }

defer:
    sb_free(sb);
    da_free(procs);
    da_free(file_paths);

    return result;
}
#undef _

#define _(x) do { if(!(x)) return 1; } while(0)
int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    const char *program = shift_args(&argc, &argv);

    Cmd cmd = {0};
    String_Builder sb = {0};

    if (argc > 0) {
        const char *subcommand = shift_args(&argc, &argv);

        if (strcmp(subcommand, "run") == 0) {
            _(build_aoc(&cmd));

            cmd_append(&cmd, "./aoc");
            da_append_many(&cmd, argv, argc);

            _(cmd_run_sync_and_reset(&cmd));
        } else if (strcmp(subcommand, "next_day") == 0) {
            size_t next_day = DAY + 1;

            sb_append_cstr(&sb, temp_sprintf("#define DAY %zu\n", next_day));
            _(write_entire_file("day.h", sb.items, sb.count));

            time_t t = time(NULL);
            struct tm *tm = localtime(&t);
            char date_string[64];
            assert(strftime(date_string, sizeof(date_string), "%D", tm));

            sb.count = 0;

            sb_append_cstr(&sb, "/* ========================================================================\n");
            sb_append_cstr(&sb, temp_sprintf("$File: day%02zu.cpp $\n", next_day));
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

            sb_append_cstr(&sb, temp_sprintf("\nsolution Solution%02zu =\n", next_day));
            sb_append_cstr(&sb, "{\n    SolvePartOne,\n    SolvePartTwo,\n};\n");

            _(write_entire_file(temp_sprintf("code/day%02zu.cpp", next_day), sb.items, sb.count));
        } else {
            _(build_aoc(&cmd));
        }
    }

    return 0;
}
#undef _
