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

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_بفر_SERIALIZE

#شامل "hb-buffer.hh"


ساکن ثابت حرف *hb_بفر_serialize_formats[] = {
  "text",
  "json",
  خالی_اشاریہ
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
  واپس hb_بفر_serialize_formats;
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
hb_بفر_serialize_format_from_string (ثابت حرف *str, صحیح len)
{
  /* Upper-case it. */
  واپس (hb_بفر_serialize_format_t) (hb_لیبل_from_string (str, len) & ~0x20202020u);
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
hb_بفر_serialize_format_مقصد_string (hb_بفر_serialize_format_t format)
{
  انتخاب ((غیرعلامتی) format)
  {
    حالت HB_بفر_SERIALIZE_FORMAT_متن: واپس hb_بفر_serialize_formats[0];
    حالت HB_بفر_SERIALIZE_FORMAT_JSON: واپس hb_بفر_serialize_formats[1];
    طےشدہ:
    حالت HB_بفر_SERIALIZE_FORMAT_INVALID:  واپس خالی_اشاریہ;
  }
}

ساکن غیرعلامتی صحیح
hb_بفر_serialize_glyphs_json (hb_بفر_t *بفر,
                                  غیرعلامتی صحیح آغاز,
                                  غیرعلامتی صحیح انتہا,
                                  حرف *buf,
                                  غیرعلامتی صحیح buf_سائز,
                                  غیرعلامتی صحیح *buf_consumed,
                                  hb_font_t *font,
                                  hb_بفر_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_بفر_get_glyph_infos (بفر, خالی_اشاریہ);
  hb_glyph_position_t *pos = (flags & HB_بفر_SERIALIZE_FLAG_NO_POSITIONS) ?
                             خالی_اشاریہ : hb_بفر_get_glyph_positions (بفر, خالی_اشاریہ);

  *buf_consumed = 0;
  hb_position_t x = 0, y = 0;

  /* Calculate the advance of the previous glyphs */
  اگر (pos && (flags & HB_بفر_SERIALIZE_FLAG_NO_ADVANCES))
  {
    برائے (غیرعلامتی صحیح i = 0; i < آغاز; i++)
    {
      x = hb_saturate_جوڑو (x, pos[i].x_advance);
      y = hb_saturate_جوڑو (y, pos[i].y_advance);
    }
  }

  برائے (غیرعلامتی صحیح i = آغاز; i < انتہا; i++)
  {
    حرف b[1024];
    حرف *p = b;

    /* In the following code, we know b is large enough that no overflow can happen. */

#تعریف_کرو APPEND(s) HB_STMT_آغاز { strcpy (p, s); p += لڑی_لمبائی (s); } HB_STMT_انتہا

    اگر (i)
      *p++ = ',';
    ورنہ
      *p++ = '[';

    *p++ = '{';

    APPEND ("\"g\":");
    اگر (!(flags & HB_بفر_SERIALIZE_FLAG_NO_GLYPH_نام))
    {
      حرف g[128];
      hb_font_glyph_مقصد_string (font, info[i].codepoint, g, حجم (g));
      *p++ = '"';
      برائے (حرف *q = g; *q; q++)
      {
	اگر (unlikely (*q == '"' || *q == '\\'))
	  *p++ = '\\';
	*p++ = *q;
      }
      *p++ = '"';
    }
    ورنہ
      p += hb_max (0, snprintf (p, ARRAY_لمبائی (b) - (p - b), "%u", info[i].codepoint));

    اگر (!(flags & HB_بفر_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_لمبائی (b) - (p - b), ",\"cl\":%u", info[i].cluster));
    }

    اگر (pos && !(flags & HB_بفر_SERIALIZE_FLAG_NO_POSITIONS))
    {
      hb_position_t dx = hb_saturate_جوڑو (x, pos[i].x_انحراف_2);
      hb_position_t dy = hb_saturate_جوڑو (y, pos[i].y_انحراف_2);
      p += hb_max (0, snprintf (p, ARRAY_لمبائی (b) - (p - b), ",\"dx\":%d,\"dy\":%d",
		   dx, dy));
      اگر (!(flags & HB_بفر_SERIALIZE_FLAG_NO_ADVANCES))
        p += hb_max (0, snprintf (p, ARRAY_لمبائی (b) - (p - b), ",\"ax\":%d,\"ay\":%d",
		     pos[i].x_advance, pos[i].y_advance));
    }

    اگر (flags & HB_بفر_SERIALIZE_FLAG_GLYPH_FLAGS)
    {
      اگر (info[i].نقاب & HB_GLYPH_FLAG_DEFINED)
        p += hb_max (0, snprintf (p, ARRAY_لمبائی (b) - (p - b), ",\"fl\":%u", info[i].نقاب & HB_GLYPH_FLAG_DEFINED));
    }

    اگر (flags & HB_بفر_SERIALIZE_FLAG_GLYPH_EXTENTS)
    {
      hb_glyph_extents_t extents;
      اگر (hb_font_get_glyph_extents(font, info[i].codepoint, &extents))
      {
	p += hb_max (0, snprintf (p, ARRAY_لمبائی (b) - (p - b), ",\"xb\":%d,\"yb\":%d",
				  extents.x_bearing, extents.y_bearing));
	p += hb_max (0, snprintf (p, ARRAY_لمبائی (b) - (p - b), ",\"w\":%d,\"h\":%d",
				  extents.چوڑائی, extents.اونچائی));
      }
    }

    *p++ = '}';
    اگر (i == انتہا-1)
      *p++ = ']';

    غیرعلامتی صحیح l = p - b;
    اگر (buf_سائز > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_سائز -= l;
      *buf_consumed += l;
      *buf = '\0';
    } ورنہ
      واپس i - آغاز;

    اگر (pos && (flags & HB_بفر_SERIALIZE_FLAG_NO_ADVANCES))
    {
      x = hb_saturate_جوڑو (x, pos[i].x_advance);
      y = hb_saturate_جوڑو (y, pos[i].y_advance);
    }
  }

  واپس انتہا - آغاز;
}

ساکن غیرعلامتی صحیح
hb_بفر_serialize_unicode_json (hb_بفر_t *بفر,
          غیرعلامتی صحیح آغاز,
          غیرعلامتی صحیح انتہا,
          حرف *buf,
          غیرعلامتی صحیح buf_سائز,
          غیرعلامتی صحیح *buf_consumed,
          hb_بفر_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_بفر_get_glyph_infos (بفر, خالی_اشاریہ);

  *buf_consumed = 0;
  برائے (غیرعلامتی صحیح i = آغاز; i < انتہا; i++)
  {
    حرف b[1024];
    حرف *p = b;

    اگر (i)
      *p++ = ',';
    ورنہ
      *p++ = '[';

    *p++ = '{';

    APPEND ("\"u\":");

    p += hb_max (0, snprintf (p, ARRAY_لمبائی (b) - (p - b), "%u", info[i].codepoint));

    اگر (!(flags & HB_بفر_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_لمبائی (b) - (p - b), ",\"cl\":%u", info[i].cluster));
    }

    *p++ = '}';

    اگر (i == انتہا-1)
      *p++ = ']';

    غیرعلامتی صحیح l = p - b;
    اگر (buf_سائز > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_سائز -= l;
      *buf_consumed += l;
      *buf = '\0';
    } ورنہ
      واپس i - آغاز;

  }

  واپس انتہا - آغاز;
}

