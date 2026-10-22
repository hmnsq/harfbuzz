#!/usr/bin/env python3

आयात gi

gi.require_version("Gtk", "3.0")
from gi.repository आयात Gtk, HarfBuzz as hb


POOL = {}


def move_to_f(funcs, draw_तथ्य, st, to_x, to_y, user_तथ्य):
    context = POOL[draw_तथ्य]
    context.move_to(to_x, to_y)


def पंक्ति_to_f(funcs, draw_तथ्य, st, to_x, to_y, user_तथ्य):
    context = POOL[draw_तथ्य]
    context.पंक्ति_to(to_x, to_y)


def cubic_to_f(
    funcs,
    draw_तथ्य,
    st,
    control1_x,
    control1_y,
    control2_x,
    control2_y,
    to_x,
    to_y,
    user_तथ्य,
):
    context = POOL[draw_तथ्य]
    context.curve_to(control1_x, control1_y, control2_x, control2_y, to_x, to_y)


def close_path_f(funcs, draw_तथ्य, st, user_तथ्य):
    context = POOL[draw_तथ्य]
    context.close_path()


DFUNCS = hb.draw_funcs_create()
hb.draw_funcs_set_move_to_func(DFUNCS, move_to_f, None)
hb.draw_funcs_set_पंक्ति_to_func(DFUNCS, पंक्ति_to_f, None)
hb.draw_funcs_set_cubic_to_func(DFUNCS, cubic_to_f, None)
hb.draw_funcs_set_close_path_func(DFUNCS, close_path_f, None)


def धकेलो_transform_f(funcs, paint_तथ्य, xx, yx, xy, yy, dx, dy, user_तथ्य):
    raise NotImplementedError


def निकालो_transform_f(funcs, paint_तथ्य, user_तथ्य):
    raise NotImplementedError


def color_f(funcs, paint_तथ्य, is_foreground, color, user_तथ्य):
    context = POOL[paint_तथ्य]
    r = hb.color_get_red(color) / 255
    g = hb.color_get_green(color) / 255
    b = hb.color_get_blue(color) / 255
    a = hb.color_get_alpha(color) / 255
    context.set_source_rgba(r, g, b, a)
    context.बनाओ()


def धकेलो_clip_rectangle_f(funcs, paint_तथ्य, xmin, ymin, xmax, ymax, user_तथ्य):
    context = POOL[paint_तथ्य]
    context.save()
    context.rectangle(xmin, ymin, xmax, ymax)
    context.clip()


def धकेलो_clip_glyph_f(funcs, paint_तथ्य, glyph, font, user_तथ्य):
    context = POOL[paint_तथ्य]
    context.save()
    context.new_path()
    hb.font_draw_glyph(font, glyph, DFUNCS, paint_तथ्य)
    context.close_path()
    context.clip()


def निकालो_clip_f(funcs, paint_तथ्य, user_तथ्य):
    context = POOL[paint_तथ्य]
    context.restore()


def धकेलो_group_f(funcs, paint_तथ्य, user_तथ्य):
    raise NotImplementedError


def निकालो_group_f(funcs, paint_तथ्य, mode, user_तथ्य):
    raise NotImplementedError


PFUNCS = hb.paint_funcs_create()
hb.paint_funcs_set_धकेलो_transform_func(PFUNCS, धकेलो_transform_f, None)
hb.paint_funcs_set_निकालो_transform_func(PFUNCS, निकालो_transform_f, None)
hb.paint_funcs_set_color_func(PFUNCS, color_f, None)
hb.paint_funcs_set_धकेलो_clip_glyph_func(PFUNCS, धकेलो_clip_glyph_f, None)
hb.paint_funcs_set_धकेलो_clip_rectangle_func(PFUNCS, धकेलो_clip_rectangle_f, None)
hb.paint_funcs_set_निकालो_clip_func(PFUNCS, निकालो_clip_f, None)
hb.paint_funcs_set_धकेलो_group_func(PFUNCS, धकेलो_group_f, None)
hb.paint_funcs_set_निकालो_group_func(PFUNCS, निकालो_group_f, None)


def makebuffer(शब्दगण):
    buf = hb.बफर_create()

    text = " ".मेल(शब्दगण)
    hb.बफर_add_codepoints(buf, [ord(c) क्रम c in text], 0, len(text))

    hb.बफर_guess_segment_properties(buf)

    वापस buf


def justify(face, शब्दगण, advance, target_advance):
    font = hb.font_create(face)
    buf = makebuffer(शब्दगण)

    wiggle = 5
    shrink = target_advance - wiggle < advance
    expand = target_advance + wiggle > advance

    ret, advance, tag, मूल्य_2 = hb.shape_justify(
        font,
        buf,
        None,
        None,
        target_advance,
        target_advance,
        advance,
    )

    अगर not ret:
        वापस False, buf, None

    अगर tag:
        variation = hb.variation_t()
        variation.tag = tag
        variation.मूल्य_2 = मूल्य_2
    अन्यथा:
        variation = None

    अगर shrink and advance > target_advance + wiggle:
        वापस False, buf, variation
    अगर expand and advance < target_advance - wiggle:
        वापस False, buf, variation

    वापस True, buf, variation


