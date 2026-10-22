#!/usr/bin/env python3

"This tool is intended to be used from meson"

ಆನಯನ os, sys, shutil, re

ಯದಿ len (sys.argv) < 4:
	sys.exit(__doc__)

version = sys.argv[1]
major, minor, micro = version.split (".")

OUTPUT = sys.argv[2]
ಒಳಹರಿವು_2 = sys.argv[3]
CURRENT_SOURCE_DIR = os.path.dirname(ಒಳಹರಿವು_2)

ಯತ್ನ:
	with open (OUTPUT, "r", encoding='utf-8') ಇವ old_output:
		ಪ್ರತಿ ಸಾಲು ಅನ್ತಃ old_output:
			old_version = re.match (r"#define HB_VERSION_STRING \"(\d.\d.\d)\"", ಸಾಲು)
			ಯದಿ old_version ಚ old_version[1] == version:
				sys.exit ()
ಅಪವಾದ IOError:
	pass

with open (ಒಳಹರಿವು_2, "r", encoding='utf-8') ಇವ template:
	with open (OUTPUT, "wb") ಇವ output:
		output.ಲಿಖ (template.ಪಠ ()
			.replace ("@HB_VERSION_MAJOR@", major)
			.replace ("@HB_VERSION_MINOR@", minor)
			.replace ("@HB_VERSION_MICRO@", micro)
			.replace ("@HB_VERSION@", version)
			.encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.path.join (CURRENT_SOURCE_DIR, os.path.basename (OUTPUT))
with open(baseline_filename, "rb") ಇವ baseline:
	with open(OUTPUT, "rb") ಇವ generated:
		ಯದಿ baseline.ಪಠ() != generated.ಪಠ():
			shutil.copyfile (OUTPUT, baseline_filename)
