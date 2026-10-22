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

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_బఫరు_SERIALIZE

#అన్తర్భావ "hb-buffer.hh"


స్థిర స్థిరరాశి అక్షరం *hb_బఫరు_serialize_formats[] = {
  "text",
  "json",
  శూన్యసూచిక
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
స్థిరరాశి అక్షరం **
hb_బఫరు_serialize_list_formats ()
{
  తిరిగి hb_బఫరు_serialize_formats;
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
hb_బఫరు_serialize_format_t
hb_బఫరు_serialize_format_from_string (స్థిరరాశి అక్షరం *str, పూర్ణాంకం len)
{
  /* Upper-case it. */
  తిరిగి (hb_బఫరు_serialize_format_t) (hb_ట్యాగ్_from_string (str, len) & ~0x20202020u);
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
స్థిరరాశి అక్షరం *
hb_బఫరు_serialize_format_గమ్యం_string (hb_బఫరు_serialize_format_t format)
{
  ఎంపిక ((గుర్తులేని) format)
  {
    స్థితి HB_బఫరు_SERIALIZE_FORMAT_పాఠ్యం: తిరిగి hb_బఫరు_serialize_formats[0];
    స్థితి HB_బఫరు_SERIALIZE_FORMAT_JSON: తిరిగి hb_బఫరు_serialize_formats[1];
    అప్రమేయ:
    స్థితి HB_బఫరు_SERIALIZE_FORMAT_INVALID:  తిరిగి శూన్యసూచిక;
  }
}

స్థిర గుర్తులేని పూర్ణాంకం
hb_బఫరు_serialize_glyphs_json (hb_బఫరు_t *బఫరు,
                                  గుర్తులేని పూర్ణాంకం ప్రారంభం,
                                  గుర్తులేని పూర్ణాంకం చివర,
                                  అక్షరం *buf,
                                  గుర్తులేని పూర్ణాంకం buf_కొలత,
                                  గుర్తులేని పూర్ణాంకం *buf_consumed,
                                  hb_font_t *font,
                                  hb_బఫరు_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_బఫరు_get_glyph_infos (బఫరు, శూన్యసూచిక);
  hb_glyph_position_t *pos = (flags & HB_బఫరు_SERIALIZE_FLAG_NO_POSITIONS) ?
                             శూన్యసూచిక : hb_బఫరు_get_glyph_positions (బఫరు, శూన్యసూచిక);

  *buf_consumed = 0;
  hb_position_t x = 0, y = 0;

  /* Calculate the advance of the previous glyphs */
  ఒకవేళ (pos && (flags & HB_బఫరు_SERIALIZE_FLAG_NO_ADVANCES))
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ప్రారంభం; i++)
    {
      x = hb_saturate_కూడు (x, pos[i].x_advance);
      y = hb_saturate_కూడు (y, pos[i].y_advance);
    }
  }

  కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర; i++)
  {
    అక్షరం b[1024];
    అక్షరం *p = b;

    /* In the following code, we know b is large enough that no overflow can happen. */

#నిర్వచించు APPEND(s) HB_STMT_ప్రారంభం { strcpy (p, s); p += అక్షరశ్రేణి_పొడవు (s); } HB_STMT_చివర

    ఒకవేళ (i)
      *p++ = ',';
    లేకపోతే
      *p++ = '[';

    *p++ = '{';

    APPEND ("\"g\":");
    ఒకవేళ (!(flags & HB_బఫరు_SERIALIZE_FLAG_NO_GLYPH_పేర్లు))
    {
      అక్షరం g[128];
      hb_font_glyph_గమ్యం_string (font, info[i].codepoint, g, పరిమాణం (g));
      *p++ = '"';
      కొరకు (అక్షరం *q = g; *q; q++)
      {
	ఒకవేళ (unlikely (*q == '"' || *q == '\\'))
	  *p++ = '\\';
	*p++ = *q;
      }
      *p++ = '"';
    }
    లేకపోతే
      p += hb_max (0, snprintf (p, ARRAY_నిడివి (b) - (p - b), "%u", info[i].codepoint));

    ఒకవేళ (!(flags & HB_బఫరు_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_నిడివి (b) - (p - b), ",\"cl\":%u", info[i].cluster));
    }

    ఒకవేళ (pos && !(flags & HB_బఫరు_SERIALIZE_FLAG_NO_POSITIONS))
    {
      hb_position_t dx = hb_saturate_కూడు (x, pos[i].x_స్థానభ్రంశం_2);
      hb_position_t dy = hb_saturate_కూడు (y, pos[i].y_స్థానభ్రంశం_2);
      p += hb_max (0, snprintf (p, ARRAY_నిడివి (b) - (p - b), ",\"dx\":%d,\"dy\":%d",
		   dx, dy));
      ఒకవేళ (!(flags & HB_బఫరు_SERIALIZE_FLAG_NO_ADVANCES))
        p += hb_max (0, snprintf (p, ARRAY_నిడివి (b) - (p - b), ",\"ax\":%d,\"ay\":%d",
		     pos[i].x_advance, pos[i].y_advance));
    }

    ఒకవేళ (flags & HB_బఫరు_SERIALIZE_FLAG_GLYPH_FLAGS)
    {
      ఒకవేళ (info[i].ముసుగు & HB_GLYPH_FLAG_DEFINED)
        p += hb_max (0, snprintf (p, ARRAY_నిడివి (b) - (p - b), ",\"fl\":%u", info[i].ముసుగు & HB_GLYPH_FLAG_DEFINED));
    }

    ఒకవేళ (flags & HB_బఫరు_SERIALIZE_FLAG_GLYPH_EXTENTS)
    {
      hb_glyph_extents_t extents;
      ఒకవేళ (hb_font_get_glyph_extents(font, info[i].codepoint, &extents))
      {
	p += hb_max (0, snprintf (p, ARRAY_నిడివి (b) - (p - b), ",\"xb\":%d,\"yb\":%d",
				  extents.x_bearing, extents.y_bearing));
	p += hb_max (0, snprintf (p, ARRAY_నిడివి (b) - (p - b), ",\"w\":%d,\"h\":%d",
				  extents.వెడల్పు, extents.ఎత్తు));
      }
    }

    *p++ = '}';
    ఒకవేళ (i == చివర-1)
      *p++ = ']';

    గుర్తులేని పూర్ణాంకం l = p - b;
    ఒకవేళ (buf_కొలత > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_కొలత -= l;
      *buf_consumed += l;
      *buf = '\0';
    } లేకపోతే
      తిరిగి i - ప్రారంభం;

    ఒకవేళ (pos && (flags & HB_బఫరు_SERIALIZE_FLAG_NO_ADVANCES))
    {
      x = hb_saturate_కూడు (x, pos[i].x_advance);
      y = hb_saturate_కూడు (y, pos[i].y_advance);
    }
  }

  తిరిగి చివర - ప్రారంభం;
}

