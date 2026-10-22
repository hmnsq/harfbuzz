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

#अगर_अघोषित HB_WASM_API_बफर_HH
#घोषणा HB_WASM_API_बफर_HH

#समावेश "hb-wasm-api.hh"

#समावेश "hb-buffer.hh"

namespace hb {
namespace wasm {

static_assert (माप (glyph_info_t) == माप (hb_glyph_info_t), "");
static_assert (माप (glyph_position_t) == माप (hb_glyph_position_t), "");

HB_WASM_API (bool_t, बफर_contents_realloc) (HB_WASM_EXEC_ENV
					       ptr_d(बफर_contents_t, contents),
					       uint32_t आकार)
{
  HB_PTR_PARAM (बफर_contents_t, contents);
  अगर (unlikely (!contents))
    वापस false;

  अगर (आकार <= contents->लंबाई)
    वापस true;

  अचिन्हित bytes;
  अगर (hb_unsigned_mul_overflows (आकार, माप (glyph_info_t), &bytes))
    वापस false;

  glyph_info_t *info = HB_ARRAY_APP2NATIVE (glyph_info_t, contents->info, contents->लंबाई);
  glyph_position_t *pos = HB_ARRAY_APP2NATIVE (glyph_position_t, contents->pos, contents->लंबाई);

  अगर (unlikely (!info || !pos))
    वापस false;

  glyph_info_t *new_info = nullptr;
  uint32_t new_inforef = module_malloc (bytes, (व्योम **) &new_info);
  glyph_position_t *new_pos = nullptr;
  uint32_t new_posref = module_malloc (bytes, (व्योम **) &new_pos);

  अचिन्हित old_bytes = contents->लंबाई * माप (glyph_info_t);
  अगर (likely (new_inforef))
  {
    hb_memcpy (new_info, info, old_bytes);
    module_free (contents->info);
    contents->info = new_inforef;
  }
  अगर (likely (new_posref))
  {
    hb_memcpy (new_pos, pos, old_bytes);
    module_free (contents->pos);
    contents->pos = new_posref;
  }

  अगर (likely (new_info && new_pos))
  {
    contents->लंबाई = आकार;
    वापस true;
  }

  वापस false;
}

HB_WASM_API (व्योम, बफर_contents_free) (HB_WASM_EXEC_ENV
					  ptr_d(बफर_contents_t, contents))
{
  HB_PTR_PARAM (बफर_contents_t, contents);
  अगर (unlikely (!contents))
    वापस;

  module_free (contents->info);
  module_free (contents->pos);

  contents->info = nullref;
  contents->pos = nullref;
  contents->लंबाई = 0;
}

HB_WASM_API (bool_t, बफर_copy_contents) (HB_WASM_EXEC_ENV
					    ptr_d(बफर_t, बफर),
					    ptr_d(बफर_contents_t, contents))
{
  HB_REF2OBJ (बफर);
  HB_PTR_PARAM (बफर_contents_t, contents);
  अगर (unlikely (!contents))
    वापस false;

  अगर (बफर->have_output)
    बफर->sync ();
  अगर (!बफर->have_positions)
    बफर->clear_positions ();

  अचिन्हित लंबाई = बफर->len;

  अगर (लंबाई <= contents->लंबाई)
  {
    glyph_info_t *info = HB_ARRAY_APP2NATIVE (glyph_info_t, contents->info, लंबाई);
    glyph_position_t *pos = HB_ARRAY_APP2NATIVE (glyph_position_t, contents->pos, लंबाई);

    अगर (unlikely (!info || !pos))
    {
      contents->लंबाई = 0;
      वापस false;
    }

    अचिन्हित bytes = लंबाई * माप (hb_glyph_info_t);
    hb_memcpy (info, बफर->info, bytes);
    hb_memcpy (pos, बफर->pos, bytes);

    वापस true;
  }

  module_free (contents->info);
  module_free (contents->pos);

  contents->लंबाई = लंबाई;
  अचिन्हित bytes = लंबाई * माप (hb_glyph_info_t);
  contents->info = wasm_runtime_module_dup_तथ्य (module_inst, (स्थिर अक्षर *) बफर->info, bytes);
  contents->pos = wasm_runtime_module_dup_तथ्य (module_inst, (स्थिर अक्षर *) बफर->pos, bytes);

  अगर (लंबाई && (!contents->info || !contents->pos))
  {
    contents->लंबाई = 0;
    वापस false;
  }

  वापस true;
}

HB_WASM_API (bool_t, बफर_set_contents) (HB_WASM_EXEC_ENV
					   ptr_d(बफर_t, बफर),
					   ptr_d(स्थिर बफर_contents_t, contents))
{
  HB_REF2OBJ (बफर);
  HB_PTR_PARAM (बफर_contents_t, contents);
  अगर (unlikely (!contents))
    वापस false;

  अचिन्हित लंबाई = contents->लंबाई;
  अचिन्हित bytes;
  अगर (unlikely (hb_unsigned_mul_overflows (लंबाई, माप (बफर->info[0]), &bytes)))
    वापस false;

  glyph_info_t *info = (glyph_info_t *) (validate_app_addr (contents->info, bytes) ? addr_app_to_native (contents->info) : nullptr);
  glyph_position_t *pos = (glyph_position_t *) (validate_app_addr (contents->pos, bytes) ? addr_app_to_native (contents->pos) : nullptr);
  अगर (unlikely (!info || !pos))
    वापस false;

  अगर (unlikely (!बफर->resize (लंबाई)))
    वापस false;

  अगर (!बफर->have_positions)
    बफर->clear_positions (); /* This is wasteful. */

  hb_memcpy (बफर->info, info, bytes);
  hb_memcpy (बफर->pos, pos, bytes);
  बफर->len = लंबाई;

  वापस true;
}

HB_WASM_API (direction_t, बफर_get_direction) (HB_WASM_EXEC_ENV
						 ptr_d(बफर_t, बफर))
{
  HB_REF2OBJ (बफर);

  वापस (direction_t) hb_बफर_get_direction (बफर);
}

HB_WASM_API (script_t, बफर_get_script) (HB_WASM_EXEC_ENV
					   ptr_d(बफर_t, बफर))
{
  HB_REF2OBJ (बफर);

  वापस hb_script_to_iso15924_tag (hb_बफर_get_script (बफर));
}

HB_WASM_API (व्योम, बफर_reverse) (HB_WASM_EXEC_ENV
				    ptr_d(बफर_t, बफर))
{
  HB_REF2OBJ (बफर);

  hb_बफर_reverse (बफर);
}

HB_WASM_API (व्योम, बफर_reverse_clusters) (HB_WASM_EXEC_ENV
					     ptr_d(बफर_t, बफर))
{
  HB_REF2OBJ (बफर);

  hb_बफर_reverse_clusters (बफर);
}

}}

#पूर्ण_अगर /* HB_WASM_API_BUFFER_HH */
