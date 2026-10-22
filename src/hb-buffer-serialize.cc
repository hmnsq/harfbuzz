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

#समावेश "hb.hh"

#अगर_अघोषित HB_NO_बफर_SERIALIZE

#समावेश "hb-buffer.hh"


अटल स्थिर अक्षर *hb_बफर_serialize_formats[] = {
  "text",
  "json",
  nullptr
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
स्थिर अक्षर **
hb_बफर_serialize_list_formats ()
{
  वापस hb_बफर_serialize_formats;
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
hb_बफर_serialize_format_t
hb_बफर_serialize_format_from_string (स्थिर अक्षर *str, पूर्णांक len)
{
  /* Upper-case it. */
  वापस (hb_बफर_serialize_format_t) (hb_tag_from_string (str, len) & ~0x20202020u);
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
स्थिर अक्षर *
hb_बफर_serialize_format_to_string (hb_बफर_serialize_format_t format)
{
  चयन ((अचिन्हित) format)
  {
    हाल HB_बफर_SERIALIZE_FORMAT_TEXT: वापस hb_बफर_serialize_formats[0];
    हाल HB_बफर_SERIALIZE_FORMAT_JSON: वापस hb_बफर_serialize_formats[1];
    शेष:
    हाल HB_बफर_SERIALIZE_FORMAT_INVALID:  वापस nullptr;
  }
}

अटल अचिन्हित पूर्णांक
hb_बफर_serialize_glyphs_json (hb_बफर_t *बफर,
                                  अचिन्हित पूर्णांक start,
                                  अचिन्हित पूर्णांक end,
                                  अक्षर *buf,
                                  अचिन्हित पूर्णांक buf_आकार,
                                  अचिन्हित पूर्णांक *buf_consumed,
                                  hb_font_t *font,
                                  hb_बफर_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_बफर_get_glyph_infos (बफर, nullptr);
  hb_glyph_position_t *pos = (flags & HB_बफर_SERIALIZE_FLAG_NO_POSITIONS) ?
                             nullptr : hb_बफर_get_glyph_positions (बफर, nullptr);

  *buf_consumed = 0;
  hb_position_t x = 0, y = 0;

  /* Calculate the advance of the previous glyphs */
  अगर (pos && (flags & HB_बफर_SERIALIZE_FLAG_NO_ADVANCES))
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < start; i++)
    {
      x = hb_saturate_add (x, pos[i].x_advance);
      y = hb_saturate_add (y, pos[i].y_advance);
    }
  }

  क्रम (अचिन्हित पूर्णांक i = start; i < end; i++)
  {
    अक्षर b[1024];
    अक्षर *p = b;

    /* In the following code, we know b is large enough that no overflow can happen. */

#घोषणा APPEND(s) HB_STMT_START { म_नकल (p, s); p += म_माप (s); } HB_STMT_END

    अगर (i)
      *p++ = ',';
    अन्यथा
      *p++ = '[';

    *p++ = '{';

    APPEND ("\"g\":");
    अगर (!(flags & HB_बफर_SERIALIZE_FLAG_NO_GLYPH_NAMES))
    {
      अक्षर g[128];
      hb_font_glyph_to_string (font, info[i].codepoint, g, माप (g));
      *p++ = '"';
      क्रम (अक्षर *q = g; *q; q++)
      {
	अगर (unlikely (*q == '"' || *q == '\\'))
	  *p++ = '\\';
	*p++ = *q;
      }
      *p++ = '"';
    }
    अन्यथा
      p += hb_max (0, snprintf (p, ARRAY_लंबाई (b) - (p - b), "%u", info[i].codepoint));

    अगर (!(flags & HB_बफर_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_लंबाई (b) - (p - b), ",\"cl\":%u", info[i].cluster));
    }

    अगर (pos && !(flags & HB_बफर_SERIALIZE_FLAG_NO_POSITIONS))
    {
      hb_position_t dx = hb_saturate_add (x, pos[i].x_offset);
      hb_position_t dy = hb_saturate_add (y, pos[i].y_offset);
      p += hb_max (0, snprintf (p, ARRAY_लंबाई (b) - (p - b), ",\"dx\":%d,\"dy\":%d",
		   dx, dy));
      अगर (!(flags & HB_बफर_SERIALIZE_FLAG_NO_ADVANCES))
        p += hb_max (0, snprintf (p, ARRAY_लंबाई (b) - (p - b), ",\"ax\":%d,\"ay\":%d",
		     pos[i].x_advance, pos[i].y_advance));
    }

    अगर (flags & HB_बफर_SERIALIZE_FLAG_GLYPH_FLAGS)
    {
      अगर (info[i].mask & HB_GLYPH_FLAG_DEFINED)
        p += hb_max (0, snprintf (p, ARRAY_लंबाई (b) - (p - b), ",\"fl\":%u", info[i].mask & HB_GLYPH_FLAG_DEFINED));
    }

    अगर (flags & HB_बफर_SERIALIZE_FLAG_GLYPH_EXTENTS)
    {
      hb_glyph_extents_t extents;
      अगर (hb_font_get_glyph_extents(font, info[i].codepoint, &extents))
      {
	p += hb_max (0, snprintf (p, ARRAY_लंबाई (b) - (p - b), ",\"xb\":%d,\"yb\":%d",
				  extents.x_bearing, extents.y_bearing));
	p += hb_max (0, snprintf (p, ARRAY_लंबाई (b) - (p - b), ",\"w\":%d,\"h\":%d",
				  extents.width, extents.height));
      }
    }

    *p++ = '}';
    अगर (i == end-1)
      *p++ = ']';

    अचिन्हित पूर्णांक l = p - b;
    अगर (buf_आकार > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_आकार -= l;
      *buf_consumed += l;
      *buf = '\0';
    } अन्यथा
      वापस i - start;

    अगर (pos && (flags & HB_बफर_SERIALIZE_FLAG_NO_ADVANCES))
    {
      x = hb_saturate_add (x, pos[i].x_advance);
      y = hb_saturate_add (y, pos[i].y_advance);
    }
  }

  वापस end - start;
}

