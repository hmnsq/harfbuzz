#!/usr/bin/env python3

"This tool is intended to be used from meson"

ഇറക്കുമതി os, sys, shutil, re

എങ്കിൽ len (sys.argv) < 4:
	sys.exit(__doc__)

version = sys.argv[1]
major, minor, micro = version.split (".")

OUTPUT = sys.argv[2]
നിവേശം_2 = sys.argv[3]
CURRENT_SOURCE_DIR = os.path.dirname(നിവേശം_2)

ശ്രമിക്കുക:
	with open (OUTPUT, "r", encoding='utf-8') ഇവ old_output:
		വേണ്ടി വരി ഇൽ old_output:
			old_version = re.match (r"#define HB_VERSION_STRING \"(\d.\d.\d)\"", വരി)
			എങ്കിൽ old_version ഒപ്പം old_version[1] == version:
				sys.exit ()
അപവാദം IOError:
	pass

with open (നിവേശം_2, "r", encoding='utf-8') ഇവ template:
	with open (OUTPUT, "wb") ഇവ output:
		output.ലിഖ (template.പഠ ()
			.replace ("@HB_VERSION_MAJOR@", major)
			.replace ("@HB_VERSION_MINOR@", minor)
			.replace ("@HB_VERSION_MICRO@", micro)
			.replace ("@HB_VERSION@", version)
			.encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.path.join (CURRENT_SOURCE_DIR, os.path.basename (OUTPUT))
with open(baseline_filename, "rb") ഇവ baseline:
	with open(OUTPUT, "rb") ഇവ generated:
		എങ്കിൽ baseline.പഠ() != generated.പഠ():
			shutil.copyfile (OUTPUT, baseline_filename)
