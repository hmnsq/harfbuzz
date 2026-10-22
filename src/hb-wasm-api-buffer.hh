/*
 * Copyright © 2023  Behdad Esfahbod
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

#నిర్వచితం_కాకపోతే HB_WASM_API_బఫరు_HH
#నిర్వచించు HB_WASM_API_బఫరు_HH

#అన్తర్భావ "hb-wasm-api.hh"

#అన్తర్భావ "hb-buffer.hh"

నామాకాశ hb {
నామాకాశ wasm {

static_assert (పరిమాణం (glyph_info_t) == పరిమాణం (hb_glyph_info_t), "");
static_assert (పరిమాణం (glyph_position_t) == పరిమాణం (hb_glyph_position_t), "");

HB_WASM_API (bool_t, బఫరు_contents_realloc) (HB_WASM_EXEC_ENV
					       ptr_d(బఫరు_contents_t, contents),
					       uint32_t కొలత)
{
  HB_PTR_PARAM (బఫరు_contents_t, contents);
  ఒకవేళ (unlikely (!contents))
    తిరిగి అబద్ధం;

  ఒకవేళ (కొలత <= contents->నిడివి)
    తిరిగి నిజం;

  గుర్తులేని బైట్లు;
  ఒకవేళ (hb_unsigned_mul_overflows (కొలత, పరిమాణం (glyph_info_t), &బైట్లు))
    తిరిగి అబద్ధం;

  glyph_info_t *info = HB_ARRAY_APP2NATIVE (glyph_info_t, contents->info, contents->నిడివి);
  glyph_position_t *pos = HB_ARRAY_APP2NATIVE (glyph_position_t, contents->pos, contents->నిడివి);

  ఒకవేళ (unlikely (!info || !pos))
    తిరిగి అబద్ధం;

  glyph_info_t *new_info = శూన్యసూచిక;
  uint32_t new_inforef = module_malloc (బైట్లు, (శూన్య **) &new_info);
  glyph_position_t *new_pos = శూన్యసూచిక;
  uint32_t new_posref = module_malloc (బైట్లు, (శూన్య **) &new_pos);

  గుర్తులేని old_బైట్లు = contents->నిడివి * పరిమాణం (glyph_info_t);
  ఒకవేళ (likely (new_inforef))
  {
    hb_memcpy (new_info, info, old_బైట్లు);
    module_free (contents->info);
    contents->info = new_inforef;
  }
  ఒకవేళ (likely (new_posref))
  {
    hb_memcpy (new_pos, pos, old_బైట్లు);
    module_free (contents->pos);
    contents->pos = new_posref;
  }

  ఒకవేళ (likely (new_info && new_pos))
  {
    contents->నిడివి = కొలత;
    తిరిగి నిజం;
  }

  తిరిగి అబద్ధం;
}

HB_WASM_API (శూన్య, బఫరు_contents_free) (HB_WASM_EXEC_ENV
					  ptr_d(బఫరు_contents_t, contents))
{
  HB_PTR_PARAM (బఫరు_contents_t, contents);
  ఒకవేళ (unlikely (!contents))
    తిరిగి;

  module_free (contents->info);
  module_free (contents->pos);

  contents->info = nullref;
  contents->pos = nullref;
  contents->నిడివి = 0;
}

HB_WASM_API (bool_t, బఫరు_copy_contents) (HB_WASM_EXEC_ENV
					    ptr_d(బఫరు_t, బఫరు),
					    ptr_d(బఫరు_contents_t, contents))
{
  HB_REF2OBJ (బఫరు);
  HB_PTR_PARAM (బఫరు_contents_t, contents);
  ఒకవేళ (unlikely (!contents))
    తిరిగి అబద్ధం;

  ఒకవేళ (బఫరు->have_output)
    బఫరు->సమకాలీకరణ ();
  ఒకవేళ (!బఫరు->have_positions)
    బఫరు->clear_positions ();

  గుర్తులేని నిడివి = బఫరు->len;

  ఒకవేళ (నిడివి <= contents->నిడివి)
  {
    glyph_info_t *info = HB_ARRAY_APP2NATIVE (glyph_info_t, contents->info, నిడివి);
    glyph_position_t *pos = HB_ARRAY_APP2NATIVE (glyph_position_t, contents->pos, నిడివి);

    ఒకవేళ (unlikely (!info || !pos))
    {
      contents->నిడివి = 0;
      తిరిగి అబద్ధం;
    }

    గుర్తులేని బైట్లు = నిడివి * పరిమాణం (hb_glyph_info_t);
    hb_memcpy (info, బఫరు->info, బైట్లు);
    hb_memcpy (pos, బఫరు->pos, బైట్లు);

    తిరిగి నిజం;
  }

  module_free (contents->info);
  module_free (contents->pos);

  contents->నిడివి = నిడివి;
  గుర్తులేని బైట్లు = నిడివి * పరిమాణం (hb_glyph_info_t);
  contents->info = wasm_runtime_module_dup_దత్తాంశం (module_inst, (స్థిరరాశి అక్షరం *) బఫరు->info, బైట్లు);
  contents->pos = wasm_runtime_module_dup_దత్తాంశం (module_inst, (స్థిరరాశి అక్షరం *) బఫరు->pos, బైట్లు);

  ఒకవేళ (నిడివి && (!contents->info || !contents->pos))
  {
    contents->నిడివి = 0;
    తిరిగి అబద్ధం;
  }

  తిరిగి నిజం;
}

HB_WASM_API (bool_t, బఫరు_సమితి_contents) (HB_WASM_EXEC_ENV
					   ptr_d(బఫరు_t, బఫరు),
					   ptr_d(స్థిరరాశి బఫరు_contents_t, contents))
{
  HB_REF2OBJ (బఫరు);
  HB_PTR_PARAM (బఫరు_contents_t, contents);
  ఒకవేళ (unlikely (!contents))
    తిరిగి అబద్ధం;

  గుర్తులేని నిడివి = contents->నిడివి;
  గుర్తులేని బైట్లు;
  ఒకవేళ (unlikely (hb_unsigned_mul_overflows (నిడివి, పరిమాణం (బఫరు->info[0]), &బైట్లు)))
    తిరిగి అబద్ధం;

  glyph_info_t *info = (glyph_info_t *) (validate_app_addr (contents->info, బైట్లు) ? addr_app_గమ్యం_native (contents->info) : శూన్యసూచిక);
  glyph_position_t *pos = (glyph_position_t *) (validate_app_addr (contents->pos, బైట్లు) ? addr_app_గమ్యం_native (contents->pos) : శూన్యసూచిక);
  ఒకవేళ (unlikely (!info || !pos))
    తిరిగి అబద్ధం;

  ఒకవేళ (unlikely (!బఫరు->resize (నిడివి)))
    తిరిగి అబద్ధం;

  ఒకవేళ (!బఫరు->have_positions)
    బఫరు->clear_positions (); /* This is wasteful. */

  hb_memcpy (బఫరు->info, info, బైట్లు);
  hb_memcpy (బఫరు->pos, pos, బైట్లు);
  బఫరు->len = నిడివి;

  తిరిగి నిజం;
}