స్థిర గుర్తులేని పూర్ణాంకం
hb_బఫరు_serialize_unicode_json (hb_బఫరు_t *బఫరు,
          గుర్తులేని పూర్ణాంకం ప్రారంభం,
          గుర్తులేని పూర్ణాంకం చివర,
          అక్షరం *buf,
          గుర్తులేని పూర్ణాంకం buf_కొలత,
          గుర్తులేని పూర్ణాంకం *buf_consumed,
          hb_బఫరు_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_బఫరు_get_glyph_infos (బఫరు, శూన్యసూచిక);

  *buf_consumed = 0;
  కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర; i++)
  {
    అక్షరం b[1024];
    అక్షరం *p = b;

    ఒకవేళ (i)
      *p++ = ',';
    లేకపోతే
      *p++ = '[';

    *p++ = '{';

    APPEND ("\"u\":");

    p += hb_max (0, snprintf (p, ARRAY_నిడివి (b) - (p - b), "%u", info[i].codepoint));

    ఒకవేళ (!(flags & HB_బఫరు_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_నిడివి (b) - (p - b), ",\"cl\":%u", info[i].cluster));
    }

    *p++ = '}';

    ఒకవేళ (i == చివర-1)
      *p++ = ']';

    గుర్తులేని పూర్ణాంకం l = p - b;
    ఒకవేళ (buf_కొలత > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_కొలత -= l;
      *buf_consumed += l;
      *buf = '\0';
    } లేకపోతే
      తిరిగి i - ప్రారంభం;

  }

  తిరిగి చివర - ప్రారంభం;
}

