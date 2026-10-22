/*
 * Copyright © 2022  Behdad Esfahbod
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
 */

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-machinery.hh"

నిర్మాణం hb_intp_lazy_loader_t : hb_lazy_loader_t<పూర్ణాంకం, hb_intp_lazy_loader_t>
{
  స్థిర పూర్ణాంకం* create () { తిరిగి శూన్యసూచిక; }
  స్థిర శూన్య destroy (పూర్ణాంకం* l) {}
  స్థిర పూర్ణాంకం* get_null () { తిరిగి శూన్యసూచిక; }
};

నిర్మాణం hb_void_lazy_loader_t : hb_lazy_loader_t<శూన్య, hb_void_lazy_loader_t>
{
  స్థిర శూన్య* create () { తిరిగి శూన్యసూచిక; }
  స్థిర శూన్య destroy (శూన్య* l) {}
  స్థిర శూన్య* get_null () { తిరిగి శూన్యసూచిక; }
};

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  తిరిగి 0;
}
