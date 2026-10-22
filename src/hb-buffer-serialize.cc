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

#அந்தர்பாவ "hb.hh"

#ifndef HB_NO_BUFFER_SERIALIZE

#அந்தர்பாவ "hb-buffer.hh"


நிலையான மாறிலி எழுத்து *_hb_buffer_serialize_formats[] = {
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
மாறிலி எழுத்து **
hb_buffer_serialize_list_formats ()
{
  திரும்பு _hb_buffer_serialize_formats;
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
hb_buffer_serialize_format_t
hb_buffer_serialize_format_from_string (மாறிலி எழுத்து *str, முழு len)
{
  /* Upper-case it. */
  திரும்பு (hb_buffer_serialize_format_t) (hb_tag_from_string (str, len) & ~0x20202020u);
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
மாறிலி எழுத்து *
hb_buffer_serialize_format_to_string (hb_buffer_serialize_format_t format)
{
  தேர்வு ((unsigned) format)
  {
    நிலை HB_BUFFER_SERIALIZE_FORMAT_TEXT: திரும்பு _hb_buffer_serialize_formats[0];
    நிலை HB_BUFFER_SERIALIZE_FORMAT_JSON: திரும்பு _hb_buffer_serialize_formats[1];
    இயல்பு:
    நிலை HB_BUFFER_SERIALIZE_FORMAT_INVALID:  திரும்பு nullptr;
  }
}

நிலையான unsigned முழு
_hb_buffer_serialize_glyphs_json (hb_buffer_t *buffer,
                                  unsigned முழு start,
                                  unsigned முழு end,
                                  எழுத்து *buf,
                                  unsigned முழு buf_size,
                                  unsigned முழு *buf_consumed,
                                  hb_font_t *font,
                                  hb_buffer_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_buffer_get_glyph_infos (buffer, nullptr);
  hb_glyph_position_t *pos = (flags & HB_BUFFER_SERIALIZE_FLAG_NO_POSITIONS) ?
                             nullptr : hb_buffer_get_glyph_positions (buffer, nullptr);

  *buf_consumed = 0;
  hb_position_t x = 0, y = 0;

  /* Calculate the advance of the previous glyphs */
  எனில் (pos && (flags & HB_BUFFER_SERIALIZE_FLAG_NO_ADVANCES))
  {
    க்கு (unsigned முழு i = 0; i < start; i++)
    {
      x = hb_saturate_add (x, pos[i].x_advance);
      y = hb_saturate_add (y, pos[i].y_advance);
    }
  }

  க்கு (unsigned முழு i = start; i < end; i++)
  {
    எழுத்து b[1024];
    எழுத்து *p = b;

    /* In the following code, we know b is large enough that no overflow can happen. */

#define APPEND(s) HB_STMT_START { strcpy (p, s); p += strlen (s); } HB_STMT_END

    எனில் (i)
      *p++ = ',';
    இல்லையெனில்
      *p++ = '[';

    *p++ = '{';

    APPEND ("\"g\":");
    எனில் (!(flags & HB_BUFFER_SERIALIZE_FLAG_NO_GLYPH_NAMES))
    {
      எழுத்து g[128];
      hb_font_glyph_to_string (font, info[i].codepoint, g, அளவு (g));
      *p++ = '"';
      க்கு (எழுத்து *q = g; *q; q++)
      {
	எனில் (unlikely (*q == '"' || *q == '\\'))
	  *p++ = '\\';
	*p++ = *q;
      }
      *p++ = '"';
    }
    இல்லையெனில்
      p += hb_max (0, snprintf (p, ARRAY_LENGTH (b) - (p - b), "%u", info[i].codepoint));

    எனில் (!(flags & HB_BUFFER_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_LENGTH (b) - (p - b), ",\"cl\":%u", info[i].cluster));
    }

    எனில் (pos && !(flags & HB_BUFFER_SERIALIZE_FLAG_NO_POSITIONS))
    {
      hb_position_t dx = hb_saturate_add (x, pos[i].x_offset);
      hb_position_t dy = hb_saturate_add (y, pos[i].y_offset);
      p += hb_max (0, snprintf (p, ARRAY_LENGTH (b) - (p - b), ",\"dx\":%d,\"dy\":%d",
		   dx, dy));
      எனில் (!(flags & HB_BUFFER_SERIALIZE_FLAG_NO_ADVANCES))
        p += hb_max (0, snprintf (p, ARRAY_LENGTH (b) - (p - b), ",\"ax\":%d,\"ay\":%d",
		     pos[i].x_advance, pos[i].y_advance));
    }

    எனில் (flags & HB_BUFFER_SERIALIZE_FLAG_GLYPH_FLAGS)
    {
      எனில் (info[i].mask & HB_GLYPH_FLAG_DEFINED)
        p += hb_max (0, snprintf (p, ARRAY_LENGTH (b) - (p - b), ",\"fl\":%u", info[i].mask & HB_GLYPH_FLAG_DEFINED));
    }

    எனில் (flags & HB_BUFFER_SERIALIZE_FLAG_GLYPH_EXTENTS)
    {
      hb_glyph_extents_t extents;
      எனில் (hb_font_get_glyph_extents(font, info[i].codepoint, &extents))
      {
	p += hb_max (0, snprintf (p, ARRAY_LENGTH (b) - (p - b), ",\"xb\":%d,\"yb\":%d",
				  extents.x_bearing, extents.y_bearing));
	p += hb_max (0, snprintf (p, ARRAY_LENGTH (b) - (p - b), ",\"w\":%d,\"h\":%d",
				  extents.width, extents.height));
      }
    }

    *p++ = '}';
    எனில் (i == end-1)
      *p++ = ']';

    unsigned முழு l = p - b;
    எனில் (buf_size > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_size -= l;
      *buf_consumed += l;
      *buf = '\0';
    } இல்லையெனில்
      திரும்பு i - start;

    எனில் (pos && (flags & HB_BUFFER_SERIALIZE_FLAG_NO_ADVANCES))
    {
      x = hb_saturate_add (x, pos[i].x_advance);
      y = hb_saturate_add (y, pos[i].y_advance);
    }
  }

  திரும்பு end - start;
}

நிலையான unsigned முழு
_hb_buffer_serialize_unicode_json (hb_buffer_t *buffer,
          unsigned முழு start,
          unsigned முழு end,
          எழுத்து *buf,
          unsigned முழு buf_size,
          unsigned முழு *buf_consumed,
          hb_buffer_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_buffer_get_glyph_infos (buffer, nullptr);

  *buf_consumed = 0;
  க்கு (unsigned முழு i = start; i < end; i++)
  {
    எழுத்து b[1024];
    எழுத்து *p = b;

    எனில் (i)
      *p++ = ',';
    இல்லையெனில்
      *p++ = '[';

    *p++ = '{';

    APPEND ("\"u\":");

    p += hb_max (0, snprintf (p, ARRAY_LENGTH (b) - (p - b), "%u", info[i].codepoint));

    எனில் (!(flags & HB_BUFFER_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_LENGTH (b) - (p - b), ",\"cl\":%u", info[i].cluster));
    }

    *p++ = '}';

    எனில் (i == end-1)
      *p++ = ']';

    unsigned முழு l = p - b;
    எனில் (buf_size > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_size -= l;
      *buf_consumed += l;
      *buf = '\0';
    } இல்லையெனில்
      திரும்பு i - start;

  }

  திரும்பு end - start;
}

நிலையான unsigned முழு
_hb_buffer_serialize_glyphs_text (hb_buffer_t *buffer,
                                  unsigned முழு start,
                                  unsigned முழு end,
                                  எழுத்து *buf,
                                  unsigned முழு buf_size,
                                  unsigned முழு *buf_consumed,
                                  hb_font_t *font,
                                  hb_buffer_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_buffer_get_glyph_infos (buffer, nullptr);
  hb_glyph_position_t *pos = (flags & HB_BUFFER_SERIALIZE_FLAG_NO_POSITIONS) ?
           nullptr : hb_buffer_get_glyph_positions (buffer, nullptr);

  *buf_consumed = 0;
  hb_position_t x = 0, y = 0;

  /* Calculate the advance of the previous glyphs */
  எனில் (pos && (flags & HB_BUFFER_SERIALIZE_FLAG_NO_ADVANCES))
  {
    க்கு (unsigned முழு i = 0; i < start; i++)
    {
      x = hb_saturate_add (x, pos[i].x_advance);
      y = hb_saturate_add (y, pos[i].y_advance);
    }
  }

  க்கு (unsigned முழு i = start; i < end; i++)
  {
    எழுத்து b[1024];
    எழுத்து *p = b;

    /* In the following code, we know b is large enough that no overflow can happen. */

    எனில் (i)
      *p++ = '|';
    இல்லையெனில்
      *p++ = '[';

    எனில் (!(flags & HB_BUFFER_SERIALIZE_FLAG_NO_GLYPH_NAMES))
    {
      /* TODO Escape delimiters we use. */
      hb_font_glyph_to_string (font, info[i].codepoint, p, 128);
      p += strlen (p);
    }
    இல்லையெனில்
      p += hb_max (0, snprintf (p, ARRAY_LENGTH (b) - (p - b), "%u", info[i].codepoint));

    எனில் (!(flags & HB_BUFFER_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_LENGTH (b) - (p - b), "=%u", info[i].cluster));
    }

    எனில் (pos && !(flags & HB_BUFFER_SERIALIZE_FLAG_NO_POSITIONS))
    {
      hb_position_t dx = hb_saturate_add (x, pos[i].x_offset);
      hb_position_t dy = hb_saturate_add (y, pos[i].y_offset);
      எனில் (dx || dy)
        p += hb_max (0, snprintf (p, ARRAY_LENGTH (b) - (p - b), "@%d,%d", dx, dy));

      எனில் (!(flags & HB_BUFFER_SERIALIZE_FLAG_NO_ADVANCES))
      {
        *p++ = '+';
        p += hb_max (0, snprintf (p, ARRAY_LENGTH (b) - (p - b), "%d", pos[i].x_advance));
        எனில் (pos[i].y_advance)
          p += hb_max (0, snprintf (p, ARRAY_LENGTH (b) - (p - b), ",%d", pos[i].y_advance));
      }
    }

    எனில் (flags & HB_BUFFER_SERIALIZE_FLAG_GLYPH_FLAGS)
    {
      எனில் (info[i].mask & HB_GLYPH_FLAG_DEFINED)
        p += hb_max (0, snprintf (p, ARRAY_LENGTH (b) - (p - b), "#%X", info[i].mask &HB_GLYPH_FLAG_DEFINED));
    }

    எனில் (flags & HB_BUFFER_SERIALIZE_FLAG_GLYPH_EXTENTS)
    {
      hb_glyph_extents_t extents;
      எனில் (hb_font_get_glyph_extents(font, info[i].codepoint, &extents))
	p += hb_max (0, snprintf (p, ARRAY_LENGTH (b) - (p - b), "<%d,%d,%d,%d>", extents.x_bearing, extents.y_bearing, extents.width, extents.height));
    }

    எனில் (i == end-1) {
      *p++ = ']';
    }

    unsigned முழு l = p - b;
    எனில் (buf_size > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_size -= l;
      *buf_consumed += l;
      *buf = '\0';
    } இல்லையெனில்
      திரும்பு i - start;

    எனில் (pos && (flags & HB_BUFFER_SERIALIZE_FLAG_NO_ADVANCES))
    {
      x = hb_saturate_add (x, pos[i].x_advance);
      y = hb_saturate_add (y, pos[i].y_advance);
    }
  }

  திரும்பு end - start;
}


