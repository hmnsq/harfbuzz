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

#অন্তর্ভাৱ "hb.hh"
#অন্তর্ভাৱ "hb-machinery.hh"

সংরচনা hb_intp_lazy_loader_t : hb_lazy_loader_t<পূর্ণাঙ্ক, hb_intp_lazy_loader_t>
{
  স্থির পূর্ণাঙ্ক* create () { ঘূৰাই nullptr; }
  স্থির শূন্য destroy (পূর্ণাঙ্ক* l) {}
  স্থির পূর্ণাঙ্ক* get_null () { ঘূৰাই nullptr; }
};

সংরচনা hb_void_lazy_loader_t : hb_lazy_loader_t<শূন্য, hb_void_lazy_loader_t>
{
  স্থির শূন্য* create () { ঘূৰাই nullptr; }
  স্থির শূন্য destroy (শূন্য* l) {}
  স্থির শূন্য* get_null () { ঘূৰাই nullptr; }
};

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  ঘূৰাই 0;
}
