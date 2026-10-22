#!/usr/bin/env python3

"This tool is intended to be used from meson"

దిగుమతి os, sys, shutil, re

ఒకవేళ పొడవు (sys.argv) < 4:
	sys.exit(__doc__)

సంస్కరణ = sys.argv[1]
major, minor, micro = సంస్కరణ.విభజించు (".")

OUTPUT = sys.argv[2]
ఆదానం_2 = sys.argv[3]
CURRENT_ఆధారం_DIR = os.మార్గం.dirname(ఆదానం_2)

ప్రయత్నించు:
	తోడుగా open (OUTPUT, "r", encoding='utf-8') ఇవ old_output:
		కొరకు పంక్తి లో old_output:
			old_సంస్కరణ = re.సరిపోలిక (r"#define HB_VERSION_STRING \"(\d.\d.\d)\"", పంక్తి)
			ఒకవేళ old_సంస్కరణ మరియు old_సంస్కరణ[1] == సంస్కరణ:
				sys.exit ()
మినహాయింపు IOError:
	దాటు

తోడుగా open (ఆదానం_2, "r", encoding='utf-8') ఇవ template:
	తోడుగా open (OUTPUT, "wb") ఇవ output:
		output.లిఖ (template.పఠ ()
			.replace ("@HB_VERSION_MAJOR@", major)
			.replace ("@HB_VERSION_MINOR@", minor)
			.replace ("@HB_VERSION_MICRO@", micro)
			.replace ("@HB_VERSION@", సంస్కరణ)
			.encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.మార్గం.join (CURRENT_ఆధారం_DIR, os.మార్గం.basename (OUTPUT))
తోడుగా open(baseline_filename, "rb") ఇవ baseline:
	తోడుగా open(OUTPUT, "rb") ఇవ generated:
		ఒకవేళ baseline.పఠ() != generated.పఠ():
			shutil.copyfile (OUTPUT, baseline_filename)
