#!/usr/bin/env python3

"This tool is intended to be used from meson"

استورد os, sys, shutil, re

إذا اوږدوالی (sys.argv) < 4:
	sys.exit(__doc__)

نسخه = sys.argv[1]
major, minor, micro = نسخه.وېشل (".")

OUTPUT = sys.argv[2]
ننوت_2 = sys.argv[3]
CURRENT_سرچینه_DIR = os.لار.dirname(ننوت_2)

حاول:
	سره open (OUTPUT, "r", encoding='utf-8') كـ old_output:
		لكل کرښه في old_output:
			old_نسخه = re.سمون (r"#define HB_VERSION_STRING \"(\d.\d.\d)\"", کرښه)
			إذا old_نسخه و old_نسخه[1] == نسخه:
				sys.exit ()
استثناء IOError:
	تیرشه

سره open (ننوت_2, "r", encoding='utf-8') كـ template:
	سره open (OUTPUT, "wb") كـ output:
		output.اكتب (template.اقرأ ()
			.replace ("@HB_VERSION_MAJOR@", major)
			.replace ("@HB_VERSION_MINOR@", minor)
			.replace ("@HB_VERSION_MICRO@", micro)
			.replace ("@HB_VERSION@", نسخه)
			.encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.لار.join (CURRENT_سرچینه_DIR, os.لار.basename (OUTPUT))
سره open(baseline_filename, "rb") كـ baseline:
	سره open(OUTPUT, "rb") كـ generated:
		إذا baseline.اقرأ() != generated.اقرأ():
			shutil.copyfile (OUTPUT, baseline_filename)
