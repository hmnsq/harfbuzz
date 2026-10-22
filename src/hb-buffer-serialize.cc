/*
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
 * Google Author(s): Behdad Esfahbod
 */

#ضمن "hb.hh"

#که_نه_تعریف_شوی HB_NO_بفر_SERIALIZE

#ضمن "hb-buffer.hh"


ساكن ثابت حرف *hb_بفر_serialize_formats[] = {
  "text",
  "json",
  تش_اشاره
};

/**
 * hb_buffer_serialize_list_formats:
 *
 * Returns a list of supported buffer serialization formats.
 *
 * Return value: (transfer none):
 * A string array of buffer serialization formats. Should not be freed.
 *
 * Since: 0.9.7
 **/
ثابت حرف **
hb_بفر_serialize_list_formats ()
{
  أرجع hb_بفر_serialize_formats;
}

/**
 * hb_buffer_serialize_format_from_string:
 * @str: (array length=len) (element-type uint8_t): a string to parse
 * @len: length of @str, or -1 if string is `NULL` terminated
 *
 * Parses a string into an #hb_buffer_serialize_format_t. Does not check if
 * @str is a valid buffer serialization format, use
 * hb_buffer_serialize_list_formats() to get the list of supported formats.
 *
 * Return value:
 * The parsed #hb_buffer_serialize_format_t.
 *
 * Since: 0.9.7
 **/
hb_بفر_serialize_format_t
hb_بفر_serialize_format_from_string (ثابت حرف *str, صحيح len)
{
  /* Upper-case it. */
  أرجع (hb_بفر_serialize_format_t) (hb_نښان_from_string (str, len) & ~0x20202020u);
}

/**
 * hb_buffer_serialize_format_to_string:
 * @format: an #hb_buffer_serialize_format_t to convert.
 *
 * Converts @format to the string corresponding it, or `NULL` if it is not a valid
 * #hb_buffer_serialize_format_t.
 *
 * Return value: (transfer none):
 * A `NULL` terminated string corresponding to @format. Should not be freed.
 *
 * Since: 0.9.7
 **/
ثابت حرف *
hb_بفر_serialize_format_منزل_string (hb_بفر_serialize_format_t format)
{
  بدل ((بینښه) format)
  {
    حالة HB_بفر_SERIALIZE_FORMAT_متن: أرجع hb_بفر_serialize_formats[0];
    حالة HB_بفر_SERIALIZE_FORMAT_JSON: أرجع hb_بفر_serialize_formats[1];
    افتراضي:
    حالة HB_بفر_SERIALIZE_FORMAT_INVALID:  أرجع تش_اشاره;
  }
}

