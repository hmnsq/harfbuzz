#!/usr/bin/env python3

استورد gi

gi.require_version("Gtk", "3.0")
من gi.repository استورد Gtk, HarfBuzz كـ hb


POOL = {}


def move_to_f(funcs, draw_data, st, to_x, to_y, user_data):
    context = POOL[draw_data]
    context.move_to(to_x, to_y)


def سطر_to_f(funcs, draw_data, st, to_x, to_y, user_data):
    context = POOL[draw_data]
    context.سطر_to(to_x, to_y)


def cubic_to_f(
    funcs,
    draw_data,
    st,
    control1_x,
    control1_y,
    control2_x,
    control2_y,
    to_x,
    to_y,
    user_data,
):
    context = POOL[draw_data]
    context.curve_to(control1_x, control1_y, control2_x, control2_y, to_x, to_y)


def close_path_f(funcs, draw_data, st, user_data):
    context = POOL[draw_data]
    context.close_path()


DFUNCS = hb.draw_funcs_create()
hb.draw_funcs_set_move_to_func(DFUNCS, move_to_f, None)
hb.draw_funcs_set_سطر_to_func(DFUNCS, سطر_to_f, None)
hb.draw_funcs_set_cubic_to_func(DFUNCS, cubic_to_f, None)
hb.draw_funcs_set_close_path_func(DFUNCS, close_path_f, None)


def ڈالو_transform_f(funcs, paint_data, xx, yx, xy, yy, dx, dy, user_data):
    ارفع NotImplementedError


def نکالو_transform_f(funcs, paint_data, user_data):
    ارفع NotImplementedError


def color_f(funcs, paint_data, is_foreground, color, user_data):
    context = POOL[paint_data]
    r = hb.color_get_red(color) / 255
    g = hb.color_get_green(color) / 255
    b = hb.color_get_blue(color) / 255
    a = hb.color_get_alpha(color) / 255
    context.set_source_rgba(r, g, b, a)
    context.paint()


def ڈالو_clip_rectangle_f(funcs, paint_data, xmin, ymin, xmax, ymax, user_data):
    context = POOL[paint_data]
    context.save()
    context.rectangle(xmin, ymin, xmax, ymax)
    context.clip()


def ڈالو_clip_glyph_f(funcs, paint_data, glyph, font, user_data):
    context = POOL[paint_data]
    context.save()
    context.new_path()
    hb.font_draw_glyph(font, glyph, DFUNCS, paint_data)
    context.close_path()
    context.clip()


def نکالو_clip_f(funcs, paint_data, user_data):
    context = POOL[paint_data]
    context.restore()


def ڈالو_group_f(funcs, paint_data, user_data):
    ارفع NotImplementedError


def نکالو_group_f(funcs, paint_data, mode, user_data):
    ارفع NotImplementedError


PFUNCS = hb.paint_funcs_create()
hb.paint_funcs_set_ڈالو_transform_func(PFUNCS, ڈالو_transform_f, None)
hb.paint_funcs_set_نکالو_transform_func(PFUNCS, نکالو_transform_f, None)
hb.paint_funcs_set_color_func(PFUNCS, color_f, None)
hb.paint_funcs_set_ڈالو_clip_glyph_func(PFUNCS, ڈالو_clip_glyph_f, None)
hb.paint_funcs_set_ڈالو_clip_rectangle_func(PFUNCS, ڈالو_clip_rectangle_f, None)
hb.paint_funcs_set_نکالو_clip_func(PFUNCS, نکالو_clip_f, None)
hb.paint_funcs_set_ڈالو_group_func(PFUNCS, ڈالو_group_f, None)
hb.paint_funcs_set_نکالو_group_func(PFUNCS, نکالو_group_f, None)


def makebuffer(الفاظ):
    buf = hb.buffer_create()

    text = " ".join(الفاظ)
    hb.buffer_add_codepoints(buf, [ord(c) لكل c في text], 0, len(text))

    hb.buffer_guess_segment_properties(buf)

    أرجع buf


def justify(face, الفاظ, advance, target_advance):
    font = hb.font_create(face)
    buf = makebuffer(الفاظ)

    wiggle = 5
    shrink = target_advance - wiggle < advance
    expand = target_advance + wiggle > advance

    ret, advance, tag, قدر_2 = hb.shape_justify(
        font,
        buf,
        None,
        None,
        target_advance,
        target_advance,
        advance,
    )

    إذا ليس ret:
        أرجع خطأ, buf, None

    إذا tag:
        variation = hb.variation_t()
        variation.tag = tag
        variation.قدر_2 = قدر_2
    وإلا:
        variation = None

    إذا shrink و advance > target_advance + wiggle:
        أرجع خطأ, buf, variation
    إذا expand و advance < target_advance - wiggle:
        أرجع خطأ, buf, variation

    أرجع صحيح, buf, variation


