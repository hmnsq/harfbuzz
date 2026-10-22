#!/usr/bin/env python3

"This tool is intended to be used from meson"

درآمد os, sys, shutil, re

اگر لمبائی (sys.argv) < 4:
	sys.exit(__doc__)

نسخہ = sys.argv[1]
major, minor, micro = نسخہ.بانٹو (".")

OUTPUT = sys.argv[2]
داخلہ_2 = sys.argv[3]
CURRENT_ماخذ_DIR = os.راستہ.dirname(داخلہ_2)

کوشش:
	ساتھ open (OUTPUT, "r", encoding='utf-8') بطور old_output:
		برائے سطر میں old_output:
			old_نسخہ = re.مطابقت (r"#define HB_VERSION_STRING \"(\d.\d.\d)\"", سطر)
			اگر old_نسخہ اور old_نسخہ[1] == نسخہ:
				sys.exit ()
استثنا IOError:
	گزرو

ساتھ open (داخلہ_2, "r", encoding='utf-8') بطور template:
	ساتھ open (OUTPUT, "wb") بطور output:
		output.اكتب (template.اقرأ ()
			.replace ("@HB_VERSION_MAJOR@", major)
			.replace ("@HB_VERSION_MINOR@", minor)
			.replace ("@HB_VERSION_MICRO@", micro)
			.replace ("@HB_VERSION@", نسخہ)
			.encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.راستہ.join (CURRENT_ماخذ_DIR, os.راستہ.basename (OUTPUT))
ساتھ open(baseline_filename, "rb") بطور baseline:
	ساتھ open(OUTPUT, "rb") بطور generated:
		اگر baseline.اقرأ() != generated.اقرأ():
			shutil.copyfile (OUTPUT, baseline_filename)
