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

#شامل "hb.hh"
#شامل "hb-machinery.hh"

ساختار hb_intp_lazy_loader_t : hb_lazy_loader_t<صحیح, hb_intp_lazy_loader_t>
{
  ایستا صحیح* create () { بازگشت nullptr; }
  ایستا تهی destroy (صحیح* l) {}
  ایستا صحیح* get_null () { بازگشت nullptr; }
};

ساختار hb_void_lazy_loader_t : hb_lazy_loader_t<تهی, hb_void_lazy_loader_t>
{
  ایستا تهی* create () { بازگشت nullptr; }
  ایستا تهی destroy (تهی* l) {}
  ایستا تهی* get_null () { بازگشت nullptr; }
};

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  بازگشت 0;
}