ساكن بینښه صحيح
hb_بفر_serialize_glyphs_json (hb_بفر_t *بفر,
                                  بینښه صحيح پیل,
                                  بینښه صحيح پای_ټکی,
                                  حرف *buf,
                                  بینښه صحيح buf_اندازه,
                                  بینښه صحيح *buf_consumed,
                                  hb_font_t *font,
                                  hb_بفر_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_بفر_get_glyph_infos (بفر, تش_اشاره);
  hb_glyph_position_t *pos = (flags & HB_بفر_SERIALIZE_FLAG_NO_POSITIONS) ?
                             تش_اشاره : hb_بفر_get_glyph_positions (بفر, تش_اشاره);

  *buf_consumed = 0;
  hb_position_t x = 0, y = 0;

  /* Calculate the advance of the previous glyphs */
  إذا (pos && (flags & HB_بفر_SERIALIZE_FLAG_NO_ADVANCES))
  {
    لكل (بینښه صحيح i = 0; i < پیل; i++)
    {
      x = hb_saturate_جمع_کړه (x, pos[i].x_advance);
      y = hb_saturate_جمع_کړه (y, pos[i].y_advance);
    }
  }

  لكل (بینښه صحيح i = پیل; i < پای_ټکی; i++)
  {
    حرف b[1024];
    حرف *p = b;

    /* In the following code, we know b is large enough that no overflow can happen. */

#تعریف_کړه APPEND(s) HB_STMT_پیل { strcpy (p, s); p += مزي_اوږدوالی (s); } HB_STMT_پای_ټکی

    إذا (i)
      *p++ = ',';
    وإلا
      *p++ = '[';

    *p++ = '{';

    APPEND ("\"g\":");
    إذا (!(flags & HB_بفر_SERIALIZE_FLAG_NO_GLYPH_نومونه))
    {
      حرف g[128];
      hb_font_glyph_منزل_string (font, info[i].codepoint, g, حجم (g));
      *p++ = '"';
      لكل (حرف *q = g; *q; q++)
      {
	إذا (unlikely (*q == '"' || *q == '\\'))
	  *p++ = '\\';
	*p++ = *q;
      }
      *p++ = '"';
    }
    وإلا
      p += hb_max (0, snprintf (p, ARRAY_اوږدتیا (b) - (p - b), "%u", info[i].codepoint));

    إذا (!(flags & HB_بفر_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_اوږدتیا (b) - (p - b), ",\"cl\":%u", info[i].cluster));
    }

    إذا (pos && !(flags & HB_بفر_SERIALIZE_FLAG_NO_POSITIONS))
    {
      hb_position_t dx = hb_saturate_جمع_کړه (x, pos[i].x_دننه_واټن_2);
      hb_position_t dy = hb_saturate_جمع_کړه (y, pos[i].y_دننه_واټن_2);
      p += hb_max (0, snprintf (p, ARRAY_اوږدتیا (b) - (p - b), ",\"dx\":%d,\"dy\":%d",
		   dx, dy));
      إذا (!(flags & HB_بفر_SERIALIZE_FLAG_NO_ADVANCES))
        p += hb_max (0, snprintf (p, ARRAY_اوږدتیا (b) - (p - b), ",\"ax\":%d,\"ay\":%d",
		     pos[i].x_advance, pos[i].y_advance));
    }

    إذا (flags & HB_بفر_SERIALIZE_FLAG_GLYPH_FLAGS)
    {
      إذا (info[i].ماسک & HB_GLYPH_FLAG_DEFINED)
        p += hb_max (0, snprintf (p, ARRAY_اوږدتیا (b) - (p - b), ",\"fl\":%u", info[i].ماسک & HB_GLYPH_FLAG_DEFINED));
    }

    إذا (flags & HB_بفر_SERIALIZE_FLAG_GLYPH_EXTENTS)
    {
      hb_glyph_extents_t extents;
      إذا (hb_font_get_glyph_extents(font, info[i].codepoint, &extents))
      {
	p += hb_max (0, snprintf (p, ARRAY_اوږدتیا (b) - (p - b), ",\"xb\":%d,\"yb\":%d",
				  extents.x_bearing, extents.y_bearing));
	p += hb_max (0, snprintf (p, ARRAY_اوږدتیا (b) - (p - b), ",\"w\":%d,\"h\":%d",
				  extents.پلنوالی, extents.لوړوالی));
      }
    }

    *p++ = '}';
    إذا (i == پای_ټکی-1)
      *p++ = ']';

    بینښه صحيح l = p - b;
    إذا (buf_اندازه > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_اندازه -= l;
      *buf_consumed += l;
      *buf = '\0';
    } وإلا
      أرجع i - پیل;

    إذا (pos && (flags & HB_بفر_SERIALIZE_FLAG_NO_ADVANCES))
    {
      x = hb_saturate_جمع_کړه (x, pos[i].x_advance);
      y = hb_saturate_جمع_کړه (y, pos[i].y_advance);
    }
  }

  أرجع پای_ټکی - پیل;
}

ساكن بینښه صحيح
hb_بفر_serialize_unicode_json (hb_بفر_t *بفر,
          بینښه صحيح پیل,
          بینښه صحيح پای_ټکی,
          حرف *buf,
          بینښه صحيح buf_اندازه,
          بینښه صحيح *buf_consumed,
          hb_بفر_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_بفر_get_glyph_infos (بفر, تش_اشاره);

  *buf_consumed = 0;
  لكل (بینښه صحيح i = پیل; i < پای_ټکی; i++)
  {
    حرف b[1024];
    حرف *p = b;

    إذا (i)
      *p++ = ',';
    وإلا
      *p++ = '[';

    *p++ = '{';

    APPEND ("\"u\":");

    p += hb_max (0, snprintf (p, ARRAY_اوږدتیا (b) - (p - b), "%u", info[i].codepoint));

    إذا (!(flags & HB_بفر_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_اوږدتیا (b) - (p - b), ",\"cl\":%u", info[i].cluster));
    }

    *p++ = '}';

    إذا (i == پای_ټکی-1)
      *p++ = ']';

    بینښه صحيح l = p - b;
    إذا (buf_اندازه > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_اندازه -= l;
      *buf_consumed += l;
      *buf = '\0';
    } وإلا
      أرجع i - پیل;

  }

  أرجع پای_ټکی - پیل;
}

