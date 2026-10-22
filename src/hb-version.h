/*
 * Copyright © 2011  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Google Author(s): Behdad Esfahbod
 */

#ఒకవేళ !defined(HB_H_లోపల) && !defined(HB_NO_SINGLE_శీర్షిక_ERROR)
#దోషం "Include <hb.h> instead."
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_సంస్కరణ_H
#నిర్వచించు HB_సంస్కరణ_H

#అన్తర్భావ "hb-common.h"

HB_BEGIN_DECLS


/**
 * HB_VERSION_MAJOR:
 *
 * The major component of the library version available at compile-time.
 */
#నిర్వచించు HB_సంస్కరణ_MAJOR 14
/**
 * HB_VERSION_MINOR:
 *
 * The minor component of the library version available at compile-time.
 */
#నిర్వచించు HB_సంస్కరణ_MINOR 6
/**
 * HB_VERSION_MICRO:
 *
 * The micro component of the library version available at compile-time.
 */
#నిర్వచించు HB_సంస్కరణ_MICRO 0

/**
 * HB_VERSION_STRING:
 *
 * A string literal containing the library version available at compile-time.
 */
#నిర్వచించు HB_సంస్కరణ_STRING "14.6.0"

/**
 * HB_VERSION_ATLEAST:
 * @major: the major component of the version number
 * @minor: the minor component of the version number
 * @micro: the micro component of the version number
 *
 * Tests the library version at compile-time against a minimum value,
 * as three integer components.
 */
#నిర్వచించు HB_సంస్కరణ_ATLEAST(major,minor,micro) \
	((major)*10000+(minor)*100+(micro) <= \
	 HB_సంస్కరణ_MAJOR*10000+HB_సంస్కరణ_MINOR*100+HB_సంస్కరణ_MICRO)


HB_EXTERN శూన్య
hb_సంస్కరణ (గుర్తులేని పూర్ణాంకం *major,
	    గుర్తులేని పూర్ణాంకం *minor,
	    గుర్తులేని పూర్ణాంకం *micro);

HB_EXTERN స్థిరరాశి అక్షరం *
hb_సంస్కరణ_string (శూన్య);

HB_EXTERN hb_bool_t
hb_సంస్కరణ_atleast (గుర్తులేని పూర్ణాంకం major,
		    గుర్తులేని పూర్ణాంకం minor,
		    గుర్తులేని పూర్ణాంకం micro);


HB_చివర_DECLS

#అయితే_ముగింపు /* HB_VERSION_H */
