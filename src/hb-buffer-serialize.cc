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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_ਬਫਰ_SERIALIZE

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-buffer.hh"


ਸਥਿਰ ਅਚਲ ਅੱਖਰ *hb_ਬਫਰ_serialize_formats[] = {
  "text",
  "json",
  ਖਾਲੀ_ਸੂਚਕ
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
ਅਚਲ ਅੱਖਰ **
hb_ਬਫਰ_serialize_list_formats ()
{
  ਵਾਪਸ hb_ਬਫਰ_serialize_formats;
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
hb_ਬਫਰ_serialize_format_t
hb_ਬਫਰ_serialize_format_from_string (ਅਚਲ ਅੱਖਰ *str, ਪੂਰਨ len)
{
  /* Upper-case it. */
  ਵਾਪਸ (hb_ਬਫਰ_serialize_format_t) (hb_ਟੈਗ_from_string (str, len) & ~0x20202020u);
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
ਅਚਲ ਅੱਖਰ *
hb_ਬਫਰ_serialize_format_ਮੰਜ਼ਿਲ_string (hb_ਬਫਰ_serialize_format_t format)
{
  ਚੋਣ ((ਬਿਨਾਨਿਸ਼ਾਨੀ) format)
  {
    ਹਾਲਤ HB_ਬਫਰ_SERIALIZE_FORMAT_ਪਾਠ: ਵਾਪਸ hb_ਬਫਰ_serialize_formats[0];
    ਹਾਲਤ HB_ਬਫਰ_SERIALIZE_FORMAT_JSON: ਵਾਪਸ hb_ਬਫਰ_serialize_formats[1];
    ਮੂਲ:
    ਹਾਲਤ HB_ਬਫਰ_SERIALIZE_FORMAT_INVALID:  ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ਬਫਰ_serialize_glyphs_json (hb_ਬਫਰ_t *ਬਫਰ,
                                  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ,
                                  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ,
                                  ਅੱਖਰ *buf,
                                  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ buf_ਮਾਪ,
                                  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *buf_consumed,
                                  hb_font_t *font,
                                  hb_ਬਫਰ_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_ਬਫਰ_get_glyph_infos (ਬਫਰ, ਖਾਲੀ_ਸੂਚਕ);
  hb_glyph_position_t *pos = (flags & HB_ਬਫਰ_SERIALIZE_FLAG_NO_POSITIONS) ?
                             ਖਾਲੀ_ਸੂਚਕ : hb_ਬਫਰ_get_glyph_positions (ਬਫਰ, ਖਾਲੀ_ਸੂਚਕ);

  *buf_consumed = 0;
  hb_position_t x = 0, y = 0;

  /* Calculate the advance of the previous glyphs */
  ਜੇ (pos && (flags & HB_ਬਫਰ_SERIALIZE_FLAG_NO_ADVANCES))
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਸ਼ੁਰੂ; i++)
    {
      x = hb_saturate_ਜੋੜੋ (x, pos[i].x_advance);
      y = hb_saturate_ਜੋੜੋ (y, pos[i].y_advance);
    }
  }

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
  {
    ਅੱਖਰ b[1024];
    ਅੱਖਰ *p = b;

    /* In the following code, we know b is large enough that no overflow can happen. */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ APPEND(s) HB_STMT_ਸ਼ੁਰੂ { strcpy (p, s); p += ਲੜੀ_ਲੰਬਾਈ (s); } HB_STMT_ਸਿਰਾ

    ਜੇ (i)
      *p++ = ',';
    ਨਹੀਂਤਾਂ
      *p++ = '[';

    *p++ = '{';

    APPEND ("\"g\":");
    ਜੇ (!(flags & HB_ਬਫਰ_SERIALIZE_FLAG_NO_GLYPH_ਨਾਂ))
    {
      ਅੱਖਰ g[128];
      hb_font_glyph_ਮੰਜ਼ਿਲ_string (font, info[i].codepoint, g, ਆਕਾਰ (g));
      *p++ = '"';
      ਲਈ (ਅੱਖਰ *q = g; *q; q++)
      {
	ਜੇ (unlikely (*q == '"' || *q == '\\'))
	  *p++ = '\\';
	*p++ = *q;
      }
      *p++ = '"';
    }
    ਨਹੀਂਤਾਂ
      p += hb_max (0, snprintf (p, ARRAY_ਲੰਮਾਈ (b) - (p - b), "%u", info[i].codepoint));

    ਜੇ (!(flags & HB_ਬਫਰ_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_ਲੰਮਾਈ (b) - (p - b), ",\"cl\":%u", info[i].cluster));
    }

    ਜੇ (pos && !(flags & HB_ਬਫਰ_SERIALIZE_FLAG_NO_POSITIONS))
    {
      hb_position_t dx = hb_saturate_ਜੋੜੋ (x, pos[i].x_ਵਿੱਥ_2);
      hb_position_t dy = hb_saturate_ਜੋੜੋ (y, pos[i].y_ਵਿੱਥ_2);
      p += hb_max (0, snprintf (p, ARRAY_ਲੰਮਾਈ (b) - (p - b), ",\"dx\":%d,\"dy\":%d",
		   dx, dy));
      ਜੇ (!(flags & HB_ਬਫਰ_SERIALIZE_FLAG_NO_ADVANCES))
        p += hb_max (0, snprintf (p, ARRAY_ਲੰਮਾਈ (b) - (p - b), ",\"ax\":%d,\"ay\":%d",
		     pos[i].x_advance, pos[i].y_advance));
    }

    ਜੇ (flags & HB_ਬਫਰ_SERIALIZE_FLAG_GLYPH_FLAGS)
    {
      ਜੇ (info[i].ਨਕਾਬ & HB_GLYPH_FLAG_DEFINED)
        p += hb_max (0, snprintf (p, ARRAY_ਲੰਮਾਈ (b) - (p - b), ",\"fl\":%u", info[i].ਨਕਾਬ & HB_GLYPH_FLAG_DEFINED));
    }

    ਜੇ (flags & HB_ਬਫਰ_SERIALIZE_FLAG_GLYPH_EXTENTS)
    {
      hb_glyph_extents_t extents;
      ਜੇ (hb_font_get_glyph_extents(font, info[i].codepoint, &extents))
      {
	p += hb_max (0, snprintf (p, ARRAY_ਲੰਮਾਈ (b) - (p - b), ",\"xb\":%d,\"yb\":%d",
				  extents.x_bearing, extents.y_bearing));
	p += hb_max (0, snprintf (p, ARRAY_ਲੰਮਾਈ (b) - (p - b), ",\"w\":%d,\"h\":%d",
				  extents.ਚੌੜਾਈ, extents.ਉਚਾਈ));
      }
    }

    *p++ = '}';
    ਜੇ (i == ਸਿਰਾ-1)
      *p++ = ']';

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ l = p - b;
    ਜੇ (buf_ਮਾਪ > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_ਮਾਪ -= l;
      *buf_consumed += l;
      *buf = '\0';
    } ਨਹੀਂਤਾਂ
      ਵਾਪਸ i - ਸ਼ੁਰੂ;

    ਜੇ (pos && (flags & HB_ਬਫਰ_SERIALIZE_FLAG_NO_ADVANCES))
    {
      x = hb_saturate_ਜੋੜੋ (x, pos[i].x_advance);
      y = hb_saturate_ਜੋੜੋ (y, pos[i].y_advance);
    }
  }

  ਵਾਪਸ ਸਿਰਾ - ਸ਼ੁਰੂ;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ਬਫਰ_serialize_unicode_json (hb_ਬਫਰ_t *ਬਫਰ,
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ,
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ,
          ਅੱਖਰ *buf,
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ buf_ਮਾਪ,
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *buf_consumed,
          hb_ਬਫਰ_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_ਬਫਰ_get_glyph_infos (ਬਫਰ, ਖਾਲੀ_ਸੂਚਕ);

  *buf_consumed = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
  {
    ਅੱਖਰ b[1024];
    ਅੱਖਰ *p = b;

    ਜੇ (i)
      *p++ = ',';
    ਨਹੀਂਤਾਂ
      *p++ = '[';

    *p++ = '{';

    APPEND ("\"u\":");

    p += hb_max (0, snprintf (p, ARRAY_ਲੰਮਾਈ (b) - (p - b), "%u", info[i].codepoint));

    ਜੇ (!(flags & HB_ਬਫਰ_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_ਲੰਮਾਈ (b) - (p - b), ",\"cl\":%u", info[i].cluster));
    }

    *p++ = '}';

    ਜੇ (i == ਸਿਰਾ-1)
      *p++ = ']';

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ l = p - b;
    ਜੇ (buf_ਮਾਪ > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_ਮਾਪ -= l;
      *buf_consumed += l;
      *buf = '\0';
    } ਨਹੀਂਤਾਂ
      ਵਾਪਸ i - ਸ਼ੁਰੂ;

  }

  ਵਾਪਸ ਸਿਰਾ - ਸ਼ੁਰੂ;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ਬਫਰ_serialize_glyphs_ਪਾਠ (hb_ਬਫਰ_t *ਬਫਰ,
                                  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ,
                                  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ,
                                  ਅੱਖਰ *buf,
                                  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ buf_ਮਾਪ,
                                  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *buf_consumed,
                                  hb_font_t *font,
                                  hb_ਬਫਰ_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_ਬਫਰ_get_glyph_infos (ਬਫਰ, ਖਾਲੀ_ਸੂਚਕ);
  hb_glyph_position_t *pos = (flags & HB_ਬਫਰ_SERIALIZE_FLAG_NO_POSITIONS) ?
           ਖਾਲੀ_ਸੂਚਕ : hb_ਬਫਰ_get_glyph_positions (ਬਫਰ, ਖਾਲੀ_ਸੂਚਕ);

  *buf_consumed = 0;
  hb_position_t x = 0, y = 0;

  /* Calculate the advance of the previous glyphs */
  ਜੇ (pos && (flags & HB_ਬਫਰ_SERIALIZE_FLAG_NO_ADVANCES))
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਸ਼ੁਰੂ; i++)
    {
      x = hb_saturate_ਜੋੜੋ (x, pos[i].x_advance);
      y = hb_saturate_ਜੋੜੋ (y, pos[i].y_advance);
    }
  }

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
  {
    ਅੱਖਰ b[1024];
    ਅੱਖਰ *p = b;

    /* In the following code, we know b is large enough that no overflow can happen. */

    ਜੇ (i)
      *p++ = '|';
    ਨਹੀਂਤਾਂ
      *p++ = '[';

    ਜੇ (!(flags & HB_ਬਫਰ_SERIALIZE_FLAG_NO_GLYPH_ਨਾਂ))
    {
      /* TODO Escape delimiters we use. */
      hb_font_glyph_ਮੰਜ਼ਿਲ_string (font, info[i].codepoint, p, 128);
      p += ਲੜੀ_ਲੰਬਾਈ (p);
    }
    ਨਹੀਂਤਾਂ
      p += hb_max (0, snprintf (p, ARRAY_ਲੰਮਾਈ (b) - (p - b), "%u", info[i].codepoint));

    ਜੇ (!(flags & HB_ਬਫਰ_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_ਲੰਮਾਈ (b) - (p - b), "=%u", info[i].cluster));
    }

    ਜੇ (pos && !(flags & HB_ਬਫਰ_SERIALIZE_FLAG_NO_POSITIONS))
    {
      hb_position_t dx = hb_saturate_ਜੋੜੋ (x, pos[i].x_ਵਿੱਥ_2);
      hb_position_t dy = hb_saturate_ਜੋੜੋ (y, pos[i].y_ਵਿੱਥ_2);
      ਜੇ (dx || dy)
        p += hb_max (0, snprintf (p, ARRAY_ਲੰਮਾਈ (b) - (p - b), "@%d,%d", dx, dy));

      ਜੇ (!(flags & HB_ਬਫਰ_SERIALIZE_FLAG_NO_ADVANCES))
      {
        *p++ = '+';
        p += hb_max (0, snprintf (p, ARRAY_ਲੰਮਾਈ (b) - (p - b), "%d", pos[i].x_advance));
        ਜੇ (pos[i].y_advance)
          p += hb_max (0, snprintf (p, ARRAY_ਲੰਮਾਈ (b) - (p - b), ",%d", pos[i].y_advance));
      }
    }

    ਜੇ (flags & HB_ਬਫਰ_SERIALIZE_FLAG_GLYPH_FLAGS)
    {
      ਜੇ (info[i].ਨਕਾਬ & HB_GLYPH_FLAG_DEFINED)
        p += hb_max (0, snprintf (p, ARRAY_ਲੰਮਾਈ (b) - (p - b), "#%X", info[i].ਨਕਾਬ &HB_GLYPH_FLAG_DEFINED));
    }

    ਜੇ (flags & HB_ਬਫਰ_SERIALIZE_FLAG_GLYPH_EXTENTS)
    {
      hb_glyph_extents_t extents;
      ਜੇ (hb_font_get_glyph_extents(font, info[i].codepoint, &extents))
	p += hb_max (0, snprintf (p, ARRAY_ਲੰਮਾਈ (b) - (p - b), "<%d,%d,%d,%d>", extents.x_bearing, extents.y_bearing, extents.ਚੌੜਾਈ, extents.ਉਚਾਈ));
    }

    ਜੇ (i == ਸਿਰਾ-1) {
      *p++ = ']';
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ l = p - b;
    ਜੇ (buf_ਮਾਪ > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_ਮਾਪ -= l;
      *buf_consumed += l;
      *buf = '\0';
    } ਨਹੀਂਤਾਂ
      ਵਾਪਸ i - ਸ਼ੁਰੂ;

    ਜੇ (pos && (flags & HB_ਬਫਰ_SERIALIZE_FLAG_NO_ADVANCES))
    {
      x = hb_saturate_ਜੋੜੋ (x, pos[i].x_advance);
      y = hb_saturate_ਜੋੜੋ (y, pos[i].y_advance);
    }
  }

  ਵਾਪਸ ਸਿਰਾ - ਸ਼ੁਰੂ;
}


ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ਬਫਰ_serialize_unicode_ਪਾਠ (hb_ਬਫਰ_t *ਬਫਰ,
                                   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ,
                                   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ,
                                   ਅੱਖਰ *buf,
                                   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ buf_ਮਾਪ,
                                   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *buf_consumed,
                                   hb_ਬਫਰ_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_ਬਫਰ_get_glyph_infos (ਬਫਰ, ਖਾਲੀ_ਸੂਚਕ);
  *buf_consumed = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
  {
    ਅੱਖਰ b[1024];
    ਅੱਖਰ *p = b;

    ਜੇ (i)
      *p++ = '|';
    ਨਹੀਂਤਾਂ
      *p++ = '<';

    p += hb_max (0, snprintf (p, ARRAY_ਲੰਮਾਈ (b) - (p - b), "U+%04X", info[i].codepoint));

    ਜੇ (!(flags & HB_ਬਫਰ_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_ਲੰਮਾਈ (b) - (p - b), "=%u", info[i].cluster));
    }

    ਜੇ (i == ਸਿਰਾ-1)
      *p++ = '>';

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ l = p - b;
    ਜੇ (buf_ਮਾਪ > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_ਮਾਪ -= l;
      *buf_consumed += l;
      *buf = '\0';
    } ਨਹੀਂਤਾਂ
      ਵਾਪਸ i - ਸ਼ੁਰੂ;
  }
  ਵਾਪਸ ਸਿਰਾ - ਸ਼ੁਰੂ;
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ਬਫਰ_serialize_glyphs (hb_ਬਫਰ_t *ਬਫਰ,
                            ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ,
                            ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ,
                            ਅੱਖਰ *buf,
                            ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ buf_ਮਾਪ,
                            ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *buf_consumed,
                            hb_font_t *font,
                            hb_ਬਫਰ_serialize_format_t format,
                            hb_ਬਫਰ_serialize_flags_t flags)
{
  ਸਿਰਾ = hb_clamp (ਸਿਰਾ, ਸ਼ੁਰੂ, ਬਫਰ->len);
  ਸ਼ੁਰੂ = hb_min (ਸ਼ੁਰੂ, ਸਿਰਾ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sconsumed;
  ਜੇ (!buf_consumed)
    buf_consumed = &sconsumed;
  *buf_consumed = 0;
  ਜੇ (buf_ਮਾਪ)
    *buf = '\0';

  ਬਫਰ->assert_glyphs ();

  ਜੇ (!ਬਫਰ->have_positions)
    flags |= HB_ਬਫਰ_SERIALIZE_FLAG_NO_POSITIONS;

  ਜੇ (unlikely (ਸ਼ੁਰੂ == ਸਿਰਾ))
    ਵਾਪਸ 0;

  ਜੇ (!font)
    font = hb_font_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  ਚੋਣ (format)
  {
    ਹਾਲਤ HB_ਬਫਰ_SERIALIZE_FORMAT_ਪਾਠ:
      ਵਾਪਸ hb_ਬਫਰ_serialize_glyphs_ਪਾਠ (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ,
                 buf, buf_ਮਾਪ, buf_consumed,
                 font, flags);

    ਹਾਲਤ HB_ਬਫਰ_SERIALIZE_FORMAT_JSON:
      ਵਾਪਸ hb_ਬਫਰ_serialize_glyphs_json (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ,
                 buf, buf_ਮਾਪ, buf_consumed,
                 font, flags);

    ਮੂਲ:
    ਹਾਲਤ HB_ਬਫਰ_SERIALIZE_FORMAT_INVALID:
      ਵਾਪਸ 0;

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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ਬਫਰ_serialize_unicode (hb_ਬਫਰ_t *ਬਫਰ,
                             ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ,
                             ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ,
                             ਅੱਖਰ *buf,
                             ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ buf_ਮਾਪ,
                             ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *buf_consumed,
                             hb_ਬਫਰ_serialize_format_t format,
                             hb_ਬਫਰ_serialize_flags_t flags)
{
  ਸਿਰਾ = hb_clamp (ਸਿਰਾ, ਸ਼ੁਰੂ, ਬਫਰ->len);
  ਸ਼ੁਰੂ = hb_min (ਸ਼ੁਰੂ, ਸਿਰਾ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sconsumed;
  ਜੇ (!buf_consumed)
    buf_consumed = &sconsumed;
  *buf_consumed = 0;
  ਜੇ (buf_ਮਾਪ)
    *buf = '\0';

  ਬਫਰ->assert_unicode ();

  ਜੇ (unlikely (ਸ਼ੁਰੂ == ਸਿਰਾ))
    ਵਾਪਸ 0;

  ਚੋਣ (format)
  {
    ਹਾਲਤ HB_ਬਫਰ_SERIALIZE_FORMAT_ਪਾਠ:
      ਵਾਪਸ hb_ਬਫਰ_serialize_unicode_ਪਾਠ (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ,
                                                buf, buf_ਮਾਪ, buf_consumed, flags);

    ਹਾਲਤ HB_ਬਫਰ_SERIALIZE_FORMAT_JSON:
      ਵਾਪਸ hb_ਬਫਰ_serialize_unicode_json (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ,
                                                buf, buf_ਮਾਪ, buf_consumed, flags);

    ਮੂਲ:
    ਹਾਲਤ HB_ਬਫਰ_SERIALIZE_FORMAT_INVALID:
      ਵਾਪਸ 0;

  }
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ਬਫਰ_serialize_invalid (hb_ਬਫਰ_t *ਬਫਰ,
                              ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ,
                              ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ,
                              ਅੱਖਰ *buf,
                              ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ buf_ਮਾਪ,
                              ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *buf_consumed,
                              hb_ਬਫਰ_serialize_format_t format,
                              hb_ਬਫਰ_serialize_flags_t flags)
{
  ਪ੍ਰਤਿਜ੍ਞਾ (!ਬਫਰ->len);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sconsumed;
  ਜੇ (!buf_consumed)
    buf_consumed = &sconsumed;
  ਜੇ (buf_ਮਾਪ < 3)
    ਵਾਪਸ 0;
  ਜੇ (format == HB_ਬਫਰ_SERIALIZE_FORMAT_JSON) {
    *buf++ = '[';
    *buf++ = ']';
    *buf = '\0';
  } ਨਹੀਂਤਾਂ ਜੇ (format == HB_ਬਫਰ_SERIALIZE_FORMAT_ਪਾਠ) {
    *buf++ = '!';
    *buf++ = '!';
    *buf = '\0';
  }
  *buf_consumed = 2;
  ਵਾਪਸ 0;
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ਬਫਰ_serialize (hb_ਬਫਰ_t *ਬਫਰ,
                     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ,
                     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ,
                     ਅੱਖਰ *buf,
                     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ buf_ਮਾਪ,
                     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *buf_consumed,
                     hb_font_t *font,
                     hb_ਬਫਰ_serialize_format_t format,
                     hb_ਬਫਰ_serialize_flags_t flags)
{
  ਚੋਣ (ਬਫਰ->content_type)
  {

    ਹਾਲਤ HB_ਬਫਰ_CONTENT_TYPE_GLYPHS:
      ਵਾਪਸ hb_ਬਫਰ_serialize_glyphs (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ, buf, buf_ਮਾਪ,
					 buf_consumed, font, format, flags);

    ਹਾਲਤ HB_ਬਫਰ_CONTENT_TYPE_UNICODE:
      ਵਾਪਸ hb_ਬਫਰ_serialize_unicode (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ, buf, buf_ਮਾਪ,
					  buf_consumed, format, flags);

    ਹਾਲਤ HB_ਬਫਰ_CONTENT_TYPE_INVALID:
    ਮੂਲ:
      ਵਾਪਸ hb_ਬਫਰ_serialize_invalid (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ, buf, buf_ਮਾਪ,
					   buf_consumed, format, flags);
  }
}

ਸਥਿਰ ਤਾਰਕਿਕ
parse_int (ਅਚਲ ਅੱਖਰ *pp, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ, int32_t *pv)
{
  ਪੂਰਨ v;
  ਅਚਲ ਅੱਖਰ *p = pp;
  ਜੇ (unlikely (!hb_parse_int (&p, ਸਿਰਾ, &v, ਸੱਚ/* whole buffer */)))
    ਵਾਪਸ ਝੂਠ;

  *pv = v;
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ
parse_uint (ਅਚਲ ਅੱਖਰ *pp, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ, uint32_t *pv)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ v;
  ਅਚਲ ਅੱਖਰ *p = pp;
  ਜੇ (unlikely (!hb_parse_uint (&p, ਸਿਰਾ, &v, ਸੱਚ/* whole buffer */)))
    ਵਾਪਸ ਝੂਠ;

  *pv = v;
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ
parse_hex (ਅਚਲ ਅੱਖਰ *pp, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ, uint32_t *pv)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ v;
  ਅਚਲ ਅੱਖਰ *p = pp;
  ਜੇ (unlikely (!hb_parse_uint (&p, ਸਿਰਾ, &v, ਸੱਚ/* whole buffer */, 16)))
    ਵਾਪਸ ਝੂਠ;

  *pv = v;
  ਵਾਪਸ ਸੱਚ;
}

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-buffer-deserialize-json.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-buffer-deserialize-text-glyphs.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-buffer-deserialize-text-unicode.hh"

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
hb_ਬਫਰ_deserialize_glyphs (hb_ਬਫਰ_t *ਬਫਰ,
                              ਅਚਲ ਅੱਖਰ *buf,
                              ਪੂਰਨ buf_len, /* -1 means nul-terminated */
                              ਅਚਲ ਅੱਖਰ **end_ptr, /* May be NULL */
                              hb_font_t *font, /* May be NULL */
                              hb_ਬਫਰ_serialize_format_t format)
{
  ਅਚਲ ਅੱਖਰ *ਸਿਰਾ;
  ਜੇ (!end_ptr)
    end_ptr = &ਸਿਰਾ;
  *end_ptr = buf;

  ਬਫਰ->assert_glyphs ();

  ਜੇ (unlikely (hb_object_is_immutable (ਬਫਰ)))
  {
    ਜੇ (end_ptr)
      *end_ptr = buf;
    ਵਾਪਸ ਝੂਠ;
  }

  ਜੇ (buf_len == -1)
    buf_len = ਲੜੀ_ਲੰਬਾਈ (buf);

  ਜੇ (!buf_len)
  {
    *end_ptr = buf;
    ਵਾਪਸ ਝੂਠ;
  }

  ਜੇ (!font)
    font = hb_font_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  ਚੋਣ (format)
  {
    ਹਾਲਤ HB_ਬਫਰ_SERIALIZE_FORMAT_ਪਾਠ:
      hb_ਬਫਰ_ਸੈੱਟ_content_type (ਬਫਰ, HB_ਬਫਰ_CONTENT_TYPE_GLYPHS);
      ਵਾਪਸ hb_ਬਫਰ_deserialize_ਪਾਠ_glyphs (ਬਫਰ,
						 buf, buf_len, end_ptr,
						 font);

    ਹਾਲਤ HB_ਬਫਰ_SERIALIZE_FORMAT_JSON:
      hb_ਬਫਰ_ਸੈੱਟ_content_type (ਬਫਰ, HB_ਬਫਰ_CONTENT_TYPE_GLYPHS);
      ਵਾਪਸ hb_ਬਫਰ_deserialize_json (ਬਫਰ,
                                          buf, buf_len, end_ptr,
                                          font);

    ਮੂਲ:
    ਹਾਲਤ HB_ਬਫਰ_SERIALIZE_FORMAT_INVALID:
      ਵਾਪਸ ਝੂਠ;

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
hb_ਬਫਰ_deserialize_unicode (hb_ਬਫਰ_t *ਬਫਰ,
                               ਅਚਲ ਅੱਖਰ *buf,
                               ਪੂਰਨ buf_len, /* -1 means nul-terminated */
                               ਅਚਲ ਅੱਖਰ **end_ptr, /* May be NULL */
                               hb_ਬਫਰ_serialize_format_t format)
{
  ਅਚਲ ਅੱਖਰ *ਸਿਰਾ;
  ਜੇ (!end_ptr)
    end_ptr = &ਸਿਰਾ;
  *end_ptr = buf;

  ਬਫਰ->assert_unicode ();

  ਜੇ (unlikely (hb_object_is_immutable (ਬਫਰ)))
  {
    ਜੇ (end_ptr)
      *end_ptr = buf;
    ਵਾਪਸ ਝੂਠ;
  }

  ਜੇ (buf_len == -1)
    buf_len = ਲੜੀ_ਲੰਬਾਈ (buf);

  ਜੇ (!buf_len)
  {
    *end_ptr = buf;
    ਵਾਪਸ ਝੂਠ;
  }

  hb_font_t* font = hb_font_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  ਚੋਣ (format)
  {
    ਹਾਲਤ HB_ਬਫਰ_SERIALIZE_FORMAT_ਪਾਠ:
      hb_ਬਫਰ_ਸੈੱਟ_content_type (ਬਫਰ, HB_ਬਫਰ_CONTENT_TYPE_UNICODE);
      ਵਾਪਸ hb_ਬਫਰ_deserialize_ਪਾਠ_unicode (ਬਫਰ,
						  buf, buf_len, end_ptr,
						  font);

    ਹਾਲਤ HB_ਬਫਰ_SERIALIZE_FORMAT_JSON:
      hb_ਬਫਰ_ਸੈੱਟ_content_type (ਬਫਰ, HB_ਬਫਰ_CONTENT_TYPE_UNICODE);
      ਵਾਪਸ hb_ਬਫਰ_deserialize_json (ਬਫਰ,
                                          buf, buf_len, end_ptr,
                                          font);

    ਮੂਲ:
    ਹਾਲਤ HB_ਬਫਰ_SERIALIZE_FORMAT_INVALID:
      ਵਾਪਸ ਝੂਠ;

  }
}


#ਜੇ_ਅੰਤ
