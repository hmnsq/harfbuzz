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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-machinery.hh"

संरचना hb_intp_lazy_loader_t : hb_lazy_loader_t<पूर्णांक, hb_intp_lazy_loader_t>
{
  स्थिर पूर्णांक* create () { परत nullptr; }
  स्थिर शून्य destroy (पूर्णांक* l) {}
  स्थिर पूर्णांक* get_null () { परत nullptr; }
};

संरचना hb_void_lazy_loader_t : hb_lazy_loader_t<शून्य, hb_void_lazy_loader_t>
{
  स्थिर शून्य* create () { परत nullptr; }
  स्थिर शून्य destroy (शून्य* l) {}
  स्थिर शून्य* get_null () { परत nullptr; }
};

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  परत 0;
}
