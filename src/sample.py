#!/usr/bin/env python3

استورد sys
استورد array
استورد gi
gi.require_نسخه('HarfBuzz', '0.0')
من gi.repository استورد HarfBuzz كـ hb
من gi.repository استورد GLib

fontdata = open (sys.argv[1], 'rb').اقرأ ()
متن = sys.argv[2]
# Need to create GLib.Bytes explicitly until this bug is fixed:
# https://bugzilla.gnome.org/show_bug.cgi?id=729541
blob = hb.glib_blob_create (GLib.بایټونه_3.جديد (fontdata))
face = hb.face_create (blob, 0)
لرېکړه blob
font = hb.font_create (face)
upem = hb.face_get_upem (face)
لرېکړه face
hb.font_سېټ_scale (font, upem, upem)
#hb.ft_font_set_funcs (font)
hb.ot_font_سېټ_funcs (font)

buf = hb.بفر_create ()
صنف Debugger (object):
	تعریف پیغام (self, buf, font, پیغام_2, معلومات, _x_what_is_this):
		چاپ (پیغام_2)
		أرجع ریښتیا
debugger = Debugger ()
hb.بفر_سېټ_پیغام_func (buf, debugger.پیغام, 1, 0)

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
إذا sys.maxunicode == 0x10FFFF:
	hb.بفر_جمع_کړه_utf32 (buf, array.array ('I', متن.encode ('utf-32'))[1:], 0, -1)
وإلا:
	hb.بفر_جمع_کړه_utf16 (buf, array.array ('H', متن.encode ('utf-16'))[1:], 0, -1)


hb.بفر_guess_segment_properties (buf)

hb.shape (font, buf, [])
لرېکړه font

infos = hb.بفر_get_glyph_infos (buf)
positions = hb.بفر_get_glyph_positions (buf)

لكل info, pos في zip (infos, positions):
	gid = info.codepoint
	cluster = info.cluster
	x_advance = pos.x_advance
	x_دننه_واټن_2 = pos.x_دننه_واټن_2
	y_دننه_واټن_2 = pos.y_دننه_واټن_2

	چاپ ("gid%d=%d@%d,%d+%d" % (gid, cluster, x_advance, x_دننه_واټن_2, y_دننه_واټن_2))