ساکن غیرعلامتی صحیح
hb_بفر_serialize_glyphs_متن (hb_بفر_t *بفر,
                                  غیرعلامتی صحیح آغاز,
                                  غیرعلامتی صحیح انتہا,
                                  حرف *buf,
                                  غیرعلامتی صحیح buf_سائز,
                                  غیرعلامتی صحیح *buf_consumed,
                                  hb_font_t *font,
                                  hb_بفر_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_بفر_get_glyph_infos (بفر, خالی_اشاریہ);
  hb_glyph_position_t *pos = (flags & HB_بفر_SERIALIZE_FLAG_NO_POSITIONS) ?
           خالی_اشاریہ : hb_بفر_get_glyph_positions (بفر, خالی_اشاریہ);

  *buf_consumed = 0;
  hb_position_t x = 0, y = 0;

  /* Calculate the advance of the previous glyphs */
  اگر (pos && (flags & HB_بفر_SERIALIZE_FLAG_NO_ADVANCES))
  {
    برائے (غیرعلامتی صحیح i = 0; i < آغاز; i++)
    {
      x = hb_saturate_جوڑو (x, pos[i].x_advance);
      y = hb_saturate_جوڑو (y, pos[i].y_advance);
    }
  }

  برائے (غیرعلامتی صحیح i = آغاز; i < انتہا; i++)
  {
    حرف b[1024];
    حرف *p = b;

    /* In the following code, we know b is large enough that no overflow can happen. */

    اگر (i)
      *p++ = '|';
    ورنہ
      *p++ = '[';

    اگر (!(flags & HB_بفر_SERIALIZE_FLAG_NO_GLYPH_نام))
    {
      /* TODO Escape delimiters we use. */
      hb_font_glyph_مقصد_string (font, info[i].codepoint, p, 128);
      p += لڑی_لمبائی (p);
    }
    ورنہ
      p += hb_max (0, snprintf (p, ARRAY_لمبائی (b) - (p - b), "%u", info[i].codepoint));

    اگر (!(flags & HB_بفر_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_لمبائی (b) - (p - b), "=%u", info[i].cluster));
    }

    اگر (pos && !(flags & HB_بفر_SERIALIZE_FLAG_NO_POSITIONS))
    {
      hb_position_t dx = hb_saturate_جوڑو (x, pos[i].x_انحراف_2);
      hb_position_t dy = hb_saturate_جوڑو (y, pos[i].y_انحراف_2);
      اگر (dx || dy)
        p += hb_max (0, snprintf (p, ARRAY_لمبائی (b) - (p - b), "@%d,%d", dx, dy));

      اگر (!(flags & HB_بفر_SERIALIZE_FLAG_NO_ADVANCES))
      {
        *p++ = '+';
        p += hb_max (0, snprintf (p, ARRAY_لمبائی (b) - (p - b), "%d", pos[i].x_advance));
        اگر (pos[i].y_advance)
          p += hb_max (0, snprintf (p, ARRAY_لمبائی (b) - (p - b), ",%d", pos[i].y_advance));
      }
    }

    اگر (flags & HB_بفر_SERIALIZE_FLAG_GLYPH_FLAGS)
    {
      اگر (info[i].نقاب & HB_GLYPH_FLAG_DEFINED)
        p += hb_max (0, snprintf (p, ARRAY_لمبائی (b) - (p - b), "#%X", info[i].نقاب &HB_GLYPH_FLAG_DEFINED));
    }

    اگر (flags & HB_بفر_SERIALIZE_FLAG_GLYPH_EXTENTS)
    {
      hb_glyph_extents_t extents;
      اگر (hb_font_get_glyph_extents(font, info[i].codepoint, &extents))
	p += hb_max (0, snprintf (p, ARRAY_لمبائی (b) - (p - b), "<%d,%d,%d,%d>", extents.x_bearing, extents.y_bearing, extents.چوڑائی, extents.اونچائی));
    }

    اگر (i == انتہا-1) {
      *p++ = ']';
    }

    غیرعلامتی صحیح l = p - b;
    اگر (buf_سائز > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_سائز -= l;
      *buf_consumed += l;
      *buf = '\0';
    } ورنہ
      واپس i - آغاز;

    اگر (pos && (flags & HB_بفر_SERIALIZE_FLAG_NO_ADVANCES))
    {
      x = hb_saturate_جوڑو (x, pos[i].x_advance);
      y = hb_saturate_جوڑو (y, pos[i].y_advance);
    }
  }

  واپس انتہا - آغاز;
}