अटल अचिन्हित पूर्णांक
hb_बफर_serialize_unicode_json (hb_बफर_t *बफर,
          अचिन्हित पूर्णांक start,
          अचिन्हित पूर्णांक end,
          अक्षर *buf,
          अचिन्हित पूर्णांक buf_आकार,
          अचिन्हित पूर्णांक *buf_consumed,
          hb_बफर_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_बफर_get_glyph_infos (बफर, nullptr);

  *buf_consumed = 0;
  क्रम (अचिन्हित पूर्णांक i = start; i < end; i++)
  {
    अक्षर b[1024];
    अक्षर *p = b;

    अगर (i)
      *p++ = ',';
    अन्यथा
      *p++ = '[';

    *p++ = '{';

    APPEND ("\"u\":");

    p += hb_max (0, snprintf (p, ARRAY_लंबाई (b) - (p - b), "%u", info[i].codepoint));

    अगर (!(flags & HB_बफर_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_लंबाई (b) - (p - b), ",\"cl\":%u", info[i].cluster));
    }

    *p++ = '}';

    अगर (i == end-1)
      *p++ = ']';

    अचिन्हित पूर्णांक l = p - b;
    अगर (buf_आकार > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_आकार -= l;
      *buf_consumed += l;
      *buf = '\0';
    } अन्यथा
      वापस i - start;

  }

  वापस end - start;
}