ساكن بینښه صحيح
hb_بفر_serialize_glyphs_متن (hb_بفر_t *بفر,
                                  بینښه صحيح پیل,
                                  بینښه صحيح پای_ټکی,
                                  حرف *buf,
                                  بینښه صحيح buf_اندازه,
                                  بینښه صحيح *buf_consumed,
                                  hb_font_t *font,
                                  hb_بفر_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_بفر_get_glyph_infos (بفر, تش_اشاره);
  hb_glyph_position_t *pos = (flags & HB_بفر_SERIALIZE_FLAG_NO_POSITIONS) ?
           تش_اشاره : hb_بفر_get_glyph_positions (بفر, تش_اشاره);

  *buf_consumed = 0;
  hb_position_t x = 0, y = 0;

  /* Calculate the advance of the previous glyphs */
  إذا (pos && (flags & HB_بفر_SERIALIZE_FLAG_NO_ADVANCES))
  {
    لكل (بینښه صحيح i = 0; i < پیل; i++)
    {
      x = hb_saturate_جمع_کړه (x, pos[i].x_advance);
      y = hb_saturate_جمع_کړه (y, pos[i].y_advance);
    }
  }

  لكل (بینښه صحيح i = پیل; i < پای_ټکی; i++)
  {
    حرف b[1024];
    حرف *p = b;

    /* In the following code, we know b is large enough that no overflow can happen. */

    إذا (i)
      *p++ = '|';
    وإلا
      *p++ = '[';

    إذا (!(flags & HB_بفر_SERIALIZE_FLAG_NO_GLYPH_نومونه))
    {
      /* TODO Escape delimiters we use. */
      hb_font_glyph_منزل_string (font, info[i].codepoint, p, 128);
      p += مزي_اوږدوالی (p);
    }
    وإلا
      p += hb_max (0, snprintf (p, ARRAY_اوږدتیا (b) - (p - b), "%u", info[i].codepoint));

    إذا (!(flags & HB_بفر_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_اوږدتیا (b) - (p - b), "=%u", info[i].cluster));
    }

    إذا (pos && !(flags & HB_بفر_SERIALIZE_FLAG_NO_POSITIONS))
    {
      hb_position_t dx = hb_saturate_جمع_کړه (x, pos[i].x_دننه_واټن_2);
      hb_position_t dy = hb_saturate_جمع_کړه (y, pos[i].y_دننه_واټن_2);
      إذا (dx || dy)
        p += hb_max (0, snprintf (p, ARRAY_اوږدتیا (b) - (p - b), "@%d,%d", dx, dy));

      إذا (!(flags & HB_بفر_SERIALIZE_FLAG_NO_ADVANCES))
      {
        *p++ = '+';
        p += hb_max (0, snprintf (p, ARRAY_اوږدتیا (b) - (p - b), "%d", pos[i].x_advance));
        إذا (pos[i].y_advance)
          p += hb_max (0, snprintf (p, ARRAY_اوږدتیا (b) - (p - b), ",%d", pos[i].y_advance));
      }
    }

    إذا (flags & HB_بفر_SERIALIZE_FLAG_GLYPH_FLAGS)
    {
      إذا (info[i].ماسک & HB_GLYPH_FLAG_DEFINED)
        p += hb_max (0, snprintf (p, ARRAY_اوږدتیا (b) - (p - b), "#%X", info[i].ماسک &HB_GLYPH_FLAG_DEFINED));
    }

    إذا (flags & HB_بفر_SERIALIZE_FLAG_GLYPH_EXTENTS)
    {
      hb_glyph_extents_t extents;
      إذا (hb_font_get_glyph_extents(font, info[i].codepoint, &extents))
	p += hb_max (0, snprintf (p, ARRAY_اوږدتیا (b) - (p - b), "<%d,%d,%d,%d>", extents.x_bearing, extents.y_bearing, extents.پلنوالی, extents.لوړوالی));
    }

    إذا (i == پای_ټکی-1) {
      *p++ = ']';
    }

    بینښه صحيح l = p - b;
    إذا (buf_اندازه > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_اندازه -= l;
      *buf_consumed += l;
      *buf = '\0';
    } وإلا
      أرجع i - پیل;

    إذا (pos && (flags & HB_بفر_SERIALIZE_FLAG_NO_ADVANCES))
    {
      x = hb_saturate_جمع_کړه (x, pos[i].x_advance);
      y = hb_saturate_جمع_کړه (y, pos[i].y_advance);
    }
  }

  أرجع پای_ټکی - پیل;
}


