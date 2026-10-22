#!/usr/bin/env python3

درآمد gi

gi.require_نسخہ("Gtk", "3.0")
سے gi.repository درآمد Gtk, HarfBuzz بطور hb


POOL = {}


تعریف move_مقصد_f(funcs, draw_ڈیٹا, st, مقصد_x, مقصد_y, user_ڈیٹا):
    context = POOL[draw_ڈیٹا]
    context.move_مقصد(مقصد_x, مقصد_y)


تعریف سطر_مقصد_f(funcs, draw_ڈیٹا, st, مقصد_x, مقصد_y, user_ڈیٹا):
    context = POOL[draw_ڈیٹا]
    context.سطر_مقصد(مقصد_x, مقصد_y)


تعریف cubic_مقصد_f(
    funcs,
    draw_ڈیٹا,
    st,
    control1_x,
    control1_y,
    control2_x,
    control2_y,
    مقصد_x,
    مقصد_y,
    user_ڈیٹا,
):
    context = POOL[draw_ڈیٹا]
    context.خم_مقصد(control1_x, control1_y, control2_x, control2_y, مقصد_x, مقصد_y)


تعریف close_راستہ_f(funcs, draw_ڈیٹا, st, user_ڈیٹا):
    context = POOL[draw_ڈیٹا]
    context.close_راستہ()


DFUNCS = hb.draw_funcs_create()
hb.draw_funcs_سیٹ_move_مقصد_func(DFUNCS, move_مقصد_f, کچھ_نہیں)
hb.draw_funcs_سیٹ_سطر_مقصد_func(DFUNCS, سطر_مقصد_f, کچھ_نہیں)
hb.draw_funcs_سیٹ_cubic_مقصد_func(DFUNCS, cubic_مقصد_f, کچھ_نہیں)
hb.draw_funcs_سیٹ_close_راستہ_func(DFUNCS, close_راستہ_f, کچھ_نہیں)


تعریف ڈالو_transform_f(funcs, paint_ڈیٹا, xx, yx, xy, yy, dx, dy, user_ڈیٹا):
    اٹھاؤ NotImplementedError


تعریف نکالو_transform_f(funcs, paint_ڈیٹا, user_ڈیٹا):
    اٹھاؤ NotImplementedError


تعریف color_f(funcs, paint_ڈیٹا, is_foreground, color, user_ڈیٹا):
    context = POOL[paint_ڈیٹا]
    r = hb.color_get_red(color) / 255
    g = hb.color_get_green(color) / 255
    b = hb.color_get_blue(color) / 255
    a = hb.color_get_alpha(color) / 255
    context.سیٹ_ماخذ_rgba(r, g, b, a)
    context.paint()


تعریف ڈالو_clip_rectangle_f(funcs, paint_ڈیٹا, xmin, ymin, xmax, ymax, user_ڈیٹا):
    context = POOL[paint_ڈیٹا]
    context.save()
    context.rectangle(xmin, ymin, xmax, ymax)
    context.clip()


تعریف ڈالو_clip_glyph_f(funcs, paint_ڈیٹا, glyph, font, user_ڈیٹا):
    context = POOL[paint_ڈیٹا]
    context.save()
    context.new_راستہ()
    hb.font_draw_glyph(font, glyph, DFUNCS, paint_ڈیٹا)
    context.close_راستہ()
    context.clip()


تعریف نکالو_clip_f(funcs, paint_ڈیٹا, user_ڈیٹا):
    context = POOL[paint_ڈیٹا]
    context.restore()


تعریف ڈالو_گروہ_f(funcs, paint_ڈیٹا, user_ڈیٹا):
    اٹھاؤ NotImplementedError


تعریف نکالو_گروہ_f(funcs, paint_ڈیٹا, منوال, user_ڈیٹا):
    اٹھاؤ NotImplementedError


PFUNCS = hb.paint_funcs_create()
hb.paint_funcs_سیٹ_ڈالو_transform_func(PFUNCS, ڈالو_transform_f, کچھ_نہیں)
hb.paint_funcs_سیٹ_نکالو_transform_func(PFUNCS, نکالو_transform_f, کچھ_نہیں)
hb.paint_funcs_سیٹ_color_func(PFUNCS, color_f, کچھ_نہیں)
hb.paint_funcs_سیٹ_ڈالو_clip_glyph_func(PFUNCS, ڈالو_clip_glyph_f, کچھ_نہیں)
hb.paint_funcs_سیٹ_ڈالو_clip_rectangle_func(PFUNCS, ڈالو_clip_rectangle_f, کچھ_نہیں)
hb.paint_funcs_سیٹ_نکالو_clip_func(PFUNCS, نکالو_clip_f, کچھ_نہیں)
hb.paint_funcs_سیٹ_ڈالو_گروہ_func(PFUNCS, ڈالو_گروہ_f, کچھ_نہیں)
hb.paint_funcs_سیٹ_نکالو_گروہ_func(PFUNCS, نکالو_گروہ_f, کچھ_نہیں)


تعریف makebuffer(الفاظ):
    buf = hb.بفر_create()

    متن = " ".join(الفاظ)
    hb.بفر_جوڑو_codepoints(buf, [ord(c) برائے c میں متن], 0, لمبائی(متن))

    hb.بفر_guess_segment_properties(buf)

    واپس buf