अटल अचिन्हित पूर्णांक
hb_बफर_serialize_glyphs_text (hb_बफर_t *बफर,
                                  अचिन्हित पूर्णांक start,
                                  अचिन्हित पूर्णांक end,
                                  अक्षर *buf,
                                  अचिन्हित पूर्णांक buf_आकार,
                                  अचिन्हित पूर्णांक *buf_consumed,
                                  hb_font_t *font,
                                  hb_बफर_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_बफर_get_glyph_infos (बफर, nullptr);
  hb_glyph_position_t *pos = (flags & HB_बफर_SERIALIZE_FLAG_NO_POSITIONS) ?
           nullptr : hb_बफर_get_glyph_positions (बफर, nullptr);

  *buf_consumed = 0;
  hb_position_t x = 0, y = 0;

  /* Calculate the advance of the previous glyphs */
  अगर (pos && (flags & HB_बफर_SERIALIZE_FLAG_NO_ADVANCES))
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < start; i++)
    {
      x = hb_saturate_add (x, pos[i].x_advance);
      y = hb_saturate_add (y, pos[i].y_advance);
    }
  }

  क्रम (अचिन्हित पूर्णांक i = start; i < end; i++)
  {
    अक्षर b[1024];
    अक्षर *p = b;

    /* In the following code, we know b is large enough that no overflow can happen. */

    अगर (i)
      *p++ = '|';
    अन्यथा
      *p++ = '[';

    अगर (!(flags & HB_बफर_SERIALIZE_FLAG_NO_GLYPH_NAMES))
    {
      /* TODO Escape delimiters we use. */
      hb_font_glyph_to_string (font, info[i].codepoint, p, 128);
      p += म_माप (p);
    }
    अन्यथा
      p += hb_max (0, snprintf (p, ARRAY_लंबाई (b) - (p - b), "%u", info[i].codepoint));

    अगर (!(flags & HB_बफर_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_लंबाई (b) - (p - b), "=%u", info[i].cluster));
    }

    अगर (pos && !(flags & HB_बफर_SERIALIZE_FLAG_NO_POSITIONS))
    {
      hb_position_t dx = hb_saturate_add (x, pos[i].x_offset);
      hb_position_t dy = hb_saturate_add (y, pos[i].y_offset);
      अगर (dx || dy)
        p += hb_max (0, snprintf (p, ARRAY_लंबाई (b) - (p - b), "@%d,%d", dx, dy));

      अगर (!(flags & HB_बफर_SERIALIZE_FLAG_NO_ADVANCES))
      {
        *p++ = '+';
        p += hb_max (0, snprintf (p, ARRAY_लंबाई (b) - (p - b), "%d", pos[i].x_advance));
        अगर (pos[i].y_advance)
          p += hb_max (0, snprintf (p, ARRAY_लंबाई (b) - (p - b), ",%d", pos[i].y_advance));
      }
    }

    अगर (flags & HB_बफर_SERIALIZE_FLAG_GLYPH_FLAGS)
    {
      अगर (info[i].mask & HB_GLYPH_FLAG_DEFINED)
        p += hb_max (0, snprintf (p, ARRAY_लंबाई (b) - (p - b), "#%X", info[i].mask &HB_GLYPH_FLAG_DEFINED));
    }

    अगर (flags & HB_बफर_SERIALIZE_FLAG_GLYPH_EXTENTS)
    {
      hb_glyph_extents_t extents;
      अगर (hb_font_get_glyph_extents(font, info[i].codepoint, &extents))
	p += hb_max (0, snprintf (p, ARRAY_लंबाई (b) - (p - b), "<%d,%d,%d,%d>", extents.x_bearing, extents.y_bearing, extents.width, extents.height));
    }

    अगर (i == end-1) {
      *p++ = ']';
    }

    अचिन्हित पूर्णांक l = p - b;
    अगर (buf_आकार > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_आकार -= l;
      *buf_consumed += l;
      *buf = '\0';
    } अन्यथा
      वापस i - start;

    अगर (pos && (flags & HB_बफर_SERIALIZE_FLAG_NO_ADVANCES))
    {
      x = hb_saturate_add (x, pos[i].x_advance);
      y = hb_saturate_add (y, pos[i].y_advance);
    }
  }

  वापस end - start;
}