ساكن بینښه صحيح
hb_بفر_serialize_unicode_متن (hb_بفر_t *بفر,
                                   بینښه صحيح پیل,
                                   بینښه صحيح پای_ټکی,
                                   حرف *buf,
                                   بینښه صحيح buf_اندازه,
                                   بینښه صحيح *buf_consumed,
                                   hb_بفر_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_بفر_get_glyph_infos (بفر, تش_اشاره);
  *buf_consumed = 0;
  لكل (بینښه صحيح i = پیل; i < پای_ټکی; i++)
  {
    حرف b[1024];
    حرف *p = b;

    إذا (i)
      *p++ = '|';
    وإلا
      *p++ = '<';

    p += hb_max (0, snprintf (p, ARRAY_اوږدتیا (b) - (p - b), "U+%04X", info[i].codepoint));

    إذا (!(flags & HB_بفر_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_اوږدتیا (b) - (p - b), "=%u", info[i].cluster));
    }

    إذا (i == پای_ټکی-1)
      *p++ = '>';

    بینښه صحيح l = p - b;
    إذا (buf_اندازه > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_اندازه -= l;
      *buf_consumed += l;
      *buf = '\0';
    } وإلا
      أرجع i - پیل;
  }
  أرجع پای_ټکی - پیل;
}

/**
 * hb_buffer_serialize_glyphs:
 * @buffer: an #hb_buffer_t buffer.
 * @start: the first item in @buffer to serialize.
 * @end: the last item in @buffer to serialize.
 * @buf: (out) (array length=buf_size) (element-type uint8_t): output string to
 *       write serialized buffer into.
 * @buf_size: the size of @buf.
 * @buf_consumed: (out) (optional): if not `NULL`, will be set to the number of bytes written into @buf.
 * @font: (nullable): the #hb_font_t used to shape this buffer, needed to
 *        read glyph names and extents. If `NULL`, an empty font will be used.
 * @format: the #hb_buffer_serialize_format_t to use for formatting the output.
 * @flags: the #hb_buffer_serialize_flags_t that control what glyph properties
 *         to serialize.
 *
 * Serializes @buffer into a textual representation of its glyph content,
 * useful for showing the contents of the buffer, for example during debugging.
 * There are currently two supported serialization formats:
 *
 * ## text
 * A human-readable, plain text format.
 * The serialized glyphs will look something like:
 *
 * |[<!-- language="plain" -->
 * [uni0651=0@518,0+0|uni0628=0+1897]
 * ]|
 *
 * - The serialized glyphs are delimited with `[` and `]`.
 * - Glyphs are separated with `|`
 * - Each glyph starts with glyph name, or glyph index if
 *   #HB_BUFFER_SERIALIZE_FLAG_NO_GLYPH_NAMES flag is set. Then,
 *   - If #HB_BUFFER_SERIALIZE_FLAG_NO_CLUSTERS is not set, `=` then #hb_glyph_info_t.cluster.
 *   - If #HB_BUFFER_SERIALIZE_FLAG_NO_POSITIONS is not set, the #hb_glyph_position_t in the format:
 *     - If #hb_glyph_position_t.x_offset and #hb_glyph_position_t.y_offset are not both 0, `@x_offset,y_offset`. Then,
 *     - `+x_advance`, then `,y_advance` if #hb_glyph_position_t.y_advance is not 0. Then,
 *   - If #HB_BUFFER_SERIALIZE_FLAG_GLYPH_EXTENTS is set, the #hb_glyph_extents_t in the format `<x_bearing,y_bearing,width,height>`
 *
 * ## json
 * A machine-readable, structured format.
 * The serialized glyphs will look something like:
 *
 * |[<!-- language="plain" -->
 * [{"g":"uni0651","cl":0,"dx":518,"dy":0,"ax":0,"ay":0},
 *  {"g":"uni0628","cl":0,"dx":0,"dy":0,"ax":1897,"ay":0}]
 * ]|
 *
 * Each glyph is a JSON object, with the following properties:
 * - `g`: the glyph name or glyph index if
 *   #HB_BUFFER_SERIALIZE_FLAG_NO_GLYPH_NAMES flag is set.
 * - `cl`: #hb_glyph_info_t.cluster if
 *   #HB_BUFFER_SERIALIZE_FLAG_NO_CLUSTERS is not set.
 * - `dx`,`dy`,`ax`,`ay`: #hb_glyph_position_t.x_offset, #hb_glyph_position_t.y_offset,
 *    #hb_glyph_position_t.x_advance and #hb_glyph_position_t.y_advance
 *    respectively, if #HB_BUFFER_SERIALIZE_FLAG_NO_POSITIONS is not set.
 * - `xb`,`yb`,`w`,`h`: #hb_glyph_extents_t.x_bearing, #hb_glyph_extents_t.y_bearing,
 *    #hb_glyph_extents_t.width and #hb_glyph_extents_t.height respectively if
 *    #HB_BUFFER_SERIALIZE_FLAG_GLYPH_EXTENTS is set.
 *
 * Return value:
 * The number of serialized items.
 *
 * Since: 0.9.7
 **/