நிலையான unsigned முழு
_hb_buffer_serialize_unicode_text (hb_buffer_t *buffer,
                                   unsigned முழு start,
                                   unsigned முழு end,
                                   எழுத்து *buf,
                                   unsigned முழு buf_size,
                                   unsigned முழு *buf_consumed,
                                   hb_buffer_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_buffer_get_glyph_infos (buffer, nullptr);
  *buf_consumed = 0;
  க்கு (unsigned முழு i = start; i < end; i++)
  {
    எழுத்து b[1024];
    எழுத்து *p = b;

    எனில் (i)
      *p++ = '|';
    இல்லையெனில்
      *p++ = '<';

    p += hb_max (0, snprintf (p, ARRAY_LENGTH (b) - (p - b), "U+%04X", info[i].codepoint));

    எனில் (!(flags & HB_BUFFER_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_LENGTH (b) - (p - b), "=%u", info[i].cluster));
    }

    எனில் (i == end-1)
      *p++ = '>';

    unsigned முழு l = p - b;
    எனில் (buf_size > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_size -= l;
      *buf_consumed += l;
      *buf = '\0';
    } இல்லையெனில்
      திரும்பு i - start;
  }
  திரும்பு end - start;
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
unsigned முழு
hb_buffer_serialize_glyphs (hb_buffer_t *buffer,
                            unsigned முழு start,
                            unsigned முழு end,
                            எழுத்து *buf,
                            unsigned முழு buf_size,
                            unsigned முழு *buf_consumed,
                            hb_font_t *font,
                            hb_buffer_serialize_format_t format,
                            hb_buffer_serialize_flags_t flags)
{
  end = hb_clamp (end, start, buffer->len);
  start = hb_min (start, end);

  unsigned முழு sconsumed;
  எனில் (!buf_consumed)
    buf_consumed = &sconsumed;
  *buf_consumed = 0;
  எனில் (buf_size)
    *buf = '\0';

  buffer->assert_glyphs ();

  எனில் (!buffer->have_positions)
    flags |= HB_BUFFER_SERIALIZE_FLAG_NO_POSITIONS;

  எனில் (unlikely (start == end))
    திரும்பு 0;

  எனில் (!font)
    font = hb_font_get_empty ();

  தேர்வு (format)
  {
    நிலை HB_BUFFER_SERIALIZE_FORMAT_TEXT:
      திரும்பு _hb_buffer_serialize_glyphs_text (buffer, start, end,
                 buf, buf_size, buf_consumed,
                 font, flags);

    நிலை HB_BUFFER_SERIALIZE_FORMAT_JSON:
      திரும்பு _hb_buffer_serialize_glyphs_json (buffer, start, end,
                 buf, buf_size, buf_consumed,
                 font, flags);

    இயல்பு:
    நிலை HB_BUFFER_SERIALIZE_FORMAT_INVALID:
      திரும்பு 0;

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
unsigned முழு
hb_buffer_serialize_unicode (hb_buffer_t *buffer,
                             unsigned முழு start,
                             unsigned முழு end,
                             எழுத்து *buf,
                             unsigned முழு buf_size,
                             unsigned முழு *buf_consumed,
                             hb_buffer_serialize_format_t format,
                             hb_buffer_serialize_flags_t flags)
{
  end = hb_clamp (end, start, buffer->len);
  start = hb_min (start, end);

  unsigned முழு sconsumed;
  எனில் (!buf_consumed)
    buf_consumed = &sconsumed;
  *buf_consumed = 0;
  எனில் (buf_size)
    *buf = '\0';

  buffer->assert_unicode ();

  எனில் (unlikely (start == end))
    திரும்பு 0;

  தேர்வு (format)
  {
    நிலை HB_BUFFER_SERIALIZE_FORMAT_TEXT:
      திரும்பு _hb_buffer_serialize_unicode_text (buffer, start, end,
                                                buf, buf_size, buf_consumed, flags);

    நிலை HB_BUFFER_SERIALIZE_FORMAT_JSON:
      திரும்பு _hb_buffer_serialize_unicode_json (buffer, start, end,
                                                buf, buf_size, buf_consumed, flags);

    இயல்பு:
    நிலை HB_BUFFER_SERIALIZE_FORMAT_INVALID:
      திரும்பு 0;

  }
}

நிலையான unsigned முழு
_hb_buffer_serialize_invalid (hb_buffer_t *buffer,
                              unsigned முழு start,
                              unsigned முழு end,
                              எழுத்து *buf,
                              unsigned முழு buf_size,
                              unsigned முழு *buf_consumed,
                              hb_buffer_serialize_format_t format,
                              hb_buffer_serialize_flags_t flags)
{
  ப்ரதிஜ்ஞா (!buffer->len);

  unsigned முழு sconsumed;
  எனில் (!buf_consumed)
    buf_consumed = &sconsumed;
  எனில் (buf_size < 3)
    திரும்பு 0;
  எனில் (format == HB_BUFFER_SERIALIZE_FORMAT_JSON) {
    *buf++ = '[';
    *buf++ = ']';
    *buf = '\0';
  } இல்லையெனில் எனில் (format == HB_BUFFER_SERIALIZE_FORMAT_TEXT) {
    *buf++ = '!';
    *buf++ = '!';
    *buf = '\0';
  }
  *buf_consumed = 2;
  திரும்பு 0;
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
unsigned முழு
hb_buffer_serialize (hb_buffer_t *buffer,
                     unsigned முழு start,
                     unsigned முழு end,
                     எழுத்து *buf,
                     unsigned முழு buf_size,
                     unsigned முழு *buf_consumed,
                     hb_font_t *font,
                     hb_buffer_serialize_format_t format,
                     hb_buffer_serialize_flags_t flags)
{
  தேர்வு (buffer->content_type)
  {

    நிலை HB_BUFFER_CONTENT_TYPE_GLYPHS:
      திரும்பு hb_buffer_serialize_glyphs (buffer, start, end, buf, buf_size,
					 buf_consumed, font, format, flags);

    நிலை HB_BUFFER_CONTENT_TYPE_UNICODE:
      திரும்பு hb_buffer_serialize_unicode (buffer, start, end, buf, buf_size,
					  buf_consumed, format, flags);

    நிலை HB_BUFFER_CONTENT_TYPE_INVALID:
    இயல்பு:
      திரும்பு _hb_buffer_serialize_invalid (buffer, start, end, buf, buf_size,
					   buf_consumed, format, flags);
  }
}

நிலையான தருக்க
parse_int (மாறிலி எழுத்து *pp, மாறிலி எழுத்து *end, int32_t *pv)
{
  முழு v;
  மாறிலி எழுத்து *p = pp;
  எனில் (unlikely (!hb_parse_int (&p, end, &v, உண்மை/* whole buffer */)))
    திரும்பு பொய்;

  *pv = v;
  திரும்பு உண்மை;
}

நிலையான தருக்க
parse_uint (மாறிலி எழுத்து *pp, மாறிலி எழுத்து *end, uint32_t *pv)
{
  unsigned முழு v;
  மாறிலி எழுத்து *p = pp;
  எனில் (unlikely (!hb_parse_uint (&p, end, &v, உண்மை/* whole buffer */)))
    திரும்பு பொய்;

  *pv = v;
  திரும்பு உண்மை;
}

நிலையான தருக்க
parse_hex (மாறிலி எழுத்து *pp, மாறிலி எழுத்து *end, uint32_t *pv)
{
  unsigned முழு v;
  மாறிலி எழுத்து *p = pp;
  எனில் (unlikely (!hb_parse_uint (&p, end, &v, உண்மை/* whole buffer */, 16)))
    திரும்பு பொய்;

  *pv = v;
  திரும்பு உண்மை;
}

#அந்தர்பாவ "hb-buffer-deserialize-json.hh"
#அந்தர்பாவ "hb-buffer-deserialize-text-glyphs.hh"
#அந்தர்பாவ "hb-buffer-deserialize-text-unicode.hh"

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
hb_buffer_deserialize_glyphs (hb_buffer_t *buffer,
                              மாறிலி எழுத்து *buf,
                              முழு buf_len, /* -1 means nul-terminated */
                              மாறிலி எழுத்து **end_ptr, /* May be NULL */
                              hb_font_t *font, /* May be NULL */
                              hb_buffer_serialize_format_t format)
{
  மாறிலி எழுத்து *end;
  எனில் (!end_ptr)
    end_ptr = &end;
  *end_ptr = buf;

  buffer->assert_glyphs ();

  எனில் (unlikely (hb_object_is_immutable (buffer)))
  {
    எனில் (end_ptr)
      *end_ptr = buf;
    திரும்பு பொய்;
  }

  எனில் (buf_len == -1)
    buf_len = strlen (buf);

  எனில் (!buf_len)
  {
    *end_ptr = buf;
    திரும்பு பொய்;
  }

  எனில் (!font)
    font = hb_font_get_empty ();

  தேர்வு (format)
  {
    நிலை HB_BUFFER_SERIALIZE_FORMAT_TEXT:
      hb_buffer_set_content_type (buffer, HB_BUFFER_CONTENT_TYPE_GLYPHS);
      திரும்பு _hb_buffer_deserialize_text_glyphs (buffer,
						 buf, buf_len, end_ptr,
						 font);

    நிலை HB_BUFFER_SERIALIZE_FORMAT_JSON:
      hb_buffer_set_content_type (buffer, HB_BUFFER_CONTENT_TYPE_GLYPHS);
      திரும்பு _hb_buffer_deserialize_json (buffer,
                                          buf, buf_len, end_ptr,
                                          font);

    இயல்பு:
    நிலை HB_BUFFER_SERIALIZE_FORMAT_INVALID:
      திரும்பு பொய்;

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
hb_buffer_deserialize_unicode (hb_buffer_t *buffer,
                               மாறிலி எழுத்து *buf,
                               முழு buf_len, /* -1 means nul-terminated */
                               மாறிலி எழுத்து **end_ptr, /* May be NULL */
                               hb_buffer_serialize_format_t format)
{
  மாறிலி எழுத்து *end;
  எனில் (!end_ptr)
    end_ptr = &end;
  *end_ptr = buf;

  buffer->assert_unicode ();

  எனில் (unlikely (hb_object_is_immutable (buffer)))
  {
    எனில் (end_ptr)
      *end_ptr = buf;
    திரும்பு பொய்;
  }

  எனில் (buf_len == -1)
    buf_len = strlen (buf);

  எனில் (!buf_len)
  {
    *end_ptr = buf;
    திரும்பு பொய்;
  }

  hb_font_t* font = hb_font_get_empty ();

  தேர்வு (format)
  {
    நிலை HB_BUFFER_SERIALIZE_FORMAT_TEXT:
      hb_buffer_set_content_type (buffer, HB_BUFFER_CONTENT_TYPE_UNICODE);
      திரும்பு _hb_buffer_deserialize_text_unicode (buffer,
						  buf, buf_len, end_ptr,
						  font);

    நிலை HB_BUFFER_SERIALIZE_FORMAT_JSON:
      hb_buffer_set_content_type (buffer, HB_BUFFER_CONTENT_TYPE_UNICODE);
      திரும்பு _hb_buffer_deserialize_json (buffer,
                                          buf, buf_len, end_ptr,
                                          font);

    இயல்பு:
    நிலை HB_BUFFER_SERIALIZE_FORMAT_INVALID:
      திரும்பு பொய்;

  }
}


#endif