अटल अचिन्हित पूर्णांक
hb_बफर_serialize_unicode_text (hb_बफर_t *बफर,
                                   अचिन्हित पूर्णांक start,
                                   अचिन्हित पूर्णांक end,
                                   अक्षर *buf,
                                   अचिन्हित पूर्णांक buf_आकार,
                                   अचिन्हित पूर्णांक *buf_consumed,
                                   hb_बफर_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_बफर_get_glyph_infos (बफर, nullptr);
  *buf_consumed = 0;
  क्रम (अचिन्हित पूर्णांक i = start; i < end; i++)
  {
    अक्षर b[1024];
    अक्षर *p = b;

    अगर (i)
      *p++ = '|';
    अन्यथा
      *p++ = '<';

    p += hb_max (0, snprintf (p, ARRAY_लंबाई (b) - (p - b), "U+%04X", info[i].codepoint));

    अगर (!(flags & HB_बफर_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_लंबाई (b) - (p - b), "=%u", info[i].cluster));
    }

    अगर (i == end-1)
      *p++ = '>';

    अचिन्हित पूर्णांक l = p - b;
    अगर (buf_आकार > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_आकार -= l;
      *buf_consumed += l;
      *buf = '\0';
    } अन्यथा
      वापस i - start;
  }
  वापस end - start;
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
अचिन्हित पूर्णांक
hb_बफर_serialize_glyphs (hb_बफर_t *बफर,
                            अचिन्हित पूर्णांक start,
                            अचिन्हित पूर्णांक end,
                            अक्षर *buf,
                            अचिन्हित पूर्णांक buf_आकार,
                            अचिन्हित पूर्णांक *buf_consumed,
                            hb_font_t *font,
                            hb_बफर_serialize_format_t format,
                            hb_बफर_serialize_flags_t flags)
{
  end = hb_clamp (end, start, बफर->len);
  start = hb_min (start, end);

  अचिन्हित पूर्णांक sconsumed;
  अगर (!buf_consumed)
    buf_consumed = &sconsumed;
  *buf_consumed = 0;
  अगर (buf_आकार)
    *buf = '\0';

  बफर->assert_glyphs ();

  अगर (!बफर->have_positions)
    flags |= HB_बफर_SERIALIZE_FLAG_NO_POSITIONS;

  अगर (unlikely (start == end))
    वापस 0;

  अगर (!font)
    font = hb_font_get_empty ();

  चयन (format)
  {
    हाल HB_बफर_SERIALIZE_FORMAT_TEXT:
      वापस hb_बफर_serialize_glyphs_text (बफर, start, end,
                 buf, buf_आकार, buf_consumed,
                 font, flags);

    हाल HB_बफर_SERIALIZE_FORMAT_JSON:
      वापस hb_बफर_serialize_glyphs_json (बफर, start, end,
                 buf, buf_आकार, buf_consumed,
                 font, flags);

    शेष:
    हाल HB_बफर_SERIALIZE_FORMAT_INVALID:
      वापस 0;

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
अचिन्हित पूर्णांक
hb_बफर_serialize_unicode (hb_बफर_t *बफर,
                             अचिन्हित पूर्णांक start,
                             अचिन्हित पूर्णांक end,
                             अक्षर *buf,
                             अचिन्हित पूर्णांक buf_आकार,
                             अचिन्हित पूर्णांक *buf_consumed,
                             hb_बफर_serialize_format_t format,
                             hb_बफर_serialize_flags_t flags)
{
  end = hb_clamp (end, start, बफर->len);
  start = hb_min (start, end);

  अचिन्हित पूर्णांक sconsumed;
  अगर (!buf_consumed)
    buf_consumed = &sconsumed;
  *buf_consumed = 0;
  अगर (buf_आकार)
    *buf = '\0';

  बफर->assert_unicode ();

  अगर (unlikely (start == end))
    वापस 0;

  चयन (format)
  {
    हाल HB_बफर_SERIALIZE_FORMAT_TEXT:
      वापस hb_बफर_serialize_unicode_text (बफर, start, end,
                                                buf, buf_आकार, buf_consumed, flags);

    हाल HB_बफर_SERIALIZE_FORMAT_JSON:
      वापस hb_बफर_serialize_unicode_json (बफर, start, end,
                                                buf, buf_आकार, buf_consumed, flags);

    शेष:
    हाल HB_बफर_SERIALIZE_FORMAT_INVALID:
      वापस 0;

  }
}

अटल अचिन्हित पूर्णांक
hb_बफर_serialize_invalid (hb_बफर_t *बफर,
                              अचिन्हित पूर्णांक start,
                              अचिन्हित पूर्णांक end,
                              अक्षर *buf,
                              अचिन्हित पूर्णांक buf_आकार,
                              अचिन्हित पूर्णांक *buf_consumed,
                              hb_बफर_serialize_format_t format,
                              hb_बफर_serialize_flags_t flags)
{
  निश्चित (!बफर->len);

  अचिन्हित पूर्णांक sconsumed;
  अगर (!buf_consumed)
    buf_consumed = &sconsumed;
  अगर (buf_आकार < 3)
    वापस 0;
  अगर (format == HB_बफर_SERIALIZE_FORMAT_JSON) {
    *buf++ = '[';
    *buf++ = ']';
    *buf = '\0';
  } अन्यथा अगर (format == HB_बफर_SERIALIZE_FORMAT_TEXT) {
    *buf++ = '!';
    *buf++ = '!';
    *buf = '\0';
  }
  *buf_consumed = 2;
  वापस 0;
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
अचिन्हित पूर्णांक
hb_बफर_serialize (hb_बफर_t *बफर,
                     अचिन्हित पूर्णांक start,
                     अचिन्हित पूर्णांक end,
                     अक्षर *buf,
                     अचिन्हित पूर्णांक buf_आकार,
                     अचिन्हित पूर्णांक *buf_consumed,
                     hb_font_t *font,
                     hb_बफर_serialize_format_t format,
                     hb_बफर_serialize_flags_t flags)
{
  चयन (बफर->content_type)
  {

    हाल HB_बफर_CONTENT_TYPE_GLYPHS:
      वापस hb_बफर_serialize_glyphs (बफर, start, end, buf, buf_आकार,
					 buf_consumed, font, format, flags);

    हाल HB_बफर_CONTENT_TYPE_UNICODE:
      वापस hb_बफर_serialize_unicode (बफर, start, end, buf, buf_आकार,
					  buf_consumed, format, flags);

    हाल HB_बफर_CONTENT_TYPE_INVALID:
    शेष:
      वापस hb_बफर_serialize_invalid (बफर, start, end, buf, buf_आकार,
					   buf_consumed, format, flags);
  }
}

अटल bool
parse_int (स्थिर अक्षर *pp, स्थिर अक्षर *end, int32_t *pv)
{
  पूर्णांक v;
  स्थिर अक्षर *p = pp;
  अगर (unlikely (!hb_parse_int (&p, end, &v, true/* whole buffer */)))
    वापस false;

  *pv = v;
  वापस true;
}

अटल bool
parse_uint (स्थिर अक्षर *pp, स्थिर अक्षर *end, uint32_t *pv)
{
  अचिन्हित पूर्णांक v;
  स्थिर अक्षर *p = pp;
  अगर (unlikely (!hb_parse_uint (&p, end, &v, true/* whole buffer */)))
    वापस false;

  *pv = v;
  वापस true;
}

अटल bool
parse_hex (स्थिर अक्षर *pp, स्थिर अक्षर *end, uint32_t *pv)
{
  अचिन्हित पूर्णांक v;
  स्थिर अक्षर *p = pp;
  अगर (unlikely (!hb_parse_uint (&p, end, &v, true/* whole buffer */, 16)))
    वापस false;

  *pv = v;
  वापस true;
}

#समावेश "hb-buffer-deserialize-json.hh"
#समावेश "hb-buffer-deserialize-text-glyphs.hh"
#समावेश "hb-buffer-deserialize-text-unicode.hh"

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
hb_बफर_deserialize_glyphs (hb_बफर_t *बफर,
                              स्थिर अक्षर *buf,
                              पूर्णांक buf_len, /* -1 means nul-terminated */
                              स्थिर अक्षर **end_ptr, /* May be NULL */
                              hb_font_t *font, /* May be NULL */
                              hb_बफर_serialize_format_t format)
{
  स्थिर अक्षर *end;
  अगर (!end_ptr)
    end_ptr = &end;
  *end_ptr = buf;

  बफर->assert_glyphs ();

  अगर (unlikely (hb_object_is_immutable (बफर)))
  {
    अगर (end_ptr)
      *end_ptr = buf;
    वापस false;
  }

  अगर (buf_len == -1)
    buf_len = म_माप (buf);

  अगर (!buf_len)
  {
    *end_ptr = buf;
    वापस false;
  }

  अगर (!font)
    font = hb_font_get_empty ();

  चयन (format)
  {
    हाल HB_बफर_SERIALIZE_FORMAT_TEXT:
      hb_बफर_set_content_type (बफर, HB_बफर_CONTENT_TYPE_GLYPHS);
      वापस hb_बफर_deserialize_text_glyphs (बफर,
						 buf, buf_len, end_ptr,
						 font);

    हाल HB_बफर_SERIALIZE_FORMAT_JSON:
      hb_बफर_set_content_type (बफर, HB_बफर_CONTENT_TYPE_GLYPHS);
      वापस hb_बफर_deserialize_json (बफर,
                                          buf, buf_len, end_ptr,
                                          font);

    शेष:
    हाल HB_बफर_SERIALIZE_FORMAT_INVALID:
      वापस false;

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
hb_बफर_deserialize_unicode (hb_बफर_t *बफर,
                               स्थिर अक्षर *buf,
                               पूर्णांक buf_len, /* -1 means nul-terminated */
                               स्थिर अक्षर **end_ptr, /* May be NULL */
                               hb_बफर_serialize_format_t format)
{
  स्थिर अक्षर *end;
  अगर (!end_ptr)
    end_ptr = &end;
  *end_ptr = buf;

  बफर->assert_unicode ();

  अगर (unlikely (hb_object_is_immutable (बफर)))
  {
    अगर (end_ptr)
      *end_ptr = buf;
    वापस false;
  }

  अगर (buf_len == -1)
    buf_len = म_माप (buf);

  अगर (!buf_len)
  {
    *end_ptr = buf;
    वापस false;
  }

  hb_font_t* font = hb_font_get_empty ();

  चयन (format)
  {
    हाल HB_बफर_SERIALIZE_FORMAT_TEXT:
      hb_बफर_set_content_type (बफर, HB_बफर_CONTENT_TYPE_UNICODE);
      वापस hb_बफर_deserialize_text_unicode (बफर,
						  buf, buf_len, end_ptr,
						  font);

    हाल HB_बफर_SERIALIZE_FORMAT_JSON:
      hb_बफर_set_content_type (बफर, HB_बफर_CONTENT_TYPE_UNICODE);
      वापस hb_बफर_deserialize_json (बफर,
                                          buf, buf_len, end_ptr,
                                          font);

    शेष:
    हाल HB_बफर_SERIALIZE_FORMAT_INVALID:
      वापस false;

  }
}


#पूर्ण_अगर