بینښه صحيح
hb_بفر_serialize_glyphs (hb_بفر_t *بفر,
                            بینښه صحيح پیل,
                            بینښه صحيح پای_ټکی,
                            حرف *buf,
                            بینښه صحيح buf_اندازه,
                            بینښه صحيح *buf_consumed,
                            hb_font_t *font,
                            hb_بفر_serialize_format_t format,
                            hb_بفر_serialize_flags_t flags)
{
  پای_ټکی = hb_clamp (پای_ټکی, پیل, بفر->len);
  پیل = hb_min (پیل, پای_ټکی);

  بینښه صحيح sconsumed;
  إذا (!buf_consumed)
    buf_consumed = &sconsumed;
  *buf_consumed = 0;
  إذا (buf_اندازه)
    *buf = '\0';

  بفر->assert_glyphs ();

  إذا (!بفر->have_positions)
    flags |= HB_بفر_SERIALIZE_FLAG_NO_POSITIONS;

  إذا (unlikely (پیل == پای_ټکی))
    أرجع 0;

  إذا (!font)
    font = hb_font_get_تشه ();

  بدل (format)
  {
    حالة HB_بفر_SERIALIZE_FORMAT_متن:
      أرجع hb_بفر_serialize_glyphs_متن (بفر, پیل, پای_ټکی,
                 buf, buf_اندازه, buf_consumed,
                 font, flags);

    حالة HB_بفر_SERIALIZE_FORMAT_JSON:
      أرجع hb_بفر_serialize_glyphs_json (بفر, پیل, پای_ټکی,
                 buf, buf_اندازه, buf_consumed,
                 font, flags);

    افتراضي:
    حالة HB_بفر_SERIALIZE_FORMAT_INVALID:
      أرجع 0;

  }
}

