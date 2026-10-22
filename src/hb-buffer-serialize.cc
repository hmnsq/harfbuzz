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

#অন্তর্ভাব "hb.hh"

#ifndef HB_NO_বাফার_SERIALIZE

#অন্তর্ভাব "hb-buffer.hh"


স্থির ধ্রুবক অক্ষর *hb_বাফার_serialize_formats[] = {
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
ধ্রুবক অক্ষর **
hb_বাফার_serialize_list_formats ()
{
  ফেরত hb_বাফার_serialize_formats;
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
hb_বাফার_serialize_format_t
hb_বাফার_serialize_format_from_string (ধ্রুবক অক্ষর *str, পূর্ণসংখ্যা len)
{
  /* Upper-case it. */
  ফেরত (hb_বাফার_serialize_format_t) (hb_tag_from_string (str, len) & ~0x20202020u);
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
ধ্রুবক অক্ষর *
hb_বাফার_serialize_format_to_string (hb_বাফার_serialize_format_t format)
{
  নির্বাচন ((unsigned) format)
  {
    ক্ষেত্র HB_বাফার_SERIALIZE_FORMAT_TEXT: ফেরত hb_বাফার_serialize_formats[0];
    ক্ষেত্র HB_বাফার_SERIALIZE_FORMAT_JSON: ফেরত hb_বাফার_serialize_formats[1];
    পূর্বনির্ধারিত:
    ক্ষেত্র HB_বাফার_SERIALIZE_FORMAT_INVALID:  ফেরত nullptr;
  }
}

স্থির unsigned পূর্ণসংখ্যা
hb_বাফার_serialize_glyphs_json (hb_বাফার_t *বাফার,
                                  unsigned পূর্ণসংখ্যা start,
                                  unsigned পূর্ণসংখ্যা end,
                                  অক্ষর *buf,
                                  unsigned পূর্ণসংখ্যা buf_আকার,
                                  unsigned পূর্ণসংখ্যা *buf_consumed,
                                  hb_font_t *font,
                                  hb_বাফার_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_বাফার_get_glyph_infos (বাফার, nullptr);
  hb_glyph_position_t *pos = (flags & HB_বাফার_SERIALIZE_FLAG_NO_POSITIONS) ?
                             nullptr : hb_বাফার_get_glyph_positions (বাফার, nullptr);

  *buf_consumed = 0;
  hb_position_t x = 0, y = 0;

  /* Calculate the advance of the previous glyphs */
  যদি (pos && (flags & HB_বাফার_SERIALIZE_FLAG_NO_ADVANCES))
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < start; i++)
    {
      x = hb_saturate_add (x, pos[i].x_advance);
      y = hb_saturate_add (y, pos[i].y_advance);
    }
  }

  জন্য (unsigned পূর্ণসংখ্যা i = start; i < end; i++)
  {
    অক্ষর b[1024];
    অক্ষর *p = b;

    /* In the following code, we know b is large enough that no overflow can happen. */

#define APPEND(s) HB_STMT_START { strcpy (p, s); p += strlen (s); } HB_STMT_END

    যদি (i)
      *p++ = ',';
    অন্যথা
      *p++ = '[';

    *p++ = '{';

    APPEND ("\"g\":");
    যদি (!(flags & HB_বাফার_SERIALIZE_FLAG_NO_GLYPH_NAMES))
    {
      অক্ষর g[128];
      hb_font_glyph_to_string (font, info[i].codepoint, g, আকার (g));
      *p++ = '"';
      জন্য (অক্ষর *q = g; *q; q++)
      {
	যদি (unlikely (*q == '"' || *q == '\\'))
	  *p++ = '\\';
	*p++ = *q;
      }
      *p++ = '"';
    }
    অন্যথা
      p += hb_max (0, snprintf (p, ARRAY_দৈর্ঘ্য (b) - (p - b), "%u", info[i].codepoint));

    যদি (!(flags & HB_বাফার_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_দৈর্ঘ্য (b) - (p - b), ",\"cl\":%u", info[i].cluster));
    }

    যদি (pos && !(flags & HB_বাফার_SERIALIZE_FLAG_NO_POSITIONS))
    {
      hb_position_t dx = hb_saturate_add (x, pos[i].x_offset);
      hb_position_t dy = hb_saturate_add (y, pos[i].y_offset);
      p += hb_max (0, snprintf (p, ARRAY_দৈর্ঘ্য (b) - (p - b), ",\"dx\":%d,\"dy\":%d",
		   dx, dy));
      যদি (!(flags & HB_বাফার_SERIALIZE_FLAG_NO_ADVANCES))
        p += hb_max (0, snprintf (p, ARRAY_দৈর্ঘ্য (b) - (p - b), ",\"ax\":%d,\"ay\":%d",
		     pos[i].x_advance, pos[i].y_advance));
    }

    যদি (flags & HB_বাফার_SERIALIZE_FLAG_GLYPH_FLAGS)
    {
      যদি (info[i].mask & HB_GLYPH_FLAG_DEFINED)
        p += hb_max (0, snprintf (p, ARRAY_দৈর্ঘ্য (b) - (p - b), ",\"fl\":%u", info[i].mask & HB_GLYPH_FLAG_DEFINED));
    }

    যদি (flags & HB_বাফার_SERIALIZE_FLAG_GLYPH_EXTENTS)
    {
      hb_glyph_extents_t extents;
      যদি (hb_font_get_glyph_extents(font, info[i].codepoint, &extents))
      {
	p += hb_max (0, snprintf (p, ARRAY_দৈর্ঘ্য (b) - (p - b), ",\"xb\":%d,\"yb\":%d",
				  extents.x_bearing, extents.y_bearing));
	p += hb_max (0, snprintf (p, ARRAY_দৈর্ঘ্য (b) - (p - b), ",\"w\":%d,\"h\":%d",
				  extents.width, extents.height));
      }
    }

    *p++ = '}';
    যদি (i == end-1)
      *p++ = ']';

    unsigned পূর্ণসংখ্যা l = p - b;
    যদি (buf_আকার > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_আকার -= l;
      *buf_consumed += l;
      *buf = '\0';
    } অন্যথা
      ফেরত i - start;

    যদি (pos && (flags & HB_বাফার_SERIALIZE_FLAG_NO_ADVANCES))
    {
      x = hb_saturate_add (x, pos[i].x_advance);
      y = hb_saturate_add (y, pos[i].y_advance);
    }
  }

  ফেরত end - start;
}

