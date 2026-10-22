#!/usr/bin/env python3

استورد gi

gi.require_نسخه("Gtk", "3.0")
من gi.repository استورد Gtk, HarfBuzz كـ hb


POOL = {}


تعریف move_مقصد_f(funcs, draw_داده, st, مقصد_x, مقصد_y, user_داده):
    context = POOL[draw_داده]
    context.move_مقصد(مقصد_x, مقصد_y)


تعریف سطر_مقصد_f(funcs, draw_داده, st, مقصد_x, مقصد_y, user_داده):
    context = POOL[draw_داده]
    context.سطر_مقصد(مقصد_x, مقصد_y)


تعریف cubic_مقصد_f(
    funcs,
    draw_داده,
    st,
    control1_x,
    control1_y,
    control2_x,
    control2_y,
    مقصد_x,
    مقصد_y,
    user_داده,
):
    context = POOL[draw_داده]
    context.منحنی_مقصد(control1_x, control1_y, control2_x, control2_y, مقصد_x, مقصد_y)


تعریف close_مسیر_f(funcs, draw_داده, st, user_داده):
    context = POOL[draw_داده]
    context.close_مسیر()


DFUNCS = hb.draw_funcs_create()
hb.draw_funcs_مجموعه_move_مقصد_func(DFUNCS, move_مقصد_f, هیچ)
hb.draw_funcs_مجموعه_سطر_مقصد_func(DFUNCS, سطر_مقصد_f, هیچ)
hb.draw_funcs_مجموعه_cubic_مقصد_func(DFUNCS, cubic_مقصد_f, هیچ)
hb.draw_funcs_مجموعه_close_مسیر_func(DFUNCS, close_مسیر_f, هیچ)


تعریف افزودن_transform_f(funcs, paint_داده, xx, yx, xy, yy, dx, dy, user_داده):
    ارفع NotImplementedError


تعریف برداشت_transform_f(funcs, paint_داده, user_داده):
    ارفع NotImplementedError


تعریف color_f(funcs, paint_داده, is_foreground, color, user_داده):
    context = POOL[paint_داده]
    r = hb.color_get_red(color) / 255
    g = hb.color_get_green(color) / 255
    b = hb.color_get_blue(color) / 255
    a = hb.color_get_alpha(color) / 255
    context.مجموعه_منبع_rgba(r, g, b, a)
    context.paint()


تعریف افزودن_clip_rectangle_f(funcs, paint_داده, xmin, ymin, xmax, ymax, user_داده):
    context = POOL[paint_داده]
    context.save()
    context.rectangle(xmin, ymin, xmax, ymax)
    context.clip()


تعریف افزودن_clip_glyph_f(funcs, paint_داده, glyph, font, user_داده):
    context = POOL[paint_داده]
    context.save()
    context.new_مسیر()
    hb.font_draw_glyph(font, glyph, DFUNCS, paint_داده)
    context.close_مسیر()
    context.clip()


تعریف برداشت_clip_f(funcs, paint_داده, user_داده):
    context = POOL[paint_داده]
    context.restore()


تعریف افزودن_گروه_f(funcs, paint_داده, user_داده):
    ارفع NotImplementedError


تعریف برداشت_گروه_f(funcs, paint_داده, نما, user_داده):
    ارفع NotImplementedError


PFUNCS = hb.paint_funcs_create()
hb.paint_funcs_مجموعه_افزودن_transform_func(PFUNCS, افزودن_transform_f, هیچ)
hb.paint_funcs_مجموعه_برداشت_transform_func(PFUNCS, برداشت_transform_f, هیچ)
hb.paint_funcs_مجموعه_color_func(PFUNCS, color_f, هیچ)
hb.paint_funcs_مجموعه_افزودن_clip_glyph_func(PFUNCS, افزودن_clip_glyph_f, هیچ)
hb.paint_funcs_مجموعه_افزودن_clip_rectangle_func(PFUNCS, افزودن_clip_rectangle_f, هیچ)
hb.paint_funcs_مجموعه_برداشت_clip_func(PFUNCS, برداشت_clip_f, هیچ)
hb.paint_funcs_مجموعه_افزودن_گروه_func(PFUNCS, افزودن_گروه_f, هیچ)
hb.paint_funcs_مجموعه_برداشت_گروه_func(PFUNCS, برداشت_گروه_f, هیچ)


تعریف makebuffer(واژگان):
    buf = hb.میانگیر_create()

    متن = " ".join(واژگان)
    hb.میانگیر_جمع_کن_codepoints(buf, [ord(c) لكل c في متن], 0, طول(متن))

    hb.میانگیر_guess_segment_properties(buf)

    أرجع buf


تعریف justify(face, واژگان, advance, هدف_advance):
    font = hb.font_create(face)
    buf = makebuffer(واژگان)

    wiggle = 5
    shrink = هدف_advance - wiggle < advance
    expand = هدف_advance + wiggle > advance

    ret, advance, برچسب_2, مقدار_2 = hb.shape_justify(
        font,
        buf,
        هیچ,
        هیچ,
        هدف_advance,
        هدف_advance,
        advance,
    )

    إذا ليس ret:
        أرجع خطأ, buf, هیچ

    إذا برچسب_2:
        variation = hb.variation_t()
        variation.برچسب_2 = برچسب_2
        variation.مقدار_2 = مقدار_2
    وإلا:
        variation = هیچ

    إذا shrink و advance > هدف_advance + wiggle:
        أرجع خطأ, buf, variation
    إذا expand و advance < هدف_advance - wiggle:
        أرجع خطأ, buf, variation

    أرجع درست, buf, variation