/**
 * hb_buffer_serialize_unicode:
 * @buffer: an #hb_buffer_t buffer.
 * @start: the first item in @buffer to serialize.
 * @end: the last item in @buffer to serialize.
 * @buf: (out) (array length=buf_size) (element-type uint8_t): output string to
 *       write serialized buffer into.
 * @buf_size: the size of @buf.
 * @buf_consumed: (out) (optional): if not `NULL`, will be set to the number of bytes written into @buf.
 * @format: the #hb_buffer_serialize_format_t to use for formatting the output.
 * @flags: the #hb_buffer_serialize_flags_t that control what glyph properties
 *         to serialize.
 *
 * Serializes @buffer into a textual representation of its content,
 * when the buffer contains Unicode codepoints (i.e., before shaping). This is
 * useful for showing the contents of the buffer, for example during debugging.
 * There are currently two supported serialization formats:
 *
 * ## text
 * A human-readable, plain text format.
 * The serialized codepoints will look something like:
 *
 * |[<!-- language="plain" -->
 *  <U+0651=0|U+0628=1>
 * ]|
 *
 * - Glyphs are separated with `|`
 * - Unicode codepoints are expressed as zero-padded four (or more)
 *   digit hexadecimal numbers preceded by `U+`
 * - If #HB_BUFFER_SERIALIZE_FLAG_NO_CLUSTERS is not set, the cluster
 *   will be indicated with a `=` then #hb_glyph_info_t.cluster.
 *
 * ## json
 * A machine-readable, structured format.
 * The serialized codepoints will be a list of objects with the following
 * properties:
 * - `u`: the Unicode codepoint as a decimal integer
 * - `cl`: #hb_glyph_info_t.cluster if
 *   #HB_BUFFER_SERIALIZE_FLAG_NO_CLUSTERS is not set.
 *
 * For example:
 *
 * |[<!-- language="plain" -->
 * [{u:1617,cl:0},{u:1576,cl:1}]
 * ]|
 *
 * Return value:
 * The number of serialized items.
 *
 * Since: 2.7.3
 **/
بینښه صحيح
hb_بفر_serialize_unicode (hb_بفر_t *بفر,
                             بینښه صحيح پیل,
                             بینښه صحيح پای_ټکی,
                             حرف *buf,
                             بینښه صحيح buf_اندازه,
                             بینښه صحيح *buf_consumed,
                             hb_بفر_serialize_format_t format,
                             hb_بفر_serialize_flags_t flags)
{
  پای_ټکی = hb_clamp (پای_ټکی, پیل, بفر->len);
  پیل = hb_min (پیل, پای_ټکی);

  بینښه صحيح sconsumed;
  إذا (!buf_consumed)
    buf_consumed = &sconsumed;
  *buf_consumed = 0;
  إذا (buf_اندازه)
    *buf = '\0';

  بفر->assert_unicode ();

  إذا (unlikely (پیل == پای_ټکی))
    أرجع 0;

  بدل (format)
  {
    حالة HB_بفر_SERIALIZE_FORMAT_متن:
      أرجع hb_بفر_serialize_unicode_متن (بفر, پیل, پای_ټکی,
                                                buf, buf_اندازه, buf_consumed, flags);

    حالة HB_بفر_SERIALIZE_FORMAT_JSON:
      أرجع hb_بفر_serialize_unicode_json (بفر, پیل, پای_ټکی,
                                                buf, buf_اندازه, buf_consumed, flags);

    افتراضي:
    حالة HB_بفر_SERIALIZE_FORMAT_INVALID:
      أرجع 0;

  }
}

ساكن بینښه صحيح
hb_بفر_serialize_invalid (hb_بفر_t *بفر,
                              بینښه صحيح پیل,
                              بینښه صحيح پای_ټکی,
                              حرف *buf,
                              بینښه صحيح buf_اندازه,
                              بینښه صحيح *buf_consumed,
                              hb_بفر_serialize_format_t format,
                              hb_بفر_serialize_flags_t flags)
{
  أكد (!بفر->len);

  بینښه صحيح sconsumed;
  إذا (!buf_consumed)
    buf_consumed = &sconsumed;
  إذا (buf_اندازه < 3)
    أرجع 0;
  إذا (format == HB_بفر_SERIALIZE_FORMAT_JSON) {
    *buf++ = '[';
    *buf++ = ']';
    *buf = '\0';
  } وإلا إذا (format == HB_بفر_SERIALIZE_FORMAT_متن) {
    *buf++ = '!';
    *buf++ = '!';
    *buf = '\0';
  }
  *buf_consumed = 2;
  أرجع 0;
}

