/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2012,2013  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#నిర్వచితం_కాకపోతే HB_OT_FACE_HH
#నిర్వచించు HB_OT_FACE_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-machinery.hh"


/*
 * hb_ot_face_t
 */

/* Declare tables. */
#నిర్వచించు HB_OT_పట్టిక(Namespace, Type) నామాకాశ Namespace { నిర్మాణం Type; }
#నిర్వచించు HB_OT_ACCELERATOR(Namespace, Type) HB_OT_పట్టిక (Namespace, Type##_accelerator_t)
#అన్తర్భావ "hb-ot-face-table-list.hh"
#నిర్వచనం_రద్దు HB_OT_ACCELERATOR
#నిర్వచనం_రద్దు HB_OT_పట్టిక

నిర్మాణం hb_ot_face_t
{
  HB_అంతర్గత శూన్య init0 (hb_face_t *face);
  HB_అంతర్గత శూన్య fini ();

#నిర్వచించు HB_OT_పట్టిక_క్రమం(Namespace, Type) \
    HB_PASTE (క్రమం_2, HB_PASTE (Namespace, HB_PASTE (_, Type)))
  గణన క్రమం_t
  {
    క్రమం_ZERO,
#నిర్వచించు HB_OT_పట్టిక(Namespace, Type) HB_OT_పట్టిక_క్రమం (Namespace, Type),
#అన్తర్భావ "hb-ot-face-table-list.hh"
#నిర్వచనం_రద్దు HB_OT_పట్టిక
  };

  hb_face_t *face; /* MUST be JUST before the lazy loaders. */
#నిర్వచించు HB_OT_పట్టిక(Namespace, Type) \
  hb_పట్టిక_lazy_loader_t<Namespace::Type, HB_OT_పట్టిక_క్రమం (Namespace, Type)> Type;
#నిర్వచించు HB_OT_ACCELERATOR(Namespace, Type) \
  hb_face_lazy_loader_t<Namespace::Type##_accelerator_t, HB_OT_పట్టిక_క్రమం (Namespace, Type)> Type;
#అన్తర్భావ "hb-ot-face-table-list.hh"
#నిర్వచనం_రద్దు HB_OT_ACCELERATOR
#నిర్వచనం_రద్దు HB_OT_పట్టిక
};


#అయితే_ముగింపు /* HB_OT_FACE_HH */