def shape(face, शब्दगण):
    font = hb.font_create(face)
    buf = makebuffer(शब्दगण)
    hb.shape(font, buf)
    positions = hb.बफर_get_glyph_positions(buf)
    advance = योग_2(p.x_advance क्रम p in positions)
    वापस buf, advance


def typeset(face, text, target_advance):
    पंक्तियाँ = []
    शब्दगण = []
    क्रम word in text.split():
        शब्दगण.append(word)
        buf, advance = shape(face, शब्दगण)
        अगर advance > target_advance:
            # Shrink
            ret, buf, variation = justify(face, शब्दगण, advance, target_advance)
            अगर ret:
                पंक्तियाँ.append((buf, variation))
                शब्दगण = []
            # If if fails, pop the last word and shrink, and hope for the best.
            # A too short line is better than too long.
            elif len(शब्दगण) > 1:
                शब्दगण.निकालो()
                _, buf, variation = justify(face, शब्दगण, advance, target_advance)
                पंक्तियाँ.append((buf, variation))
                शब्दगण = [word]
            # But if it is one word, meh.
            अन्यथा:
                पंक्तियाँ.append((buf, variation))
                शब्दगण = []

    # Justify last line
    अगर शब्दगण:
        _, buf, variation = justify(face, शब्दगण, advance, target_advance)
        पंक्तियाँ.append((buf, variation))

    वापस पंक्तियाँ


def render(face, text, context, width, height, fontsize):
    font = hb.font_create(face)

    margin = fontsize * 2
    scale = fontsize / hb.face_get_upem(face)
    target_advance = (width - (margin * 2)) / scale

    पंक्तियाँ = typeset(face, text, target_advance)

    _, extents = hb.font_get_h_extents(font)
    lineheight = extents.ascender - extents.descender + extents.पंक्ति_gap
    lineheight *= scale

    context.save()
    context.translate(0, margin)
    context.set_font_आकार(12)
    context.set_source_rgb(1, 0, 0)
    क्रम buf, variation in पंक्तियाँ:
        rtl = hb.बफर_get_direction(buf) == hb.direction_t.RTL
        अगर rtl:
            hb.बफर_reverse(buf)
        infos = hb.बफर_get_glyph_infos(buf)
        positions = hb.बफर_get_glyph_positions(buf)
        advance = योग_2(p.x_advance क्रम p in positions)

        context.translate(0, lineheight)
        context.save()

        context.save()
        context.move_to(0, -20)
        अगर variation:
            tag = hb.tag_to_string(variation.tag).decode("ascii")
            context.show_text(f" {tag}={variation.value:g}")
        context.move_to(0, 0)
        context.show_text(f" {advance:g}/{target_advance:g}")
        context.restore()

        अगर variation:
            hb.font_set_variations(font, [variation])

        context.translate(margin, 0)
        context.scale(scale, -scale)

        अगर rtl:
            context.translate(target_advance, 0)

        क्रम info, pos in zip(infos, positions):
            अगर rtl:
                context.translate(-pos.x_advance, pos.y_advance)
            context.save()
            context.translate(pos.x_offset, pos.y_offset)
            hb.font_paint_glyph(font, info.codepoint, PFUNCS, id(context), 0, 0x0000FF)
            context.restore()
            अगर not rtl:
                context.translate(+pos.x_advance, pos.y_advance)

        context.restore()
    context.restore()


def मुख्य(fontpath, textpath):
    fontsize = 70

    blob = hb.blob_create_from_file(fontpath)
    face = hb.face_create(blob, 0)

    with open(textpath) as f:
        text = f.म_पढ़ो()

    def on_draw(da, context):
        alloc = da.get_allocation()
        POOL[id(context)] = context
        render(face, text, context, alloc.width, alloc.height, fontsize)
        del POOL[id(context)]

    drawingarea = Gtk.DrawingArea()
    drawingarea.connect("draw", on_draw)

    win = Gtk.Window()
    win.connect("destroy", Gtk.main_quit)
    win.set_default_आकार(1000, 700)
    win.add(drawingarea)

    win.show_all()
    Gtk.मुख्य()


अगर __name__ == "__main__":
    आयात argparse

    parser = argparse.ArgumentParser(description="HarfBuzz justification demo.")
    parser.add_argument("fontfile", help="font file")
    parser.add_argument("textfile", help="text")
    तर्क = parser.parse_args()
    मुख्य(तर्क.fontfile, तर्क.textfile)