ساکن غیرعلامتی صحیح
hb_بفر_serialize_unicode_متن (hb_بفر_t *بفر,
                                   غیرعلامتی صحیح آغاز,
                                   غیرعلامتی صحیح انتہا,
                                   حرف *buf,
                                   غیرعلامتی صحیح buf_سائز,
                                   غیرعلامتی صحیح *buf_consumed,
                                   hb_بفر_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_بفر_get_glyph_infos (بفر, خالی_اشاریہ);
  *buf_consumed = 0;
  برائے (غیرعلامتی صحیح i = آغاز; i < انتہا; i++)
  {
    حرف b[1024];
    حرف *p = b;

    اگر (i)
      *p++ = '|';
    ورنہ
      *p++ = '<';

    p += hb_max (0, snprintf (p, ARRAY_لمبائی (b) - (p - b), "U+%04X", info[i].codepoint));

    اگر (!(flags & HB_بفر_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_لمبائی (b) - (p - b), "=%u", info[i].cluster));
    }

    اگر (i == انتہا-1)
      *p++ = '>';

    غیرعلامتی صحیح l = p - b;
    اگر (buf_سائز > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_سائز -= l;
      *buf_consumed += l;
      *buf = '\0';
    } ورنہ
      واپس i - آغاز;
  }
  واپس انتہا - آغاز;
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
غیرعلامتی صحیح
hb_بفر_serialize_glyphs (hb_بفر_t *بفر,
                            غیرعلامتی صحیح آغاز,
                            غیرعلامتی صحیح انتہا,
                            حرف *buf,
                            غیرعلامتی صحیح buf_سائز,
                            غیرعلامتی صحیح *buf_consumed,
                            hb_font_t *font,
                            hb_بفر_serialize_format_t format,
                            hb_بفر_serialize_flags_t flags)
{
  انتہا = hb_clamp (انتہا, آغاز, بفر->len);
  آغاز = hb_min (آغاز, انتہا);

  غیرعلامتی صحیح sconsumed;
  اگر (!buf_consumed)
    buf_consumed = &sconsumed;
  *buf_consumed = 0;
  اگر (buf_سائز)
    *buf = '\0';

  بفر->assert_glyphs ();

  اگر (!بفر->have_positions)
    flags |= HB_بفر_SERIALIZE_FLAG_NO_POSITIONS;

  اگر (unlikely (آغاز == انتہا))
    واپس 0;

  اگر (!font)
    font = hb_font_get_خالی_خانے ();

  انتخاب (format)
  {
    حالت HB_بفر_SERIALIZE_FORMAT_متن:
      واپس hb_بفر_serialize_glyphs_متن (بفر, آغاز, انتہا,
                 buf, buf_سائز, buf_consumed,
                 font, flags);

    حالت HB_بفر_SERIALIZE_FORMAT_JSON:
      واپس hb_بفر_serialize_glyphs_json (بفر, آغاز, انتہا,
                 buf, buf_سائز, buf_consumed,
                 font, flags);

    طےشدہ:
    حالت HB_بفر_SERIALIZE_FORMAT_INVALID:
      واپس 0;

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
غیرعلامتی صحیح
hb_بفر_serialize_unicode (hb_بفر_t *بفر,
                             غیرعلامتی صحیح آغاز,
                             غیرعلامتی صحیح انتہا,
                             حرف *buf,
                             غیرعلامتی صحیح buf_سائز,
                             غیرعلامتی صحیح *buf_consumed,
                             hb_بفر_serialize_format_t format,
                             hb_بفر_serialize_flags_t flags)
{
  انتہا = hb_clamp (انتہا, آغاز, بفر->len);
  آغاز = hb_min (آغاز, انتہا);

  غیرعلامتی صحیح sconsumed;
  اگر (!buf_consumed)
    buf_consumed = &sconsumed;
  *buf_consumed = 0;
  اگر (buf_سائز)
    *buf = '\0';

  بفر->assert_unicode ();

  اگر (unlikely (آغاز == انتہا))
    واپس 0;

  انتخاب (format)
  {
    حالت HB_بفر_SERIALIZE_FORMAT_متن:
      واپس hb_بفر_serialize_unicode_متن (بفر, آغاز, انتہا,
                                                buf, buf_سائز, buf_consumed, flags);

    حالت HB_بفر_SERIALIZE_FORMAT_JSON:
      واپس hb_بفر_serialize_unicode_json (بفر, آغاز, انتہا,
                                                buf, buf_سائز, buf_consumed, flags);

    طےشدہ:
    حالت HB_بفر_SERIALIZE_FORMAT_INVALID:
      واپس 0;

  }
}

ساکن غیرعلامتی صحیح
hb_بفر_serialize_invalid (hb_بفر_t *بفر,
                              غیرعلامتی صحیح آغاز,
                              غیرعلامتی صحیح انتہا,
                              حرف *buf,
                              غیرعلامتی صحیح buf_سائز,
                              غیرعلامتی صحیح *buf_consumed,
                              hb_بفر_serialize_format_t format,
                              hb_بفر_serialize_flags_t flags)
{
  أكد (!بفر->len);

  غیرعلامتی صحیح sconsumed;
  اگر (!buf_consumed)
    buf_consumed = &sconsumed;
  اگر (buf_سائز < 3)
    واپس 0;
  اگر (format == HB_بفر_SERIALIZE_FORMAT_JSON) {
    *buf++ = '[';
    *buf++ = ']';
    *buf = '\0';
  } ورنہ اگر (format == HB_بفر_SERIALIZE_FORMAT_متن) {
    *buf++ = '!';
    *buf++ = '!';
    *buf = '\0';
  }
  *buf_consumed = 2;
  واپس 0;
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
غیرعلامتی صحیح
hb_بفر_serialize (hb_بفر_t *بفر,
                     غیرعلامتی صحیح آغاز,
                     غیرعلامتی صحیح انتہا,
                     حرف *buf,
                     غیرعلامتی صحیح buf_سائز,
                     غیرعلامتی صحیح *buf_consumed,
                     hb_font_t *font,
                     hb_بفر_serialize_format_t format,
                     hb_بفر_serialize_flags_t flags)
{
  انتخاب (بفر->content_type)
  {

    حالت HB_بفر_CONTENT_TYPE_GLYPHS:
      واپس hb_بفر_serialize_glyphs (بفر, آغاز, انتہا, buf, buf_سائز,
					 buf_consumed, font, format, flags);

    حالت HB_بفر_CONTENT_TYPE_UNICODE:
      واپس hb_بفر_serialize_unicode (بفر, آغاز, انتہا, buf, buf_سائز,
					  buf_consumed, format, flags);

    حالت HB_بفر_CONTENT_TYPE_INVALID:
    طےشدہ:
      واپس hb_بفر_serialize_invalid (بفر, آغاز, انتہا, buf, buf_سائز,
					   buf_consumed, format, flags);
  }
}

ساکن منطقی
parse_int (ثابت حرف *pp, ثابت حرف *انتہا, int32_t *pv)
{
  صحیح v;
  ثابت حرف *p = pp;
  اگر (unlikely (!hb_parse_int (&p, انتہا, &v, سچ/* whole buffer */)))
    واپس جھوٹ;

  *pv = v;
  واپس سچ;
}

ساکن منطقی
parse_uint (ثابت حرف *pp, ثابت حرف *انتہا, uint32_t *pv)
{
  غیرعلامتی صحیح v;
  ثابت حرف *p = pp;
  اگر (unlikely (!hb_parse_uint (&p, انتہا, &v, سچ/* whole buffer */)))
    واپس جھوٹ;

  *pv = v;
  واپس سچ;
}

ساکن منطقی
parse_hex (ثابت حرف *pp, ثابت حرف *انتہا, uint32_t *pv)
{
  غیرعلامتی صحیح v;
  ثابت حرف *p = pp;
  اگر (unlikely (!hb_parse_uint (&p, انتہا, &v, سچ/* whole buffer */, 16)))
    واپس جھوٹ;

  *pv = v;
  واپس سچ;
}

#شامل "hb-buffer-deserialize-json.hh"
#شامل "hb-buffer-deserialize-text-glyphs.hh"
#شامل "hb-buffer-deserialize-text-unicode.hh"

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
                              صحیح buf_len, /* -1 means nul-terminated */
                              ثابت حرف **end_ptr, /* May be NULL */
                              hb_font_t *font, /* May be NULL */
                              hb_بفر_serialize_format_t format)
{
  ثابت حرف *انتہا;
  اگر (!end_ptr)
    end_ptr = &انتہا;
  *end_ptr = buf;

  بفر->assert_glyphs ();

  اگر (unlikely (hb_object_is_immutable (بفر)))
  {
    اگر (end_ptr)
      *end_ptr = buf;
    واپس جھوٹ;
  }

  اگر (buf_len == -1)
    buf_len = لڑی_لمبائی (buf);

  اگر (!buf_len)
  {
    *end_ptr = buf;
    واپس جھوٹ;
  }

  اگر (!font)
    font = hb_font_get_خالی_خانے ();

  انتخاب (format)
  {
    حالت HB_بفر_SERIALIZE_FORMAT_متن:
      hb_بفر_سیٹ_content_type (بفر, HB_بفر_CONTENT_TYPE_GLYPHS);
      واپس hb_بفر_deserialize_متن_glyphs (بفر,
						 buf, buf_len, end_ptr,
						 font);

    حالت HB_بفر_SERIALIZE_FORMAT_JSON:
      hb_بفر_سیٹ_content_type (بفر, HB_بفر_CONTENT_TYPE_GLYPHS);
      واپس hb_بفر_deserialize_json (بفر,
                                          buf, buf_len, end_ptr,
                                          font);

    طےشدہ:
    حالت HB_بفر_SERIALIZE_FORMAT_INVALID:
      واپس جھوٹ;

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
                               صحیح buf_len, /* -1 means nul-terminated */
                               ثابت حرف **end_ptr, /* May be NULL */
                               hb_بفر_serialize_format_t format)
{
  ثابت حرف *انتہا;
  اگر (!end_ptr)
    end_ptr = &انتہا;
  *end_ptr = buf;

  بفر->assert_unicode ();

  اگر (unlikely (hb_object_is_immutable (بفر)))
  {
    اگر (end_ptr)
      *end_ptr = buf;
    واپس جھوٹ;
  }

  اگر (buf_len == -1)
    buf_len = لڑی_لمبائی (buf);

  اگر (!buf_len)
  {
    *end_ptr = buf;
    واپس جھوٹ;
  }

  hb_font_t* font = hb_font_get_خالی_خانے ();

  انتخاب (format)
  {
    حالت HB_بفر_SERIALIZE_FORMAT_متن:
      hb_بفر_سیٹ_content_type (بفر, HB_بفر_CONTENT_TYPE_UNICODE);
      واپس hb_بفر_deserialize_متن_unicode (بفر,
						  buf, buf_len, end_ptr,
						  font);

    حالت HB_بفر_SERIALIZE_FORMAT_JSON:
      hb_بفر_سیٹ_content_type (بفر, HB_بفر_CONTENT_TYPE_UNICODE);
      واپس hb_بفر_deserialize_json (بفر,
                                          buf, buf_len, end_ptr,
                                          font);

    طےشدہ:
    حالت HB_بفر_SERIALIZE_FORMAT_INVALID:
      واپس جھوٹ;

  }
}


#اگر_ختم
