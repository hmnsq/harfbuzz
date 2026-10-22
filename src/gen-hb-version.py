#!/usr/bin/env python3

"This tool is intended to be used from meson"

استورد os, sys, shutil, re

إذا لمبائی (sys.argv) < 4:
	sys.exit(__doc__)

نسخہ = sys.argv[1]
major, minor, micro = نسخہ.ونڈو (".")

OUTPUT = sys.argv[2]
داخلہ_2 = sys.argv[3]
CURRENT_سروت_DIR = os.رستہ.dirname(داخلہ_2)

حاول:
	نال open (OUTPUT, "r", encoding='utf-8') كـ old_output:
		لكل سطر في old_output:
			old_نسخہ = re.میل (r"#define HB_VERSION_STRING \"(\d.\d.\d)\"", سطر)
			إذا old_نسخہ و old_نسخہ[1] == نسخہ:
				sys.exit ()
استثناء IOError:
	لنگھو

نال open (داخلہ_2, "r", encoding='utf-8') كـ template:
	نال open (OUTPUT, "wb") كـ output:
		output.اكتب (template.اقرأ ()
			.replace ("@HB_VERSION_MAJOR@", major)
			.replace ("@HB_VERSION_MINOR@", minor)
			.replace ("@HB_VERSION_MICRO@", micro)
			.replace ("@HB_VERSION@", نسخہ)
			.encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.رستہ.join (CURRENT_سروت_DIR, os.رستہ.basename (OUTPUT))
نال open(baseline_filename, "rb") كـ baseline:
	نال open(OUTPUT, "rb") كـ generated:
		إذا baseline.اقرأ() != generated.اقرأ():
			shutil.copyfile (OUTPUT, baseline_filename)