స్థిర గుర్తులేని పూర్ణాంకం
hb_బఫరు_serialize_glyphs_పాఠ్యం (hb_బఫరు_t *బఫరు,
                                  గుర్తులేని పూర్ణాంకం ప్రారంభం,
                                  గుర్తులేని పూర్ణాంకం చివర,
                                  అక్షరం *buf,
                                  గుర్తులేని పూర్ణాంకం buf_కొలత,
                                  గుర్తులేని పూర్ణాంకం *buf_consumed,
                                  hb_font_t *font,
                                  hb_బఫరు_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_బఫరు_get_glyph_infos (బఫరు, శూన్యసూచిక);
  hb_glyph_position_t *pos = (flags & HB_బఫరు_SERIALIZE_FLAG_NO_POSITIONS) ?
           శూన్యసూచిక : hb_బఫరు_get_glyph_positions (బఫరు, శూన్యసూచిక);

  *buf_consumed = 0;
  hb_position_t x = 0, y = 0;

  /* Calculate the advance of the previous glyphs */
  ఒకవేళ (pos && (flags & HB_బఫరు_SERIALIZE_FLAG_NO_ADVANCES))
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ప్రారంభం; i++)
    {
      x = hb_saturate_కూడు (x, pos[i].x_advance);
      y = hb_saturate_కూడు (y, pos[i].y_advance);
    }
  }

  కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర; i++)
  {
    అక్షరం b[1024];
    అక్షరం *p = b;

    /* In the following code, we know b is large enough that no overflow can happen. */

    ఒకవేళ (i)
      *p++ = '|';
    లేకపోతే
      *p++ = '[';

    ఒకవేళ (!(flags & HB_బఫరు_SERIALIZE_FLAG_NO_GLYPH_పేర్లు))
    {
      /* TODO Escape delimiters we use. */
      hb_font_glyph_గమ్యం_string (font, info[i].codepoint, p, 128);
      p += అక్షరశ్రేణి_పొడవు (p);
    }
    లేకపోతే
      p += hb_max (0, snprintf (p, ARRAY_నిడివి (b) - (p - b), "%u", info[i].codepoint));

    ఒకవేళ (!(flags & HB_బఫరు_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_నిడివి (b) - (p - b), "=%u", info[i].cluster));
    }

    ఒకవేళ (pos && !(flags & HB_బఫరు_SERIALIZE_FLAG_NO_POSITIONS))
    {
      hb_position_t dx = hb_saturate_కూడు (x, pos[i].x_స్థానభ్రంశం_2);
      hb_position_t dy = hb_saturate_కూడు (y, pos[i].y_స్థానభ్రంశం_2);
      ఒకవేళ (dx || dy)
        p += hb_max (0, snprintf (p, ARRAY_నిడివి (b) - (p - b), "@%d,%d", dx, dy));

      ఒకవేళ (!(flags & HB_బఫరు_SERIALIZE_FLAG_NO_ADVANCES))
      {
        *p++ = '+';
        p += hb_max (0, snprintf (p, ARRAY_నిడివి (b) - (p - b), "%d", pos[i].x_advance));
        ఒకవేళ (pos[i].y_advance)
          p += hb_max (0, snprintf (p, ARRAY_నిడివి (b) - (p - b), ",%d", pos[i].y_advance));
      }
    }

    ఒకవేళ (flags & HB_బఫరు_SERIALIZE_FLAG_GLYPH_FLAGS)
    {
      ఒకవేళ (info[i].ముసుగు & HB_GLYPH_FLAG_DEFINED)
        p += hb_max (0, snprintf (p, ARRAY_నిడివి (b) - (p - b), "#%X", info[i].ముసుగు &HB_GLYPH_FLAG_DEFINED));
    }

    ఒకవేళ (flags & HB_బఫరు_SERIALIZE_FLAG_GLYPH_EXTENTS)
    {
      hb_glyph_extents_t extents;
      ఒకవేళ (hb_font_get_glyph_extents(font, info[i].codepoint, &extents))
	p += hb_max (0, snprintf (p, ARRAY_నిడివి (b) - (p - b), "<%d,%d,%d,%d>", extents.x_bearing, extents.y_bearing, extents.వెడల్పు, extents.ఎత్తు));
    }

    ఒకవేళ (i == చివర-1) {
      *p++ = ']';
    }

    గుర్తులేని పూర్ణాంకం l = p - b;
    ఒకవేళ (buf_కొలత > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_కొలత -= l;
      *buf_consumed += l;
      *buf = '\0';
    } లేకపోతే
      తిరిగి i - ప్రారంభం;

    ఒకవేళ (pos && (flags & HB_బఫరు_SERIALIZE_FLAG_NO_ADVANCES))
    {
      x = hb_saturate_కూడు (x, pos[i].x_advance);
      y = hb_saturate_కూడు (y, pos[i].y_advance);
    }
  }

  తిరిగి చివర - ప్రారంభం;
}


