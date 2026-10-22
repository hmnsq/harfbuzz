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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-machinery.hh"

ಸಂರಚನಾ hb_intp_lazy_loader_t : hb_lazy_loader_t<ಪೂರ್ಣಾಙ್ಕ, hb_intp_lazy_loader_t>
{
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ* create () { ಪ್ರತ್ಯಾಗಮ nullptr; }
  ಸ್ಥಿರ ಶೂನ್ಯ destroy (ಪೂರ್ಣಾಙ್ಕ* l) {}
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ* get_null () { ಪ್ರತ್ಯಾಗಮ nullptr; }
};

ಸಂರಚನಾ hb_void_lazy_loader_t : hb_lazy_loader_t<ಶೂನ್ಯ, hb_void_lazy_loader_t>
{
  ಸ್ಥಿರ ಶೂನ್ಯ* create () { ಪ್ರತ್ಯಾಗಮ nullptr; }
  ಸ್ಥಿರ ಶೂನ್ಯ destroy (ಶೂನ್ಯ* l) {}
  ಸ್ಥಿರ ಶೂನ್ಯ* get_null () { ಪ್ರತ್ಯಾಗಮ nullptr; }
};

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪ್ರತ್ಯಾಗಮ 0;
}