স্থির unsigned পূর্ণসংখ্যা
hb_বাফার_serialize_unicode_json (hb_বাফার_t *বাফার,
          unsigned পূর্ণসংখ্যা start,
          unsigned পূর্ণসংখ্যা end,
          অক্ষর *buf,
          unsigned পূর্ণসংখ্যা buf_আকার,
          unsigned পূর্ণসংখ্যা *buf_consumed,
          hb_বাফার_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_বাফার_get_glyph_infos (বাফার, nullptr);

  *buf_consumed = 0;
  জন্য (unsigned পূর্ণসংখ্যা i = start; i < end; i++)
  {
    অক্ষর b[1024];
    অক্ষর *p = b;

    যদি (i)
      *p++ = ',';
    অন্যথা
      *p++ = '[';

    *p++ = '{';

    APPEND ("\"u\":");

    p += hb_max (0, snprintf (p, ARRAY_দৈর্ঘ্য (b) - (p - b), "%u", info[i].codepoint));

    যদি (!(flags & HB_বাফার_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_দৈর্ঘ্য (b) - (p - b), ",\"cl\":%u", info[i].cluster));
    }

    *p++ = '}';

    যদি (i == end-1)
      *p++ = ']';

    unsigned পূর্ণসংখ্যা l = p - b;
    যদি (buf_আকার > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_আকার -= l;
      *buf_consumed += l;
      *buf = '\0';
    } অন্যথা
      ফেরত i - start;

  }

  ফেরত end - start;
}

