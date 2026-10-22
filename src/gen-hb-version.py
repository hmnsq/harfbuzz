#!/usr/bin/env python3

"This tool is intended to be used from meson"

आयात os, sys, shutil, re

जर len (sys.argv) < 4:
	sys.exit(__doc__)

version = sys.argv[1]
major, minor, micro = version.split (".")

OUTPUT = sys.argv[2]
आदान_2 = sys.argv[3]
CURRENT_SOURCE_DIR = os.path.dirname(आदान_2)

प्रयत्न:
	with open (OUTPUT, "r", encoding='utf-8') इव old_output:
		साठी ओळ मध्ये old_output:
			old_version = re.match (r"#define HB_VERSION_STRING \"(\d.\d.\d)\"", ओळ)
			जर old_version आणि old_version[1] == version:
				sys.exit ()
अपवाद IOError:
	pass

with open (आदान_2, "r", encoding='utf-8') इव template:
	with open (OUTPUT, "wb") इव output:
		output.लिख (template.पठ ()
			.replace ("@HB_VERSION_MAJOR@", major)
			.replace ("@HB_VERSION_MINOR@", minor)
			.replace ("@HB_VERSION_MICRO@", micro)
			.replace ("@HB_VERSION@", version)
			.encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.path.join (CURRENT_SOURCE_DIR, os.path.basename (OUTPUT))
with open(baseline_filename, "rb") इव baseline:
	with open(OUTPUT, "rb") इव generated:
		जर baseline.पठ() != generated.पठ():
			shutil.copyfile (OUTPUT, baseline_filename)
