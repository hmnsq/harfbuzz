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

#অন্তর্ভাব "hb.hh"

#ifdef HAVE_WASM

#অন্তর্ভাব "hb-wasm-api.hh"

#define module_inst wasm_runtime_get_module_inst (exec_env)


#অন্তর্ভাব "hb-wasm-api-blob.hh"
#অন্তর্ভাব "hb-wasm-api-buffer.hh"
#অন্তর্ভাব "hb-wasm-api-common.hh"
#অন্তর্ভাব "hb-wasm-api-face.hh"
#অন্তর্ভাব "hb-wasm-api-font.hh"
#অন্তর্ভাব "hb-wasm-api-shape.hh"


#undef module_inst

hb_user_তথ্য_চাবি_t hb_wasm_ref_type_চাবি = {};

#endif