/**
 * hb_buffer_serialize:
 * @buffer: an #hb_buffer_t buffer.
 * @start: the first item in @buffer to serialize.
 * @end: the last item in @buffer to serialize.
 * @buf: (out) (array length=buf_size) (element-type uint8_t): output string to
 *       write serialized buffer into.
 * @buf_size: the size of @buf.
 * @buf_consumed: (out) (optional): if not `NULL`, will be set to the number of bytes written into @buf.
 * @font: (nullable): the #hb_font_t used to shape this buffer, needed to
 *        read glyph names and extents. If `NULL`, an empty font will be used.
 * @format: the #hb_buffer_serialize_format_t to use for formatting the output.
 * @flags: the #hb_buffer_serialize_flags_t that control what glyph properties
 *         to serialize.
 *
 * Serializes @buffer into a textual representation of its content, whether
 * Unicode codepoints or glyph identifiers and positioning information. This is
 * useful for showing the contents of the buffer, for example during debugging.
 * See the documentation of hb_buffer_serialize_unicode() and
 * hb_buffer_serialize_glyphs() for a description of the output format.
 *
 * Return value:
 * The number of serialized items.
 *
 * Since: 2.7.3
 **/
بینښه صحيح
hb_بفر_serialize (hb_بفر_t *بفر,
                     بینښه صحيح پیل,
                     بینښه صحيح پای_ټکی,
                     حرف *buf,
                     بینښه صحيح buf_اندازه,
                     بینښه صحيح *buf_consumed,
                     hb_font_t *font,
                     hb_بفر_serialize_format_t format,
                     hb_بفر_serialize_flags_t flags)
{
  بدل (بفر->content_type)
  {

    حالة HB_بفر_CONTENT_TYPE_GLYPHS:
      أرجع hb_بفر_serialize_glyphs (بفر, پیل, پای_ټکی, buf, buf_اندازه,
					 buf_consumed, font, format, flags);

    حالة HB_بفر_CONTENT_TYPE_UNICODE:
      أرجع hb_بفر_serialize_unicode (بفر, پیل, پای_ټکی, buf, buf_اندازه,
					  buf_consumed, format, flags);

    حالة HB_بفر_CONTENT_TYPE_INVALID:
    افتراضي:
      أرجع hb_بفر_serialize_invalid (بفر, پیل, پای_ټکی, buf, buf_اندازه,
					   buf_consumed, format, flags);
  }
}

ساكن منطقي
parse_int (ثابت حرف *pp, ثابت حرف *پای_ټکی, int32_t *pv)
{
  صحيح v;
  ثابت حرف *p = pp;
  إذا (unlikely (!hb_parse_int (&p, پای_ټکی, &v, ریښتیا/* whole buffer */)))
    أرجع خطأ;

  *pv = v;
  أرجع ریښتیا;
}

ساكن منطقي
parse_uint (ثابت حرف *pp, ثابت حرف *پای_ټکی, uint32_t *pv)
{
  بینښه صحيح v;
  ثابت حرف *p = pp;
  إذا (unlikely (!hb_parse_uint (&p, پای_ټکی, &v, ریښتیا/* whole buffer */)))
    أرجع خطأ;

  *pv = v;
  أرجع ریښتیا;
}

ساكن منطقي
parse_hex (ثابت حرف *pp, ثابت حرف *پای_ټکی, uint32_t *pv)
{
  بینښه صحيح v;
  ثابت حرف *p = pp;
  إذا (unlikely (!hb_parse_uint (&p, پای_ټکی, &v, ریښتیا/* whole buffer */, 16)))
    أرجع خطأ;

  *pv = v;
  أرجع ریښتیا;
}

#ضمن "hb-buffer-deserialize-json.hh"
#ضمن "hb-buffer-deserialize-text-glyphs.hh"
#ضمن "hb-buffer-deserialize-text-unicode.hh"

/**
 * hb_buffer_deserialize_glyphs:
 * @buffer: an #hb_buffer_t buffer.
 * @buf: (array length=buf_len): string to deserialize
 * @buf_len: the size of @buf, or -1 if it is `NULL`-terminated
 * @end_ptr: (out) (optional): output pointer to the character after last
 *                               consumed one.
 * @font: (nullable): font for getting glyph IDs
 * @format: the #hb_buffer_serialize_format_t of the input @buf
 *
 * Deserializes glyphs @buffer from textual representation in the format
 * produced by hb_buffer_serialize_glyphs().
 *
 * Return value: `true` if the full string was parsed, `false` otherwise.
 *
 * Since: 0.9.7
 **/
