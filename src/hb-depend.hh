/*
 * Copyright © 2024  Adobe, Inc.
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
 * Adobe Author(s): Skef Iterum
 */

#నిర్వచితం_కాకపోతే HB_DEPEND_HH
#నిర్వచించు HB_DEPEND_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-subset-depend.h"
#అన్తర్భావ "hb-depend-data.hh"


/**
 * hb_subset_depend_t:
 *
 * Internal structure implementing the dependency graph API.
 *
 * Initialized via hb_subset_depend_from_face_or_fail() which computes the dependency
 * graph once via hb_depend_data_builder_t::compile(). The graph remains
 * immutable for the lifetime of the object.
 */
నిర్మాణం hb_subset_depend_t
{
  HB_అంతర్గత hb_subset_depend_t (hb_face_t *face);

  hb_object_శీర్షిక_t శీర్షిక;

  తార్కిక successful;

  తార్కిక లోపల_error () స్థిరరాశి { తిరిగి !successful; }

  స్థిరరాశి hb_సమితి_t *get_సమితి_from_సూచీ (hb_codepoint_t సూచీ_2)
  {
    తిరిగి దత్తాంశం.get_సమితి_from_సూచీ (సూచీ_2);
  }

  hb_depend_దత్తాంశం_t దత్తాంశం;
};


#అయితే_ముగింపు /* HB_DEPEND_HH */