HB_WASM_API (direction_t, బఫరు_get_direction) (HB_WASM_EXEC_ENV
						 ptr_d(బఫరు_t, బఫరు))
{
  HB_REF2OBJ (బఫరు);

  తిరిగి (direction_t) hb_బఫరు_get_direction (బఫరు);
}

HB_WASM_API (script_t, బఫరు_get_script) (HB_WASM_EXEC_ENV
					   ptr_d(బఫరు_t, బఫరు))
{
  HB_REF2OBJ (బఫరు);

  తిరిగి hb_script_గమ్యం_iso15924_ట్యాగ్ (hb_బఫరు_get_script (బఫరు));
}

HB_WASM_API (శూన్య, బఫరు_తిరగేయి) (HB_WASM_EXEC_ENV
				    ptr_d(బఫరు_t, బఫరు))
{
  HB_REF2OBJ (బఫరు);

  hb_బఫరు_తిరగేయి (బఫరు);
}

HB_WASM_API (శూన్య, బఫరు_తిరగేయి_clusters) (HB_WASM_EXEC_ENV
					     ptr_d(బఫరు_t, బఫరు))
{
  HB_REF2OBJ (బఫరు);

  hb_బఫరు_తిరగేయి_clusters (బఫరు);
}

}}

#అయితే_ముగింపు /* HB_WASM_API_BUFFER_HH */
