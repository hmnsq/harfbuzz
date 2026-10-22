#!/usr/bin/env python3

"This tool is intended to be used from meson"

استورد os, sys, shutil, re

ܐܢ len (sys.argv) < 4:
	sys.exit(__doc__)

version = sys.argv[1]
major, minor, micro = version.split (".")

OUTPUT = sys.argv[2]
INPUT = sys.argv[3]
CURRENT_SOURCE_DIR = os.path.dirname(INPUT)

حاول:
	with open (OUTPUT, "r", encoding='utf-8') كـ old_output:
		لكل line في old_output:
			old_version = re.match (r"#define HB_VERSION_STRING \"(\d.\d.\d)\"", line)
			ܐܢ old_version ܘ old_version[1] == version:
				sys.exit ()
استثناء IOError:
	pass

with open (INPUT, "r", encoding='utf-8') كـ template:
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
		ܐܢ baseline.اقرأ() != generated.اقرأ():
			shutil.copyfile (OUTPUT, baseline_filename)