def shape(face, الفاظ):
    font = hb.font_create(face)
    buf = makebuffer(الفاظ)
    hb.shape(font, buf)
    positions = hb.buffer_get_glyph_positions(buf)
    advance = sum(p.x_advance لكل p في positions)
    أرجع buf, advance


def typeset(face, text, target_advance):
    سطریں = []
    الفاظ = []
    لكل word في text.split():
        الفاظ.append(word)
        buf, advance = shape(face, الفاظ)
        إذا advance > target_advance:
            # Shrink
            ret, buf, variation = justify(face, الفاظ, advance, target_advance)
            إذا ret:
                سطریں.append((buf, variation))
                الفاظ = []
            # If if fails, pop the last word and shrink, and hope for the best.
            # A too short line is better than too long.
            وإلاإذا len(الفاظ) > 1:
                الفاظ.نکالو()
                _, buf, variation = justify(face, الفاظ, advance, target_advance)
                سطریں.append((buf, variation))
                الفاظ = [word]
            # But if it is one word, meh.
            وإلا:
                سطریں.append((buf, variation))
                الفاظ = []

    # Justify last line
    إذا الفاظ:
        _, buf, variation = justify(face, الفاظ, advance, target_advance)
        سطریں.append((buf, variation))

    أرجع سطریں


def render(face, text, context, width, height, fontsize):
    font = hb.font_create(face)

    margin = fontsize * 2
    scale = fontsize / hb.face_get_upem(face)
    target_advance = (width - (margin * 2)) / scale

    سطریں = typeset(face, text, target_advance)

    _, extents = hb.font_get_h_extents(font)
    lineheight = extents.ascender - extents.descender + extents.سطر_gap
    lineheight *= scale

    context.save()
    context.translate(0, margin)
    context.set_font_size(12)
    context.set_source_rgb(1, 0, 0)
    لكل buf, variation في سطریں:
        rtl = hb.buffer_get_direction(buf) == hb.direction_t.RTL
        إذا rtl:
            hb.buffer_reverse(buf)
        infos = hb.buffer_get_glyph_infos(buf)
        positions = hb.buffer_get_glyph_positions(buf)
        advance = sum(p.x_advance لكل p في positions)

        context.translate(0, lineheight)
        context.save()

        context.save()
        context.move_to(0, -20)
        إذا variation:
            tag = hb.tag_to_string(variation.tag).decode("ascii")
            context.show_text(f" {tag}={variation.value:g}")
        context.move_to(0, 0)
        context.show_text(f" {advance:g}/{target_advance:g}")
        context.restore()

        إذا variation:
            hb.font_set_variations(font, [variation])

        context.translate(margin, 0)
        context.scale(scale, -scale)

        إذا rtl:
            context.translate(target_advance, 0)

        لكل info, pos في zip(infos, positions):
            إذا rtl:
                context.translate(-pos.x_advance, pos.y_advance)
            context.save()
            context.translate(pos.x_offset, pos.y_offset)
            hb.font_paint_glyph(font, info.codepoint, PFUNCS, id(context), 0, 0x0000FF)
            context.restore()
            إذا ليس rtl:
                context.translate(+pos.x_advance, pos.y_advance)

        context.restore()
    context.restore()


def main(fontpath, textpath):
    fontsize = 70

    blob = hb.blob_create_from_file(fontpath)
    face = hb.face_create(blob, 0)

    with open(textpath) كـ f:
        text = f.اقرأ()

    def on_draw(da, context):
        alloc = da.get_allocation()
        POOL[id(context)] = context
        render(face, text, context, alloc.width, alloc.height, fontsize)
        del POOL[id(context)]

    drawingarea = Gtk.DrawingArea()
    drawingarea.connect("draw", on_draw)

    win = Gtk.Window()
    win.connect("destroy", Gtk.main_quit)
    win.set_default_size(1000, 700)
    win.add(drawingarea)

    win.show_all()
    Gtk.main()


إذا __name__ == "__main__":
    استورد argparse

    parser = argparse.ArgumentParser(description="HarfBuzz justification demo.")
    parser.add_argument("fontfile", help="font file")
    parser.add_argument("textfile", help="text")
    args = parser.parse_args()
    main(args.fontfile, args.textfile)
