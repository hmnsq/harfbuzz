#!/usr/bin/env python3

"This tool is intended to be used from meson"

இறக்குமதி os, sys, shutil, re

எனில் len (sys.argv) < 4:
	sys.exit(__doc__)

version = sys.argv[1]
major, minor, micro = version.split (".")

OUTPUT = sys.argv[2]
உள்ளீடு_2 = sys.argv[3]
CURRENT_SOURCE_DIR = os.path.dirname(உள்ளீடு_2)

முயல்:
	with open (OUTPUT, "r", encoding='utf-8') இவ old_output:
		க்கு வரி உள் old_output:
			old_version = re.match (r"#define HB_VERSION_STRING \"(\d.\d.\d)\"", வரி)
			எனில் old_version மற்றும் old_version[1] == version:
				sys.exit ()
விதிவிலக்கு IOError:
	pass

with open (உள்ளீடு_2, "r", encoding='utf-8') இவ template:
	with open (OUTPUT, "wb") இவ output:
		output.லிக (template.பட ()
			.replace ("@HB_VERSION_MAJOR@", major)
			.replace ("@HB_VERSION_MINOR@", minor)
			.replace ("@HB_VERSION_MICRO@", micro)
			.replace ("@HB_VERSION@", version)
			.encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.path.join (CURRENT_SOURCE_DIR, os.path.basename (OUTPUT))
with open(baseline_filename, "rb") இவ baseline:
	with open(OUTPUT, "rb") இவ generated:
		எனில் baseline.பட() != generated.பட():
			shutil.copyfile (OUTPUT, baseline_filename)
