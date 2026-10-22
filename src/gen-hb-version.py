#!/usr/bin/env python3

"This tool is intended to be used from meson"

استورد os, sys, shutil, re

إذا ڊيگهه (sys.argv) < 4:
	sys.exit(__doc__)

نسخو = sys.argv[1]
major, minor, micro = نسخو.ورهايو (".")

OUTPUT = sys.argv[2]
داخلا_3 = sys.argv[3]
CURRENT_ذريعو_DIR = os.رستو.dirname(داخلا_3)

حاول:
	سان open (OUTPUT, "r", encoding='utf-8') كـ old_output:
		لكل سٽ في old_output:
			old_نسخو = re.ميلاپ (r"#define HB_VERSION_STRING \"(\d.\d.\d)\"", سٽ)
			إذا old_نسخو و old_نسخو[1] == نسخو:
				sys.exit ()
استثناء IOError:
	لنگهه

سان open (داخلا_3, "r", encoding='utf-8') كـ template:
	سان open (OUTPUT, "wb") كـ output:
		output.اكتب (template.اقرأ ()
			.replace ("@HB_VERSION_MAJOR@", major)
			.replace ("@HB_VERSION_MINOR@", minor)
			.replace ("@HB_VERSION_MICRO@", micro)
			.replace ("@HB_VERSION@", نسخو)
			.encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.رستو.join (CURRENT_ذريعو_DIR, os.رستو.basename (OUTPUT))
سان open(baseline_filename, "rb") كـ baseline:
	سان open(OUTPUT, "rb") كـ generated:
		إذا baseline.اقرأ() != generated.اقرأ():
			shutil.copyfile (OUTPUT, baseline_filename)
