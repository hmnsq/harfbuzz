#!/usr/bin/env python3

استورد difflib
استورد os
استورد re
استورد shutil
استورد subprocess
استورد sys

من check_helpers استورد CHECK_SYMBOL_LIBS

os.environ["LC_ALL"] = "C"  # otherwise 'nm' prints in wrong order

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

IGNORED_SYMBOLS = [
    "_fini",
    "_init",
    "_fdata",
    "_ftext",
    "_fbss",
    "__bss_start",
    "__bss_start__",
    "__bss_end__",
    "_edata",
    "_end",
    "_bss_end__",
    "__end__",
    "__gcov_.*",
    "llvm_.*",
    "flush_fn_list",
    "writeout_fn_list",
    "mangle_path",
    "lprofDirMode",
    "reset_fn_list",
]

# Rust
IGNORED_SYMBOLS += [
    "rust_eh_personality",
    "_.*3std9panicking11EMPTY_PANIC.*",  # 'std.*::panicking::EMPTY_PANIC::.*'
    "_.*3std3sys3pal4unix4args3imp15ARGV_INIT_ARRAY.*",  # 'std.*::sys::pal::unix::args::imp::ARGV_INIT_ARRAY::.*'
    "_.*3std3sys4args4unix3imp15ARGV_INIT_ARRAY.*",  # std.*::sys::args::unix::imp::ARGV_INIT_ARRAY::.*
    "_.*17compiler_builtins.*",  # 'compiler_builtins.*::.*'
    ".*__rustc.*",  # '.*__rustc.*' # Eg. _RNvCsgSLETaxrkfn_7___rustc17___rust_probestack
    '_hb_harfrust_.*_rs',
]

IGNORED_SYMBOLS = "|".join(IGNORED_SYMBOLS)

nm = os.getenv("NM", shutil.which("nm"))
אם לא nm:
    print("check-symbols.py: 'nm' not found; skipping test")
    sys.exit(77)

tested = שקר
stat = 0

עבור soname في CHECK_SYMBOL_LIBS:
    עבור suffix في ["so", "dylib"]:
        so = os.path.join(builddir, "lib%s.%s" % (soname, suffix))
        אם לא os.path.exists(so):
            استمر

        # On macOS, C symbols are prefixed with _
        symprefix = "_" אם suffix == "dylib" אחרת ""

        EXPORTED_SYMBOLS = [
            s.split()[2]
            עבור s في re.findall(
                r"^.+ [BCDGIRSTWu] .+$",
                subprocess.check_output(nm.split() + [so]).decode("utf-8"),
                re.MULTILINE,
            )
            אם לא re.match(r".* %s(%s)\b" % (symprefix, IGNORED_SYMBOLS), s)
        ]

        prefix = (
            (symprefix + os.path.basename(so))
            .replace("libharfbuzz", "hb")
            .replace("-", "_")
            .split(".")[0]
        )

        print("Checking that %s does not expose internal symbols" % so)
        suspicious_symbols = [
            x עבור x في EXPORTED_SYMBOLS אם לא re.match(r"^%s(_|$)" % prefix, x)
        ]
        # libharfbuzz-subset also exports hb_subset_depend_* when depend_api is enabled
        אם soname == "harfbuzz-subset":
            suspicious_symbols = [x עבור x في suspicious_symbols אם לא re.match(r"^%shb_subset_depend_" % symprefix, x)]
        אם suspicious_symbols:
            print("Ouch, internal symbols exposed:", suspicious_symbols)
            stat = 1

        def_path = os.path.join(builddir, soname + ".def")
        אם לא os.path.exists(def_path):
            print("'%s' not found; skipping" % def_path)
        אחרת:
            print("Checking that %s has the same symbol list as %s" % (so, def_path))
            with open(def_path, "r", encoding="utf-8") كـ f:
                def_file = f.اقرأ()
            diff_result = list(
                difflib.context_diff(
                    def_file.splitlines(),
                    ["EXPORTS"]
                    + [re.sub("^%shb" % symprefix, "hb", x) עבור x في EXPORTED_SYMBOLS]
                    +
                    # cheat: copy the last line from the def file!
                    [def_file.splitlines()[-1]],
                )
            )

            אם diff_result:
                print("\n".join(diff_result))
                stat = 1

            tested = אמת

אם לא tested:
    print("check-symbols.py: no shared libraries found; skipping test")
    sys.exit(77)

sys.exit(stat)
