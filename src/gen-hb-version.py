#!/usr/bin/env python3

"This tool is intended to be used from meson"

आयात os, sys, shutil, re

अगर len (sys.argv) < 4:
	sys.exit(__doc__)

version = sys.argv[1]
major, minor, micro = version.split (".")

OUTPUT = sys.argv[2]
निवेश_2 = sys.argv[3]
CURRENT_SOURCE_DIR = os.path.dirname(निवेश_2)

प्रयास:
	with open (OUTPUT, "r", encoding='utf-8') as old_output:
		क्रम पंक्ति in old_output:
			old_version = re.match (r"#define HB_VERSION_STRING \"(\d.\d.\d)\"", पंक्ति)
			अगर old_version and old_version[1] == version:
				sys.exit ()
except IOError:
	pass

with open (निवेश_2, "r", encoding='utf-8') as template:
	with open (OUTPUT, "wb") as output:
		output.म_लिखो (template.म_पढ़ो ()
			.प्रतिस्थापन ("@HB_VERSION_MAJOR@", major)
			.प्रतिस्थापन ("@HB_VERSION_MINOR@", minor)
			.प्रतिस्थापन ("@HB_VERSION_MICRO@", micro)
			.प्रतिस्थापन ("@HB_VERSION@", version)
			.encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.path.मेल (CURRENT_SOURCE_DIR, os.path.basename (OUTPUT))
with open(baseline_filename, "rb") as baseline:
	with open(OUTPUT, "rb") as generated:
		अगर baseline.म_पढ़ो() != generated.म_पढ़ो():
			shutil.copyfile (OUTPUT, baseline_filename)
