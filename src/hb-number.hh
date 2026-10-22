/*
 * Copyright © 2019  Ebrahim Byagowi
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
 */

#నిర్వచితం_కాకపోతే HB_సంఖ్య_HH
#నిర్వచించు HB_సంఖ్య_HH

HB_అంతర్గత తార్కిక
hb_parse_int (స్థిరరాశి అక్షరం **pp, స్థిరరాశి అక్షరం *చివర, పూర్ణాంకం *pv,
	      తార్కిక సంపూర్ణ_బఫరు = అబద్ధం);

HB_అంతర్గత తార్కిక
hb_parse_uint (స్థిరరాశి అక్షరం **pp, స్థిరరాశి అక్షరం *చివర, గుర్తులేని పూర్ణాంకం *pv,
	       తార్కిక సంపూర్ణ_బఫరు = అబద్ధం, పూర్ణాంకం పునాది = 10);

HB_అంతర్గత తార్కిక
hb_parse_double (స్థిరరాశి అక్షరం **pp, స్థిరరాశి అక్షరం *చివర, ద్విగుణ *pv,
		 తార్కిక సంపూర్ణ_బఫరు = అబద్ధం);

#అయితే_ముగింపు /* HB_NUMBER_HH */
