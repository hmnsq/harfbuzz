#!/usr/bin/env python3

"This tool is intended to be used from meson"

ਆਯਾਤ os, sys, shutil, re

ਜੇ ਲੰਬਾਈ (sys.argv) < 4:
	sys.exit(__doc__)

ਸੰਸਕਰਣ = sys.argv[1]
major, minor, micro = ਸੰਸਕਰਣ.ਵੰਡੋ (".")

OUTPUT = sys.argv[2]
ਨਿਵੇਸ਼_2 = sys.argv[3]
CURRENT_ਸਰੋਤ_DIR = os.ਰਸਤਾ.dirname(ਨਿਵੇਸ਼_2)

ਕੋਸ਼ਿਸ਼:
	ਨਾਲ open (OUTPUT, "r", encoding='utf-8') ਇਵ old_output:
		ਲਈ ਸਤਰ ਵਿੱਚ old_output:
			old_ਸੰਸਕਰਣ = re.ਮੇਲ (r"#define HB_VERSION_STRING \"(\d.\d.\d)\"", ਸਤਰ)
			ਜੇ old_ਸੰਸਕਰਣ ਅਤੇ old_ਸੰਸਕਰਣ[1] == ਸੰਸਕਰਣ:
				sys.exit ()
ਅਪਵਾਦ IOError:
	ਲੰਘੋ

ਨਾਲ open (ਨਿਵੇਸ਼_2, "r", encoding='utf-8') ਇਵ template:
	ਨਾਲ open (OUTPUT, "wb") ਇਵ output:
		output.ਲਿਖ (template.ਪਠ ()
			.replace ("@HB_VERSION_MAJOR@", major)
			.replace ("@HB_VERSION_MINOR@", minor)
			.replace ("@HB_VERSION_MICRO@", micro)
			.replace ("@HB_VERSION@", ਸੰਸਕਰਣ)
			.encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.ਰਸਤਾ.join (CURRENT_ਸਰੋਤ_DIR, os.ਰਸਤਾ.basename (OUTPUT))
ਨਾਲ open(baseline_filename, "rb") ਇਵ baseline:
	ਨਾਲ open(OUTPUT, "rb") ਇਵ generated:
		ਜੇ baseline.ਪਠ() != generated.ਪਠ():
			shutil.copyfile (OUTPUT, baseline_filename)
