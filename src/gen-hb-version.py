#!/usr/bin/env python3

"This tool is intended to be used from meson"

استورد os, sys, shutil, re

إذا طول (sys.argv) < 4:
	sys.exit(__doc__)

نسخه = sys.argv[1]
major, minor, micro = نسخه.تقسیم (".")

OUTPUT = sys.argv[2]
درون_داد_2 = sys.argv[3]
CURRENT_منبع_DIR = os.مسیر.dirname(درون_داد_2)

حاول:
	با open (OUTPUT, "r", encoding='utf-8') كـ old_output:
		لكل سطر في old_output:
			old_نسخه = re.تطابق (r"#define HB_VERSION_STRING \"(\d.\d.\d)\"", سطر)
			إذا old_نسخه و old_نسخه[1] == نسخه:
				sys.exit ()
استثناء IOError:
	بگذر

با open (درون_داد_2, "r", encoding='utf-8') كـ template:
	با open (OUTPUT, "wb") كـ output:
		output.اكتب (template.اقرأ ()
			.replace ("@HB_VERSION_MAJOR@", major)
			.replace ("@HB_VERSION_MINOR@", minor)
			.replace ("@HB_VERSION_MICRO@", micro)
			.replace ("@HB_VERSION@", نسخه)
			.encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.مسیر.join (CURRENT_منبع_DIR, os.مسیر.basename (OUTPUT))
با open(baseline_filename, "rb") كـ baseline:
	با open(OUTPUT, "rb") كـ generated:
		إذا baseline.اقرأ() != generated.اقرأ():
			shutil.copyfile (OUTPUT, baseline_filename)