স্থির unsigned পূর্ণসংখ্যা
hb_বাফার_serialize_glyphs_text (hb_বাফার_t *বাফার,
                                  unsigned পূর্ণসংখ্যা start,
                                  unsigned পূর্ণসংখ্যা end,
                                  অক্ষর *buf,
                                  unsigned পূর্ণসংখ্যা buf_আকার,
                                  unsigned পূর্ণসংখ্যা *buf_consumed,
                                  hb_font_t *font,
                                  hb_বাফার_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_বাফার_get_glyph_infos (বাফার, nullptr);
  hb_glyph_position_t *pos = (flags & HB_বাফার_SERIALIZE_FLAG_NO_POSITIONS) ?
           nullptr : hb_বাফার_get_glyph_positions (বাফার, nullptr);

  *buf_consumed = 0;
  hb_position_t x = 0, y = 0;

  /* Calculate the advance of the previous glyphs */
  যদি (pos && (flags & HB_বাফার_SERIALIZE_FLAG_NO_ADVANCES))
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < start; i++)
    {
      x = hb_saturate_add (x, pos[i].x_advance);
      y = hb_saturate_add (y, pos[i].y_advance);
    }
  }

  জন্য (unsigned পূর্ণসংখ্যা i = start; i < end; i++)
  {
    অক্ষর b[1024];
    অক্ষর *p = b;

    /* In the following code, we know b is large enough that no overflow can happen. */

    যদি (i)
      *p++ = '|';
    অন্যথা
      *p++ = '[';

    যদি (!(flags & HB_বাফার_SERIALIZE_FLAG_NO_GLYPH_NAMES))
    {
      /* TODO Escape delimiters we use. */
      hb_font_glyph_to_string (font, info[i].codepoint, p, 128);
      p += strlen (p);
    }
    অন্যথা
      p += hb_max (0, snprintf (p, ARRAY_দৈর্ঘ্য (b) - (p - b), "%u", info[i].codepoint));

    যদি (!(flags & HB_বাফার_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_দৈর্ঘ্য (b) - (p - b), "=%u", info[i].cluster));
    }

    যদি (pos && !(flags & HB_বাফার_SERIALIZE_FLAG_NO_POSITIONS))
    {
      hb_position_t dx = hb_saturate_add (x, pos[i].x_offset);
      hb_position_t dy = hb_saturate_add (y, pos[i].y_offset);
      যদি (dx || dy)
        p += hb_max (0, snprintf (p, ARRAY_দৈর্ঘ্য (b) - (p - b), "@%d,%d", dx, dy));

      যদি (!(flags & HB_বাফার_SERIALIZE_FLAG_NO_ADVANCES))
      {
        *p++ = '+';
        p += hb_max (0, snprintf (p, ARRAY_দৈর্ঘ্য (b) - (p - b), "%d", pos[i].x_advance));
        যদি (pos[i].y_advance)
          p += hb_max (0, snprintf (p, ARRAY_দৈর্ঘ্য (b) - (p - b), ",%d", pos[i].y_advance));
      }
    }

    যদি (flags & HB_বাফার_SERIALIZE_FLAG_GLYPH_FLAGS)
    {
      যদি (info[i].mask & HB_GLYPH_FLAG_DEFINED)
        p += hb_max (0, snprintf (p, ARRAY_দৈর্ঘ্য (b) - (p - b), "#%X", info[i].mask &HB_GLYPH_FLAG_DEFINED));
    }

    যদি (flags & HB_বাফার_SERIALIZE_FLAG_GLYPH_EXTENTS)
    {
      hb_glyph_extents_t extents;
      যদি (hb_font_get_glyph_extents(font, info[i].codepoint, &extents))
	p += hb_max (0, snprintf (p, ARRAY_দৈর্ঘ্য (b) - (p - b), "<%d,%d,%d,%d>", extents.x_bearing, extents.y_bearing, extents.width, extents.height));
    }

    যদি (i == end-1) {
      *p++ = ']';
    }

    unsigned পূর্ণসংখ্যা l = p - b;
    যদি (buf_আকার > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_আকার -= l;
      *buf_consumed += l;
      *buf = '\0';
    } অন্যথা
      ফেরত i - start;

    যদি (pos && (flags & HB_বাফার_SERIALIZE_FLAG_NO_ADVANCES))
    {
      x = hb_saturate_add (x, pos[i].x_advance);
      y = hb_saturate_add (y, pos[i].y_advance);
    }
  }

  ফেরত end - start;
}