hb_bool_t
hb_بفر_deserialize_glyphs (hb_بفر_t *بفر,
                              ثابت حرف *buf,
                              صحيح buf_len, /* -1 means nul-terminated */
                              ثابت حرف **end_ptr, /* May be NULL */
                              hb_font_t *font, /* May be NULL */
                              hb_بفر_serialize_format_t format)
{
  ثابت حرف *پای_ټکی;
  إذا (!end_ptr)
    end_ptr = &پای_ټکی;
  *end_ptr = buf;

  بفر->assert_glyphs ();

  إذا (unlikely (hb_object_is_immutable (بفر)))
  {
    إذا (end_ptr)
      *end_ptr = buf;
    أرجع خطأ;
  }

  إذا (buf_len == -1)
    buf_len = مزي_اوږدوالی (buf);

  إذا (!buf_len)
  {
    *end_ptr = buf;
    أرجع خطأ;
  }

  إذا (!font)
    font = hb_font_get_تشه ();

  بدل (format)
  {
    حالة HB_بفر_SERIALIZE_FORMAT_متن:
      hb_بفر_سېټ_content_type (بفر, HB_بفر_CONTENT_TYPE_GLYPHS);
      أرجع hb_بفر_deserialize_متن_glyphs (بفر,
						 buf, buf_len, end_ptr,
						 font);

    حالة HB_بفر_SERIALIZE_FORMAT_JSON:
      hb_بفر_سېټ_content_type (بفر, HB_بفر_CONTENT_TYPE_GLYPHS);
      أرجع hb_بفر_deserialize_json (بفر,
                                          buf, buf_len, end_ptr,
                                          font);

    افتراضي:
    حالة HB_بفر_SERIALIZE_FORMAT_INVALID:
      أرجع خطأ;

  }
}


/**
 * hb_buffer_deserialize_unicode:
 * @buffer: an #hb_buffer_t buffer.
 * @buf: (array length=buf_len): string to deserialize
 * @buf_len: the size of @buf, or -1 if it is `NULL`-terminated
 * @end_ptr: (out) (optional): output pointer to the character after last
 *                               consumed one.
 * @format: the #hb_buffer_serialize_format_t of the input @buf
 *
 * Deserializes Unicode @buffer from textual representation in the format
 * produced by hb_buffer_serialize_unicode().
 *
 * Return value: `true` if the full string was parsed, `false` otherwise.
 *
 * Since: 2.7.3
 **/
hb_bool_t
hb_بفر_deserialize_unicode (hb_بفر_t *بفر,
                               ثابت حرف *buf,
                               صحيح buf_len, /* -1 means nul-terminated */
                               ثابت حرف **end_ptr, /* May be NULL */
                               hb_بفر_serialize_format_t format)
{
  ثابت حرف *پای_ټکی;
  إذا (!end_ptr)
    end_ptr = &پای_ټکی;
  *end_ptr = buf;

  بفر->assert_unicode ();

  إذا (unlikely (hb_object_is_immutable (بفر)))
  {
    إذا (end_ptr)
      *end_ptr = buf;
    أرجع خطأ;
  }

  إذا (buf_len == -1)
    buf_len = مزي_اوږدوالی (buf);

  إذا (!buf_len)
  {
    *end_ptr = buf;
    أرجع خطأ;
  }

  hb_font_t* font = hb_font_get_تشه ();

  بدل (format)
  {
    حالة HB_بفر_SERIALIZE_FORMAT_متن:
      hb_بفر_سېټ_content_type (بفر, HB_بفر_CONTENT_TYPE_UNICODE);
      أرجع hb_بفر_deserialize_متن_unicode (بفر,
						  buf, buf_len, end_ptr,
						  font);

    حالة HB_بفر_SERIALIZE_FORMAT_JSON:
      hb_بفر_سېټ_content_type (بفر, HB_بفر_CONTENT_TYPE_UNICODE);
      أرجع hb_بفر_deserialize_json (بفر,
                                          buf, buf_len, end_ptr,
                                          font);

    افتراضي:
    حالة HB_بفر_SERIALIZE_FORMAT_INVALID:
      أرجع خطأ;

  }
}


#د_که_پای