స్థిర గుర్తులేని పూర్ణాంకం
hb_బఫరు_serialize_unicode_పాఠ్యం (hb_బఫరు_t *బఫరు,
                                   గుర్తులేని పూర్ణాంకం ప్రారంభం,
                                   గుర్తులేని పూర్ణాంకం చివర,
                                   అక్షరం *buf,
                                   గుర్తులేని పూర్ణాంకం buf_కొలత,
                                   గుర్తులేని పూర్ణాంకం *buf_consumed,
                                   hb_బఫరు_serialize_flags_t flags)
{
  hb_glyph_info_t *info = hb_బఫరు_get_glyph_infos (బఫరు, శూన్యసూచిక);
  *buf_consumed = 0;
  కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర; i++)
  {
    అక్షరం b[1024];
    అక్షరం *p = b;

    ఒకవేళ (i)
      *p++ = '|';
    లేకపోతే
      *p++ = '<';

    p += hb_max (0, snprintf (p, ARRAY_నిడివి (b) - (p - b), "U+%04X", info[i].codepoint));

    ఒకవేళ (!(flags & HB_బఫరు_SERIALIZE_FLAG_NO_CLUSTERS)) {
      p += hb_max (0, snprintf (p, ARRAY_నిడివి (b) - (p - b), "=%u", info[i].cluster));
    }

    ఒకవేళ (i == చివర-1)
      *p++ = '>';

    గుర్తులేని పూర్ణాంకం l = p - b;
    ఒకవేళ (buf_కొలత > l)
    {
      hb_memcpy (buf, b, l);
      buf += l;
      buf_కొలత -= l;
      *buf_consumed += l;
      *buf = '\0';
    } లేకపోతే
      తిరిగి i - ప్రారంభం;
  }
  తిరిగి చివర - ప్రారంభం;
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
గుర్తులేని పూర్ణాంకం
hb_బఫరు_serialize_glyphs (hb_బఫరు_t *బఫరు,
                            గుర్తులేని పూర్ణాంకం ప్రారంభం,
                            గుర్తులేని పూర్ణాంకం చివర,
                            అక్షరం *buf,
                            గుర్తులేని పూర్ణాంకం buf_కొలత,
                            గుర్తులేని పూర్ణాంకం *buf_consumed,
                            hb_font_t *font,
                            hb_బఫరు_serialize_format_t format,
                            hb_బఫరు_serialize_flags_t flags)
{
  చివర = hb_clamp (చివర, ప్రారంభం, బఫరు->len);
  ప్రారంభం = hb_min (ప్రారంభం, చివర);

  గుర్తులేని పూర్ణాంకం sconsumed;
  ఒకవేళ (!buf_consumed)
    buf_consumed = &sconsumed;
  *buf_consumed = 0;
  ఒకవేళ (buf_కొలత)
    *buf = '\0';

  బఫరు->assert_glyphs ();

  ఒకవేళ (!బఫరు->have_positions)
    flags |= HB_బఫరు_SERIALIZE_FLAG_NO_POSITIONS;

  ఒకవేళ (unlikely (ప్రారంభం == చివర))
    తిరిగి 0;

  ఒకవేళ (!font)
    font = hb_font_get_ఖాళీలు ();

  ఎంపిక (format)
  {
    స్థితి HB_బఫరు_SERIALIZE_FORMAT_పాఠ్యం:
      తిరిగి hb_బఫరు_serialize_glyphs_పాఠ్యం (బఫరు, ప్రారంభం, చివర,
                 buf, buf_కొలత, buf_consumed,
                 font, flags);

    స్థితి HB_బఫరు_SERIALIZE_FORMAT_JSON:
      తిరిగి hb_బఫరు_serialize_glyphs_json (బఫరు, ప్రారంభం, చివర,
                 buf, buf_కొలత, buf_consumed,
                 font, flags);

    అప్రమేయ:
    స్థితి HB_బఫరు_SERIALIZE_FORMAT_INVALID:
      తిరిగి 0;

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
గుర్తులేని పూర్ణాంకం
hb_బఫరు_serialize_unicode (hb_బఫరు_t *బఫరు,
                             గుర్తులేని పూర్ణాంకం ప్రారంభం,
                             గుర్తులేని పూర్ణాంకం చివర,
                             అక్షరం *buf,
                             గుర్తులేని పూర్ణాంకం buf_కొలత,
                             గుర్తులేని పూర్ణాంకం *buf_consumed,
                             hb_బఫరు_serialize_format_t format,
                             hb_బఫరు_serialize_flags_t flags)
{
  చివర = hb_clamp (చివర, ప్రారంభం, బఫరు->len);
  ప్రారంభం = hb_min (ప్రారంభం, చివర);

  గుర్తులేని పూర్ణాంకం sconsumed;
  ఒకవేళ (!buf_consumed)
    buf_consumed = &sconsumed;
  *buf_consumed = 0;
  ఒకవేళ (buf_కొలత)
    *buf = '\0';

  బఫరు->assert_unicode ();

  ఒకవేళ (unlikely (ప్రారంభం == చివర))
    తిరిగి 0;

  ఎంపిక (format)
  {
    స్థితి HB_బఫరు_SERIALIZE_FORMAT_పాఠ్యం:
      తిరిగి hb_బఫరు_serialize_unicode_పాఠ్యం (బఫరు, ప్రారంభం, చివర,
                                                buf, buf_కొలత, buf_consumed, flags);

    స్థితి HB_బఫరు_SERIALIZE_FORMAT_JSON:
      తిరిగి hb_బఫరు_serialize_unicode_json (బఫరు, ప్రారంభం, చివర,
                                                buf, buf_కొలత, buf_consumed, flags);

    అప్రమేయ:
    స్థితి HB_బఫరు_SERIALIZE_FORMAT_INVALID:
      తిరిగి 0;

  }
}

స్థిర గుర్తులేని పూర్ణాంకం
hb_బఫరు_serialize_invalid (hb_బఫరు_t *బఫరు,
                              గుర్తులేని పూర్ణాంకం ప్రారంభం,
                              గుర్తులేని పూర్ణాంకం చివర,
                              అక్షరం *buf,
                              గుర్తులేని పూర్ణాంకం buf_కొలత,
                              గుర్తులేని పూర్ణాంకం *buf_consumed,
                              hb_బఫరు_serialize_format_t format,
                              hb_బఫరు_serialize_flags_t flags)
{
  ప్రతిజ్ఞా (!బఫరు->len);

  గుర్తులేని పూర్ణాంకం sconsumed;
  ఒకవేళ (!buf_consumed)
    buf_consumed = &sconsumed;
  ఒకవేళ (buf_కొలత < 3)
    తిరిగి 0;
  ఒకవేళ (format == HB_బఫరు_SERIALIZE_FORMAT_JSON) {
    *buf++ = '[';
    *buf++ = ']';
    *buf = '\0';
  } లేకపోతే ఒకవేళ (format == HB_బఫరు_SERIALIZE_FORMAT_పాఠ్యం) {
    *buf++ = '!';
    *buf++ = '!';
    *buf = '\0';
  }
  *buf_consumed = 2;
  తిరిగి 0;
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
గుర్తులేని పూర్ణాంకం
hb_బఫరు_serialize (hb_బఫరు_t *బఫరు,
                     గుర్తులేని పూర్ణాంకం ప్రారంభం,
                     గుర్తులేని పూర్ణాంకం చివర,
                     అక్షరం *buf,
                     గుర్తులేని పూర్ణాంకం buf_కొలత,
                     గుర్తులేని పూర్ణాంకం *buf_consumed,
                     hb_font_t *font,
                     hb_బఫరు_serialize_format_t format,
                     hb_బఫరు_serialize_flags_t flags)
{
  ఎంపిక (బఫరు->content_type)
  {

    స్థితి HB_బఫరు_CONTENT_TYPE_GLYPHS:
      తిరిగి hb_బఫరు_serialize_glyphs (బఫరు, ప్రారంభం, చివర, buf, buf_కొలత,
					 buf_consumed, font, format, flags);

    స్థితి HB_బఫరు_CONTENT_TYPE_UNICODE:
      తిరిగి hb_బఫరు_serialize_unicode (బఫరు, ప్రారంభం, చివర, buf, buf_కొలత,
					  buf_consumed, format, flags);

    స్థితి HB_బఫరు_CONTENT_TYPE_INVALID:
    అప్రమేయ:
      తిరిగి hb_బఫరు_serialize_invalid (బఫరు, ప్రారంభం, చివర, buf, buf_కొలత,
					   buf_consumed, format, flags);
  }
}

స్థిర తార్కిక
parse_int (స్థిరరాశి అక్షరం *pp, స్థిరరాశి అక్షరం *చివర, int32_t *pv)
{
  పూర్ణాంకం v;
  స్థిరరాశి అక్షరం *p = pp;
  ఒకవేళ (unlikely (!hb_parse_int (&p, చివర, &v, నిజం/* whole buffer */)))
    తిరిగి అబద్ధం;

  *pv = v;
  తిరిగి నిజం;
}

స్థిర తార్కిక
parse_uint (స్థిరరాశి అక్షరం *pp, స్థిరరాశి అక్షరం *చివర, uint32_t *pv)
{
  గుర్తులేని పూర్ణాంకం v;
  స్థిరరాశి అక్షరం *p = pp;
  ఒకవేళ (unlikely (!hb_parse_uint (&p, చివర, &v, నిజం/* whole buffer */)))
    తిరిగి అబద్ధం;

  *pv = v;
  తిరిగి నిజం;
}

స్థిర తార్కిక
parse_hex (స్థిరరాశి అక్షరం *pp, స్థిరరాశి అక్షరం *చివర, uint32_t *pv)
{
  గుర్తులేని పూర్ణాంకం v;
  స్థిరరాశి అక్షరం *p = pp;
  ఒకవేళ (unlikely (!hb_parse_uint (&p, చివర, &v, నిజం/* whole buffer */, 16)))
    తిరిగి అబద్ధం;

  *pv = v;
  తిరిగి నిజం;
}

#అన్తర్భావ "hb-buffer-deserialize-json.hh"
#అన్తర్భావ "hb-buffer-deserialize-text-glyphs.hh"
#అన్తర్భావ "hb-buffer-deserialize-text-unicode.hh"

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
hb_బఫరు_deserialize_glyphs (hb_బఫరు_t *బఫరు,
                              స్థిరరాశి అక్షరం *buf,
                              పూర్ణాంకం buf_len, /* -1 means nul-terminated */
                              స్థిరరాశి అక్షరం **end_ptr, /* May be NULL */
                              hb_font_t *font, /* May be NULL */
                              hb_బఫరు_serialize_format_t format)
{
  స్థిరరాశి అక్షరం *చివర;
  ఒకవేళ (!end_ptr)
    end_ptr = &చివర;
  *end_ptr = buf;

  బఫరు->assert_glyphs ();

  ఒకవేళ (unlikely (hb_object_is_immutable (బఫరు)))
  {
    ఒకవేళ (end_ptr)
      *end_ptr = buf;
    తిరిగి అబద్ధం;
  }

  ఒకవేళ (buf_len == -1)
    buf_len = అక్షరశ్రేణి_పొడవు (buf);

  ఒకవేళ (!buf_len)
  {
    *end_ptr = buf;
    తిరిగి అబద్ధం;
  }

  ఒకవేళ (!font)
    font = hb_font_get_ఖాళీలు ();

  ఎంపిక (format)
  {
    స్థితి HB_బఫరు_SERIALIZE_FORMAT_పాఠ్యం:
      hb_బఫరు_సమితి_content_type (బఫరు, HB_బఫరు_CONTENT_TYPE_GLYPHS);
      తిరిగి hb_బఫరు_deserialize_పాఠ్యం_glyphs (బఫరు,
						 buf, buf_len, end_ptr,
						 font);

    స్థితి HB_బఫరు_SERIALIZE_FORMAT_JSON:
      hb_బఫరు_సమితి_content_type (బఫరు, HB_బఫరు_CONTENT_TYPE_GLYPHS);
      తిరిగి hb_బఫరు_deserialize_json (బఫరు,
                                          buf, buf_len, end_ptr,
                                          font);

    అప్రమేయ:
    స్థితి HB_బఫరు_SERIALIZE_FORMAT_INVALID:
      తిరిగి అబద్ధం;

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
hb_బఫరు_deserialize_unicode (hb_బఫరు_t *బఫరు,
                               స్థిరరాశి అక్షరం *buf,
                               పూర్ణాంకం buf_len, /* -1 means nul-terminated */
                               స్థిరరాశి అక్షరం **end_ptr, /* May be NULL */
                               hb_బఫరు_serialize_format_t format)
{
  స్థిరరాశి అక్షరం *చివర;
  ఒకవేళ (!end_ptr)
    end_ptr = &చివర;
  *end_ptr = buf;

  బఫరు->assert_unicode ();

  ఒకవేళ (unlikely (hb_object_is_immutable (బఫరు)))
  {
    ఒకవేళ (end_ptr)
      *end_ptr = buf;
    తిరిగి అబద్ధం;
  }

  ఒకవేళ (buf_len == -1)
    buf_len = అక్షరశ్రేణి_పొడవు (buf);

  ఒకవేళ (!buf_len)
  {
    *end_ptr = buf;
    తిరిగి అబద్ధం;
  }

  hb_font_t* font = hb_font_get_ఖాళీలు ();

  ఎంపిక (format)
  {
    స్థితి HB_బఫరు_SERIALIZE_FORMAT_పాఠ్యం:
      hb_బఫరు_సమితి_content_type (బఫరు, HB_బఫరు_CONTENT_TYPE_UNICODE);
      తిరిగి hb_బఫరు_deserialize_పాఠ్యం_unicode (బఫరు,
						  buf, buf_len, end_ptr,
						  font);

    స్థితి HB_బఫరు_SERIALIZE_FORMAT_JSON:
      hb_బఫరు_సమితి_content_type (బఫరు, HB_బఫరు_CONTENT_TYPE_UNICODE);
      తిరిగి hb_బఫరు_deserialize_json (బఫరు,
                                          buf, buf_len, end_ptr,
                                          font);

    అప్రమేయ:
    స్థితి HB_బఫరు_SERIALIZE_FORMAT_INVALID:
      తిరిగి అబద్ధం;

  }
}


#అయితే_ముగింపు