تعریف shape(face, واژگان):
    font = hb.font_create(face)
    buf = makebuffer(واژگان)
    hb.shape(font, buf)
    positions = hb.میانگیر_get_glyph_positions(buf)
    advance = جمع(p.x_advance لكل p في positions)
    أرجع buf, advance


تعریف typeset(face, متن, هدف_advance):
    سطرها = []
    واژگان = []
    لكل کلمه_2 في متن.تقسیم():
        واژگان.append(کلمه_2)
        buf, advance = shape(face, واژگان)
        إذا advance > هدف_advance:
            # Shrink
            ret, buf, variation = justify(face, واژگان, advance, هدف_advance)
            إذا ret:
                سطرها.append((buf, variation))
                واژگان = []
            # If if fails, pop the last word and shrink, and hope for the best.
            # A too short line is better than too long.
            وإلاإذا طول(واژگان) > 1:
                واژگان.برداشت()
                _, buf, variation = justify(face, واژگان, advance, هدف_advance)
                سطرها.append((buf, variation))
                واژگان = [کلمه_2]
            # But if it is one word, meh.
            وإلا:
                سطرها.append((buf, variation))
                واژگان = []

    # Justify last line
    إذا واژگان:
        _, buf, variation = justify(face, واژگان, advance, هدف_advance)
        سطرها.append((buf, variation))

    أرجع سطرها


تعریف render(face, متن, context, عرض, ارتفاع, fontsize):
    font = hb.font_create(face)

    margin = fontsize * 2
    scale = fontsize / hb.face_get_upem(face)
    هدف_advance = (عرض - (margin * 2)) / scale

    سطرها = typeset(face, متن, هدف_advance)

    _, extents = hb.font_get_h_extents(font)
    lineheight = extents.ascender - extents.descender + extents.سطر_gap
    lineheight *= scale

    context.save()
    context.translate(0, margin)
    context.مجموعه_font_اندازه(12)
    context.مجموعه_منبع_rgb(1, 0, 0)
    لكل buf, variation في سطرها:
        rtl = hb.میانگیر_get_direction(buf) == hb.direction_t.RTL
        إذا rtl:
            hb.میانگیر_معکوس(buf)
        infos = hb.میانگیر_get_glyph_infos(buf)
        positions = hb.میانگیر_get_glyph_positions(buf)
        advance = جمع(p.x_advance لكل p في positions)

        context.translate(0, lineheight)
        context.save()

        context.save()
        context.move_مقصد(0, -20)
        إذا variation:
            برچسب_2 = hb.برچسب_مقصد_string(variation.برچسب_2).decode("ascii")
            context.show_متن(f" {tag}={variation.value:g}")
        context.move_مقصد(0, 0)
        context.show_متن(f" {advance:g}/{target_advance:g}")
        context.restore()

        إذا variation:
            hb.font_مجموعه_variations(font, [variation])

        context.translate(margin, 0)
        context.scale(scale, -scale)

        إذا rtl:
            context.translate(هدف_advance, 0)

        لكل info, pos في zip(infos, positions):
            إذا rtl:
                context.translate(-pos.x_advance, pos.y_advance)
            context.save()
            context.translate(pos.x_جابه_جایی_2, pos.y_جابه_جایی_2)
            hb.font_paint_glyph(font, info.codepoint, PFUNCS, id(context), 0, 0x0000FF)
            context.restore()
            إذا ليس rtl:
                context.translate(+pos.x_advance, pos.y_advance)

        context.restore()
    context.restore()


تعریف اصلی(fontpath, textpath):
    fontsize = 70

    blob = hb.blob_create_from_file(fontpath)
    face = hb.face_create(blob, 0)

    با open(textpath) كـ f:
        متن = f.اقرأ()

    تعریف on_draw(da, context):
        alloc = da.get_تخصیص()
        POOL[id(context)] = context
        render(face, متن, context, alloc.عرض, alloc.ارتفاع, fontsize)
        پاک POOL[id(context)]

    drawingarea = Gtk.DrawingArea()
    drawingarea.connect("draw", on_draw)

    win = Gtk.پنجره()
    win.connect("destroy", Gtk.main_quit)
    win.مجموعه_default_اندازه(1000, 700)
    win.جمع_کن(drawingarea)

    win.show_all()
    Gtk.اصلی()


إذا __name__ == "__main__":
    استورد argparse

    parser = argparse.آرگومان_Parser(description="HarfBuzz justification demo.")
    parser.جمع_کن_آرگومان("fontfile", help="font file")
    parser.جمع_کن_آرگومان("textfile", help="text")
    آرگومانها = parser.parse_آرگومانها()
    اصلی(آرگومانها.fontfile, آرگومانها.textfile)
