#!/usr/bin/env python3

"This tool is intended to be used from meson"

استورد os, sys, shutil, re

إذا len (sys.argv) < 4:
	sys.exit(__doc__)

version = sys.argv[1]
major, minor, micro = version.split (".")

OUTPUT = sys.argv[2]
إدخال_2 = sys.argv[3]
CURRENT_SOURCE_DIR = os.path.dirname(إدخال_2)

حاول:
	with open (OUTPUT, "r", encoding='utf-8') كـ old_output:
		لكل سطر في old_output:
			old_version = re.match (r"#define HB_VERSION_STRING \"(\d.\d.\d)\"", سطر)
			إذا old_version و old_version[1] == version:
				sys.exit ()
استثناء IOError:
	pass

with open (إدخال_2, "r", encoding='utf-8') كـ template:
	with open (OUTPUT, "wb") كـ output:
		output.اكتب (template.اقرأ ()
			.replace ("@HB_VERSION_MAJOR@", major)
			.replace ("@HB_VERSION_MINOR@", minor)
			.replace ("@HB_VERSION_MICRO@", micro)
			.replace ("@HB_VERSION@", version)
			.encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.path.join (CURRENT_SOURCE_DIR, os.path.basename (OUTPUT))
with open(baseline_filename, "rb") كـ baseline:
	with open(OUTPUT, "rb") كـ generated:
		إذا baseline.اقرأ() != generated.اقرأ():
			shutil.copyfile (OUTPUT, baseline_filename)