تعریف justify(face, الفاظ, advance, ہدف_advance):
    font = hb.font_create(face)
    buf = makebuffer(الفاظ)

    wiggle = 5
    shrink = ہدف_advance - wiggle < advance
    expand = ہدف_advance + wiggle > advance

    ret, advance, لیبل_2, قدر_2 = hb.shape_justify(
        font,
        buf,
        کچھ_نہیں,
        کچھ_نہیں,
        ہدف_advance,
        ہدف_advance,
        advance,
    )

    اگر نہیں ret:
        واپس جھوٹ, buf, کچھ_نہیں

    اگر لیبل_2:
        variation = hb.variation_t()
        variation.لیبل_2 = لیبل_2
        variation.قدر_2 = قدر_2
    ورنہ:
        variation = کچھ_نہیں

    اگر shrink اور advance > ہدف_advance + wiggle:
        واپس جھوٹ, buf, variation
    اگر expand اور advance < ہدف_advance - wiggle:
        واپس جھوٹ, buf, variation

    واپس سچ, buf, variation


تعریف shape(face, الفاظ):
    font = hb.font_create(face)
    buf = makebuffer(الفاظ)
    hb.shape(font, buf)
    positions = hb.بفر_get_glyph_positions(buf)
    advance = مجموع(p.x_advance برائے p میں positions)
    واپس buf, advance


تعریف typeset(face, متن, ہدف_advance):
    سطریں = []
    الفاظ = []
    برائے لفظ_2 میں متن.بانٹو():
        الفاظ.append(لفظ_2)
        buf, advance = shape(face, الفاظ)
        اگر advance > ہدف_advance:
            # Shrink
            ret, buf, variation = justify(face, الفاظ, advance, ہدف_advance)
            اگر ret:
                سطریں.append((buf, variation))
                الفاظ = []
            # If if fails, pop the last word and shrink, and hope for the best.
            # A too short line is better than too long.
            وگرنہ_اگر لمبائی(الفاظ) > 1:
                الفاظ.نکالو()
                _, buf, variation = justify(face, الفاظ, advance, ہدف_advance)
                سطریں.append((buf, variation))
                الفاظ = [لفظ_2]
            # But if it is one word, meh.
            ورنہ:
                سطریں.append((buf, variation))
                الفاظ = []

    # Justify last line
    اگر الفاظ:
        _, buf, variation = justify(face, الفاظ, advance, ہدف_advance)
        سطریں.append((buf, variation))

    واپس سطریں


تعریف render(face, متن, context, چوڑائی, اونچائی, fontsize):
    font = hb.font_create(face)

    margin = fontsize * 2
    scale = fontsize / hb.face_get_upem(face)
    ہدف_advance = (چوڑائی - (margin * 2)) / scale

    سطریں = typeset(face, متن, ہدف_advance)

    _, extents = hb.font_get_h_extents(font)
    lineheight = extents.ascender - extents.descender + extents.سطر_gap
    lineheight *= scale

    context.save()
    context.translate(0, margin)
    context.سیٹ_font_سائز(12)
    context.سیٹ_ماخذ_rgb(1, 0, 0)
    برائے buf, variation میں سطریں:
        rtl = hb.بفر_get_direction(buf) == hb.direction_t.RTL
        اگر rtl:
            hb.بفر_الٹ(buf)
        infos = hb.بفر_get_glyph_infos(buf)
        positions = hb.بفر_get_glyph_positions(buf)
        advance = مجموع(p.x_advance برائے p میں positions)

        context.translate(0, lineheight)
        context.save()

        context.save()
        context.move_مقصد(0, -20)
        اگر variation:
            لیبل_2 = hb.لیبل_مقصد_string(variation.لیبل_2).decode("ascii")
            context.show_متن(f" {tag}={variation.value:g}")
        context.move_مقصد(0, 0)
        context.show_متن(f" {advance:g}/{target_advance:g}")
        context.restore()

        اگر variation:
            hb.font_سیٹ_variations(font, [variation])

        context.translate(margin, 0)
        context.scale(scale, -scale)

        اگر rtl:
            context.translate(ہدف_advance, 0)

        برائے info, pos میں zip(infos, positions):
            اگر rtl:
                context.translate(-pos.x_advance, pos.y_advance)
            context.save()
            context.translate(pos.x_انحراف_2, pos.y_انحراف_2)
            hb.font_paint_glyph(font, info.codepoint, PFUNCS, id(context), 0, 0x0000FF)
            context.restore()
            اگر نہیں rtl:
                context.translate(+pos.x_advance, pos.y_advance)

        context.restore()
    context.restore()


تعریف مین(fontpath, textpath):
    fontsize = 70

    blob = hb.blob_create_from_file(fontpath)
    face = hb.face_create(blob, 0)

    ساتھ open(textpath) بطور f:
        متن = f.اقرأ()

    تعریف on_draw(da, context):
        alloc = da.get_تخصیص()
        POOL[id(context)] = context
        render(face, متن, context, alloc.چوڑائی, alloc.اونچائی, fontsize)
        مٹاؤ POOL[id(context)]

    drawingarea = Gtk.DrawingArea()
    drawingarea.connect("draw", on_draw)

    win = Gtk.کھڑکی()
    win.connect("destroy", Gtk.main_quit)
    win.سیٹ_default_سائز(1000, 700)
    win.جوڑو(drawingarea)

    win.show_all()
    Gtk.مین()


اگر __name__ == "__main__":
    درآمد argparse

    parser = argparse.دلیل_Parser(description="HarfBuzz justification demo.")
    parser.جوڑو_دلیل("fontfile", help="font file")
    parser.جوڑو_دلیل("textfile", help="text")
    دلائل = parser.parse_دلائل()
    مین(دلائل.fontfile, دلائل.textfile)
