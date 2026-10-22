#!/usr/bin/env python3

استورد difflib
استورد os
استورد re
استورد shutil
استورد subprocess
استورد sys

من check_helpers استورد CHECK_علامت_LIBS

os.environ["LC_ALL"] = "C"  # otherwise 'nm' prints in wrong order

srcdir = sys.argv[1]
ادھار_srcdir = sys.argv[2]
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
إذا ليس nm:
    چھاپو("check-symbols.py: 'nm' not found; skipping test")
    sys.exit(77)

tested = خطأ
stat = 0

لكل soname في CHECK_علامت_LIBS:
    لكل suffix في ["so", "dylib"]:
        so = os.رستہ.join(builddir, "lib%s.%s" % (soname, suffix))
        إذا ليس os.رستہ.exists(so):
            استمر

        # On macOS, C symbols are prefixed with _
        symprefix = "_" إذا suffix == "dylib" وإلا ""

        EXPORTED_SYMBOLS = [
            s.ونڈو()[2]
            لكل s في re.findall(
                r"^.+ [BCDGIRSTWu] .+$",
                subprocess.check_output(nm.ونڈو() + [so]).decode("utf-8"),
                re.MULTILINE,
            )
            إذا ليس re.میل(r".* %s(%s)\b" % (symprefix, IGNORED_SYMBOLS), s)
        ]

        اگیتر = (
            (symprefix + os.رستہ.basename(so))
            .replace("libharfbuzz", "hb")
            .replace("-", "_")
            .ونڈو(".")[0]
        )

        چھاپو("Checking that %s does not expose internal symbols" % so)
        suspicious_symbols = [
            x لكل x في EXPORTED_SYMBOLS إذا ليس re.میل(r"^%s(_|$)" % اگیتر, x)
        ]
        # libharfbuzz-subset also exports hb_subset_depend_* when depend_api is enabled
        إذا soname == "harfbuzz-subset":
            suspicious_symbols = [x لكل x في suspicious_symbols إذا ليس re.میل(r"^%shb_subset_depend_" % symprefix, x)]
        إذا suspicious_symbols:
            چھاپو("Ouch, internal symbols exposed:", suspicious_symbols)
            stat = 1

        def_رستہ = os.رستہ.join(builddir, soname + ".def")
        إذا ليس os.رستہ.exists(def_رستہ):
            چھاپو("'%s' not found; skipping" % def_رستہ)
        وإلا:
            چھاپو("Checking that %s has the same symbol list as %s" % (so, def_رستہ))
            نال open(def_رستہ, "r", encoding="utf-8") كـ f:
                def_file = f.اقرأ()
            diff_نتیجہ = list(
                difflib.context_diff(
                    def_file.splitlines(),
                    ["EXPORTS"]
                    + [re.sub("^%shb" % symprefix, "hb", x) لكل x في EXPORTED_SYMBOLS]
                    +
                    # cheat: copy the last line from the def file!
                    [def_file.splitlines()[-1]],
                )
            )

            إذا diff_نتیجہ:
                چھاپو("\n".join(diff_نتیجہ))
                stat = 1

            tested = سچ

إذا ليس tested:
    چھاپو("check-symbols.py: no shared libraries found; skipping test")
    sys.exit(77)

sys.exit(stat)
