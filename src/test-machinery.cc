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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-machinery.hh"

ഘടന hb_intp_lazy_loader_t : hb_lazy_loader_t<പൂർണ്ണസംഖ്യ, hb_intp_lazy_loader_t>
{
  സ്ഥിതം പൂർണ്ണസംഖ്യ* create () { തിരികെ nullptr; }
  സ്ഥിതം ശൂന്യം destroy (പൂർണ്ണസംഖ്യ* l) {}
  സ്ഥിതം പൂർണ്ണസംഖ്യ* get_null () { തിരികെ nullptr; }
};

ഘടന hb_void_lazy_loader_t : hb_lazy_loader_t<ശൂന്യം, hb_void_lazy_loader_t>
{
  സ്ഥിതം ശൂന്യം* create () { തിരികെ nullptr; }
  സ്ഥിതം ശൂന്യം destroy (ശൂന്യം* l) {}
  സ്ഥിതം ശൂന്യം* get_null () { തിരികെ nullptr; }
};

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  തിരികെ 0;
}
