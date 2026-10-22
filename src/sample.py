#!/usr/bin/env python3

درآمد sys
درآمد array
درآمد gi
gi.require_نسخہ('HarfBuzz', '0.0')
سے gi.repository درآمد HarfBuzz بطور hb
سے gi.repository درآمد GLib

fontdata = open (sys.argv[1], 'rb').اقرأ ()
متن = sys.argv[2]
# Need to create GLib.Bytes explicitly until this bug is fixed:
# https://bugzilla.gnome.org/show_bug.cgi?id=729541
blob = hb.glib_blob_create (GLib.بائٹیں_3.نیا (fontdata))
face = hb.face_create (blob, 0)
مٹاؤ blob
font = hb.font_create (face)
upem = hb.face_get_upem (face)
مٹاؤ face
hb.font_سیٹ_scale (font, upem, upem)
#hb.ft_font_set_funcs (font)
hb.ot_font_سیٹ_funcs (font)

buf = hb.بفر_create ()
جماعت Debugger (object):
	تعریف پیغام (self, buf, font, پیغام_2, ڈیٹا, _x_what_is_this):
		چھاپو (پیغام_2)
		واپس سچ
debugger = Debugger ()
hb.بفر_سیٹ_پیغام_func (buf, debugger.پیغام, 1, 0)

##
## Add text to buffer
##
#
# See https://github.com/harfbuzz/harfbuzz/pull/271
#
# If you do not care about cluster values reflecting Python
# string indices, then this is quickest way to add text to
# buffer:
# hb.buffer_add_utf8 (buf, text.encode('utf-8'), 0, -1)
# Otherwise, then following handles both narrow and wide
# Python builds (the first item in the array is BOM, so we skip it):
اگر sys.maxunicode == 0x10FFFF:
	hb.بفر_جوڑو_utf32 (buf, array.array ('I', متن.encode ('utf-32'))[1:], 0, -1)
ورنہ:
	hb.بفر_جوڑو_utf16 (buf, array.array ('H', متن.encode ('utf-16'))[1:], 0, -1)


hb.بفر_guess_segment_properties (buf)

hb.shape (font, buf, [])
مٹاؤ font

infos = hb.بفر_get_glyph_infos (buf)
positions = hb.بفر_get_glyph_positions (buf)

برائے info, pos میں zip (infos, positions):
	gid = info.codepoint
	cluster = info.cluster
	x_advance = pos.x_advance
	x_انحراف_2 = pos.x_انحراف_2
	y_انحراف_2 = pos.y_انحراف_2

	چھاپو ("gid%d=%d@%d,%d+%d" % (gid, cluster, x_advance, x_انحراف_2, y_انحراف_2))