স্থির unsigned পূর্ণসংখ্যা
hb_বাফার_serialize_unicode_text (hb_বাফার_t *বাফার,
                                   unsigned পূর্ণসংখ্যা start,
                                   unsigned পূর্ণসংখ্যা end,
                                   অক্ষর *buf,
                                   unsigned পূর্ণসংখ্যা buf_আকার,
                                   unsigned পূর্ণসংখ্যা *buf_consumed,
                                   hb_বাফার_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_বাফার_get_glyph_infos (বাফার, nullptr);
  *buf_consumed = 0;
  জন্য (unsigned পূর্ণসংখ্যা i = start; i < end; i++)
  {
    অক্ষর b[1024];
    অক্ষর *p = b;

    যদি (i)
      *p++ = '|';
    অন্যথা
      *p++ = '<';

    p += hb_max (0, snprintf (p, ARRAY_দৈর্ঘ্য (b) - (p - b), "U+%04X", info[i].codepoint));

    যদি (!(flags & HB_বাফার_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_দৈর্ঘ্য (b) - (p - b), "=%u", info[i].cluster));
    }

    যদি (i == end-1)
      *p++ = '>';

    unsigned পূর্ণসংখ্যা l = p - b;
    যদি (buf_আকার > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_আকার -= l;
      *buf_consumed += l;
      *buf = '\0';
    } অন্যথা
      ফেরত i - start;
  }
  ফেরত end - start;
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
unsigned পূর্ণসংখ্যা
hb_বাফার_serialize_glyphs (hb_বাফার_t *বাফার,
                            unsigned পূর্ণসংখ্যা start,
                            unsigned পূর্ণসংখ্যা end,
                            অক্ষর *buf,
                            unsigned পূর্ণসংখ্যা buf_আকার,
                            unsigned পূর্ণসংখ্যা *buf_consumed,
                            hb_font_t *font,
                            hb_বাফার_serialize_format_t format,
                            hb_বাফার_serialize_flags_t flags)
{
  end = hb_clamp (end, start, বাফার->len);
  start = hb_min (start, end);

  unsigned পূর্ণসংখ্যা sconsumed;
  যদি (!buf_consumed)
    buf_consumed = &sconsumed;
  *buf_consumed = 0;
  যদি (buf_আকার)
    *buf = '\0';

  বাফার->assert_glyphs ();

  যদি (!বাফার->have_positions)
    flags |= HB_বাফার_SERIALIZE_FLAG_NO_POSITIONS;

  যদি (unlikely (start == end))
    ফেরত 0;

  যদি (!font)
    font = hb_font_get_empty ();

  নির্বাচন (format)
  {
    ক্ষেত্র HB_বাফার_SERIALIZE_FORMAT_TEXT:
      ফেরত hb_বাফার_serialize_glyphs_text (বাফার, start, end,
                 buf, buf_আকার, buf_consumed,
                 font, flags);

    ক্ষেত্র HB_বাফার_SERIALIZE_FORMAT_JSON:
      ফেরত hb_বাফার_serialize_glyphs_json (বাফার, start, end,
                 buf, buf_আকার, buf_consumed,
                 font, flags);

    পূর্বনির্ধারিত:
    ক্ষেত্র HB_বাফার_SERIALIZE_FORMAT_INVALID:
      ফেরত 0;

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
unsigned পূর্ণসংখ্যা
hb_বাফার_serialize_unicode (hb_বাফার_t *বাফার,
                             unsigned পূর্ণসংখ্যা start,
                             unsigned পূর্ণসংখ্যা end,
                             অক্ষর *buf,
                             unsigned পূর্ণসংখ্যা buf_আকার,
                             unsigned পূর্ণসংখ্যা *buf_consumed,
                             hb_বাফার_serialize_format_t format,
                             hb_বাফার_serialize_flags_t flags)
{
  end = hb_clamp (end, start, বাফার->len);
  start = hb_min (start, end);

  unsigned পূর্ণসংখ্যা sconsumed;
  যদি (!buf_consumed)
    buf_consumed = &sconsumed;
  *buf_consumed = 0;
  যদি (buf_আকার)
    *buf = '\0';

  বাফার->assert_unicode ();

  যদি (unlikely (start == end))
    ফেরত 0;

  নির্বাচন (format)
  {
    ক্ষেত্র HB_বাফার_SERIALIZE_FORMAT_TEXT:
      ফেরত hb_বাফার_serialize_unicode_text (বাফার, start, end,
                                                buf, buf_আকার, buf_consumed, flags);

    ক্ষেত্র HB_বাফার_SERIALIZE_FORMAT_JSON:
      ফেরত hb_বাফার_serialize_unicode_json (বাফার, start, end,
                                                buf, buf_আকার, buf_consumed, flags);

    পূর্বনির্ধারিত:
    ক্ষেত্র HB_বাফার_SERIALIZE_FORMAT_INVALID:
      ফেরত 0;

  }
}

স্থির unsigned পূর্ণসংখ্যা
hb_বাফার_serialize_invalid (hb_বাফার_t *বাফার,
                              unsigned পূর্ণসংখ্যা start,
                              unsigned পূর্ণসংখ্যা end,
                              অক্ষর *buf,
                              unsigned পূর্ণসংখ্যা buf_আকার,
                              unsigned পূর্ণসংখ্যা *buf_consumed,
                              hb_বাফার_serialize_format_t format,
                              hb_বাফার_serialize_flags_t flags)
{
  প্রতিজ্ঞা (!বাফার->len);

  unsigned পূর্ণসংখ্যা sconsumed;
  যদি (!buf_consumed)
    buf_consumed = &sconsumed;
  যদি (buf_আকার < 3)
    ফেরত 0;
  যদি (format == HB_বাফার_SERIALIZE_FORMAT_JSON) {
    *buf++ = '[';
    *buf++ = ']';
    *buf = '\0';
  } অন্যথা যদি (format == HB_বাফার_SERIALIZE_FORMAT_TEXT) {
    *buf++ = '!';
    *buf++ = '!';
    *buf = '\0';
  }
  *buf_consumed = 2;
  ফেরত 0;
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
unsigned পূর্ণসংখ্যা
hb_বাফার_serialize (hb_বাফার_t *বাফার,
                     unsigned পূর্ণসংখ্যা start,
                     unsigned পূর্ণসংখ্যা end,
                     অক্ষর *buf,
                     unsigned পূর্ণসংখ্যা buf_আকার,
                     unsigned পূর্ণসংখ্যা *buf_consumed,
                     hb_font_t *font,
                     hb_বাফার_serialize_format_t format,
                     hb_বাফার_serialize_flags_t flags)
{
  নির্বাচন (বাফার->content_type)
  {

    ক্ষেত্র HB_বাফার_CONTENT_TYPE_GLYPHS:
      ফেরত hb_বাফার_serialize_glyphs (বাফার, start, end, buf, buf_আকার,
					 buf_consumed, font, format, flags);

    ক্ষেত্র HB_বাফার_CONTENT_TYPE_UNICODE:
      ফেরত hb_বাফার_serialize_unicode (বাফার, start, end, buf, buf_আকার,
					  buf_consumed, format, flags);

    ক্ষেত্র HB_বাফার_CONTENT_TYPE_INVALID:
    পূর্বনির্ধারিত:
      ফেরত hb_বাফার_serialize_invalid (বাফার, start, end, buf, buf_আকার,
					   buf_consumed, format, flags);
  }
}

স্থির যৌক্তিক
parse_int (ধ্রুবক অক্ষর *pp, ধ্রুবক অক্ষর *end, int32_t *pv)
{
  পূর্ণসংখ্যা v;
  ধ্রুবক অক্ষর *p = pp;
  যদি (unlikely (!hb_parse_int (&p, end, &v, সত্য/* whole buffer */)))
    ফেরত মিথ্যা;

  *pv = v;
  ফেরত সত্য;
}

স্থির যৌক্তিক
parse_uint (ধ্রুবক অক্ষর *pp, ধ্রুবক অক্ষর *end, uint32_t *pv)
{
  unsigned পূর্ণসংখ্যা v;
  ধ্রুবক অক্ষর *p = pp;
  যদি (unlikely (!hb_parse_uint (&p, end, &v, সত্য/* whole buffer */)))
    ফেরত মিথ্যা;

  *pv = v;
  ফেরত সত্য;
}

স্থির যৌক্তিক
parse_hex (ধ্রুবক অক্ষর *pp, ধ্রুবক অক্ষর *end, uint32_t *pv)
{
  unsigned পূর্ণসংখ্যা v;
  ধ্রুবক অক্ষর *p = pp;
  যদি (unlikely (!hb_parse_uint (&p, end, &v, সত্য/* whole buffer */, 16)))
    ফেরত মিথ্যা;

  *pv = v;
  ফেরত সত্য;
}

#অন্তর্ভাব "hb-buffer-deserialize-json.hh"
#অন্তর্ভাব "hb-buffer-deserialize-text-glyphs.hh"
#অন্তর্ভাব "hb-buffer-deserialize-text-unicode.hh"

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
hb_বাফার_deserialize_glyphs (hb_বাফার_t *বাফার,
                              ধ্রুবক অক্ষর *buf,
                              পূর্ণসংখ্যা buf_len, /* -1 means nul-terminated */
                              ধ্রুবক অক্ষর **end_ptr, /* May be NULL */
                              hb_font_t *font, /* May be NULL */
                              hb_বাফার_serialize_format_t format)
{
  ধ্রুবক অক্ষর *end;
  যদি (!end_ptr)
    end_ptr = &end;
  *end_ptr = buf;

  বাফার->assert_glyphs ();

  যদি (unlikely (hb_object_is_immutable (বাফার)))
  {
    যদি (end_ptr)
      *end_ptr = buf;
    ফেরত মিথ্যা;
  }

  যদি (buf_len == -1)
    buf_len = strlen (buf);

  যদি (!buf_len)
  {
    *end_ptr = buf;
    ফেরত মিথ্যা;
  }

  যদি (!font)
    font = hb_font_get_empty ();

  নির্বাচন (format)
  {
    ক্ষেত্র HB_বাফার_SERIALIZE_FORMAT_TEXT:
      hb_বাফার_set_content_type (বাফার, HB_বাফার_CONTENT_TYPE_GLYPHS);
      ফেরত hb_বাফার_deserialize_text_glyphs (বাফার,
						 buf, buf_len, end_ptr,
						 font);

    ক্ষেত্র HB_বাফার_SERIALIZE_FORMAT_JSON:
      hb_বাফার_set_content_type (বাফার, HB_বাফার_CONTENT_TYPE_GLYPHS);
      ফেরত hb_বাফার_deserialize_json (বাফার,
                                          buf, buf_len, end_ptr,
                                          font);

    পূর্বনির্ধারিত:
    ক্ষেত্র HB_বাফার_SERIALIZE_FORMAT_INVALID:
      ফেরত মিথ্যা;

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
hb_বাফার_deserialize_unicode (hb_বাফার_t *বাফার,
                               ধ্রুবক অক্ষর *buf,
                               পূর্ণসংখ্যা buf_len, /* -1 means nul-terminated */
                               ধ্রুবক অক্ষর **end_ptr, /* May be NULL */
                               hb_বাফার_serialize_format_t format)
{
  ধ্রুবক অক্ষর *end;
  যদি (!end_ptr)
    end_ptr = &end;
  *end_ptr = buf;

  বাফার->assert_unicode ();

  যদি (unlikely (hb_object_is_immutable (বাফার)))
  {
    যদি (end_ptr)
      *end_ptr = buf;
    ফেরত মিথ্যা;
  }

  যদি (buf_len == -1)
    buf_len = strlen (buf);

  যদি (!buf_len)
  {
    *end_ptr = buf;
    ফেরত মিথ্যা;
  }

  hb_font_t* font = hb_font_get_empty ();

  নির্বাচন (format)
  {
    ক্ষেত্র HB_বাফার_SERIALIZE_FORMAT_TEXT:
      hb_বাফার_set_content_type (বাফার, HB_বাফার_CONTENT_TYPE_UNICODE);
      ফেরত hb_বাফার_deserialize_text_unicode (বাফার,
						  buf, buf_len, end_ptr,
						  font);

    ক্ষেত্র HB_বাফার_SERIALIZE_FORMAT_JSON:
      hb_বাফার_set_content_type (বাফার, HB_বাফার_CONTENT_TYPE_UNICODE);
      ফেরত hb_বাফার_deserialize_json (বাফার,
                                          buf, buf_len, end_ptr,
                                          font);

    পূর্বনির্ধারিত:
    ক্ষেত্র HB_বাফার_SERIALIZE_FORMAT_INVALID:
      ফেরত মিথ্যা;

  }
}


#endif
