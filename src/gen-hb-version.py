#!/usr/bin/env python3

"This tool is intended to be used from meson"

আনযন os, sys, shutil, re

যদি len (sys.argv) < 4:
	sys.exit(__doc__)

version = sys.argv[1]
major, minor, micro = version.split (".")

OUTPUT = sys.argv[2]
প্রবেশ_2 = sys.argv[3]
CURRENT_SOURCE_DIR = os.path.dirname(প্রবেশ_2)

যত্ন:
	with open (OUTPUT, "r", encoding='utf-8') ইব old_output:
		প্রতি সারি অন্তঃ old_output:
			old_version = re.match (r"#define HB_VERSION_STRING \"(\d.\d.\d)\"", সারি)
			যদি old_version চ old_version[1] == version:
				sys.exit ()
অপবাদ IOError:
	pass

with open (প্রবেশ_2, "r", encoding='utf-8') ইব template:
	with open (OUTPUT, "wb") ইব output:
		output.লিখ (template.পঠ ()
			.replace ("@HB_VERSION_MAJOR@", major)
			.replace ("@HB_VERSION_MINOR@", minor)
			.replace ("@HB_VERSION_MICRO@", micro)
			.replace ("@HB_VERSION@", version)
			.encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.path.join (CURRENT_SOURCE_DIR, os.path.basename (OUTPUT))
with open(baseline_filename, "rb") ইব baseline:
	with open(OUTPUT, "rb") ইব generated:
		যদি baseline.পঠ() != generated.পঠ():
			shutil.copyfile (OUTPUT, baseline_filename)
