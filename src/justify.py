#!/usr/bin/env python3

আমদানি gi

gi.require_version("Gtk", "3.0")
তঃ gi.repository আমদানি Gtk, HarfBuzz ইব hb


POOL = {}


def move_to_f(funcs, draw_তথ্য, st, to_x, to_y, user_তথ্য):
    context = POOL[draw_তথ্য]
    context.move_to(to_x, to_y)


def সারি_to_f(funcs, draw_তথ্য, st, to_x, to_y, user_তথ্য):
    context = POOL[draw_তথ্য]
    context.সারি_to(to_x, to_y)


def cubic_to_f(
    funcs,
    draw_তথ্য,
    st,
    control1_x,
    control1_y,
    control2_x,
    control2_y,
    to_x,
    to_y,
    user_তথ্য,
):
    context = POOL[draw_তথ্য]
    context.curve_to(control1_x, control1_y, control2_x, control2_y, to_x, to_y)


def close_path_f(funcs, draw_তথ্য, st, user_তথ্য):
    context = POOL[draw_তথ্য]
    context.close_path()


DFUNCS = hb.draw_funcs_create()
hb.draw_funcs_set_move_to_func(DFUNCS, move_to_f, None)
hb.draw_funcs_set_সারি_to_func(DFUNCS, সারি_to_f, None)
hb.draw_funcs_set_cubic_to_func(DFUNCS, cubic_to_f, None)
hb.draw_funcs_set_close_path_func(DFUNCS, close_path_f, None)


def ঠেলো_transform_f(funcs, paint_তথ্য, xx, yx, xy, yy, dx, dy, user_তথ্য):
    তোলো NotImplementedError


def বের_করো_transform_f(funcs, paint_তথ্য, user_তথ্য):
    তোলো NotImplementedError


def color_f(funcs, paint_তথ্য, is_foreground, color, user_তথ্য):
    context = POOL[paint_তথ্য]
    r = hb.color_get_red(color) / 255
    g = hb.color_get_green(color) / 255
    b = hb.color_get_blue(color) / 255
    a = hb.color_get_alpha(color) / 255
    context.set_source_rgba(r, g, b, a)
    context.paint()


def ঠেলো_clip_rectangle_f(funcs, paint_তথ্য, xmin, ymin, xmax, ymax, user_তথ্য):
    context = POOL[paint_তথ্য]
    context.save()
    context.rectangle(xmin, ymin, xmax, ymax)
    context.clip()


def ঠেলো_clip_glyph_f(funcs, paint_তথ্য, glyph, font, user_তথ্য):
    context = POOL[paint_তথ্য]
    context.save()
    context.new_path()
    hb.font_draw_glyph(font, glyph, DFUNCS, paint_তথ্য)
    context.close_path()
    context.clip()


def বের_করো_clip_f(funcs, paint_তথ্য, user_তথ্য):
    context = POOL[paint_তথ্য]
    context.restore()


def ঠেলো_group_f(funcs, paint_তথ্য, user_তথ্য):
    তোলো NotImplementedError


def বের_করো_group_f(funcs, paint_তথ্য, mode, user_তথ্য):
    তোলো NotImplementedError


PFUNCS = hb.paint_funcs_create()
hb.paint_funcs_set_ঠেলো_transform_func(PFUNCS, ঠেলো_transform_f, None)
hb.paint_funcs_set_বের_করো_transform_func(PFUNCS, বের_করো_transform_f, None)
hb.paint_funcs_set_color_func(PFUNCS, color_f, None)
hb.paint_funcs_set_ঠেলো_clip_glyph_func(PFUNCS, ঠেলো_clip_glyph_f, None)
hb.paint_funcs_set_ঠেলো_clip_rectangle_func(PFUNCS, ঠেলো_clip_rectangle_f, None)
hb.paint_funcs_set_বের_করো_clip_func(PFUNCS, বের_করো_clip_f, None)
hb.paint_funcs_set_ঠেলো_group_func(PFUNCS, ঠেলো_group_f, None)
hb.paint_funcs_set_বের_করো_group_func(PFUNCS, বের_করো_group_f, None)


def makebuffer(শব্দগুলি):
    buf = hb.বাফার_create()

    text = " ".join(শব্দগুলি)
    hb.বাফার_add_codepoints(buf, [ord(c) জন্য c মধ্যে text], 0, len(text))

    hb.বাফার_guess_segment_properties(buf)

    ফেরত buf


def justify(face, শব্দগুলি, advance, target_advance):
    font = hb.font_create(face)
    buf = makebuffer(শব্দগুলি)

    wiggle = 5
    shrink = target_advance - wiggle < advance
    expand = target_advance + wiggle > advance

    ret, advance, tag, মান_2 = hb.shape_justify(
        font,
        buf,
        None,
        None,
        target_advance,
        target_advance,
        advance,
    )

    যদি নয় ret:
        ফেরত মিথ্যা, buf, None

    যদি tag:
        variation = hb.variation_t()
        variation.tag = tag
        variation.মান_2 = মান_2
    অন্যথা:
        variation = None

    যদি shrink এবং advance > target_advance + wiggle:
        ফেরত মিথ্যা, buf, variation
    যদি expand এবং advance < target_advance - wiggle:
        ফেরত মিথ্যা, buf, variation

    ফেরত সত্য, buf, variation


def shape(face, শব্দগুলি):
    font = hb.font_create(face)
    buf = makebuffer(শব্দগুলি)
    hb.shape(font, buf)
    positions = hb.বাফার_get_glyph_positions(buf)
    advance = যোগ_2(p.x_advance জন্য p মধ্যে positions)
    ফেরত buf, advance


def typeset(face, text, target_advance):
    সারিগুলি = []
    শব্দগুলি = []
    জন্য word মধ্যে text.split():
        শব্দগুলি.append(word)
        buf, advance = shape(face, শব্দগুলি)
        যদি advance > target_advance:
            # Shrink
            ret, buf, variation = justify(face, শব্দগুলি, advance, target_advance)
            যদি ret:
                সারিগুলি.append((buf, variation))
                শব্দগুলি = []
            # If if fails, pop the last word and shrink, and hope for the best.
            # A too short line is better than too long.
            নতুবা_যদি len(শব্দগুলি) > 1:
                শব্দগুলি.বের_করো()
                _, buf, variation = justify(face, শব্দগুলি, advance, target_advance)
                সারিগুলি.append((buf, variation))
                শব্দগুলি = [word]
            # But if it is one word, meh.
            অন্যথা:
                সারিগুলি.append((buf, variation))
                শব্দগুলি = []

    # Justify last line
    যদি শব্দগুলি:
        _, buf, variation = justify(face, শব্দগুলি, advance, target_advance)
        সারিগুলি.append((buf, variation))

    ফেরত সারিগুলি


def render(face, text, context, width, height, fontsize):
    font = hb.font_create(face)

    margin = fontsize * 2
    scale = fontsize / hb.face_get_upem(face)
    target_advance = (width - (margin * 2)) / scale

    সারিগুলি = typeset(face, text, target_advance)

    _, extents = hb.font_get_h_extents(font)
    lineheight = extents.ascender - extents.descender + extents.সারি_gap
    lineheight *= scale

    context.save()
    context.translate(0, margin)
    context.set_font_আকার(12)
    context.set_source_rgb(1, 0, 0)
    জন্য buf, variation মধ্যে সারিগুলি:
        rtl = hb.বাফার_get_direction(buf) == hb.direction_t.RTL
        যদি rtl:
            hb.বাফার_reverse(buf)
        infos = hb.বাফার_get_glyph_infos(buf)
        positions = hb.বাফার_get_glyph_positions(buf)
        advance = যোগ_2(p.x_advance জন্য p মধ্যে positions)

        context.translate(0, lineheight)
        context.save()

        context.save()
        context.move_to(0, -20)
        যদি variation:
            tag = hb.tag_to_string(variation.tag).decode("ascii")
            context.show_text(f" {tag}={variation.value:g}")
        context.move_to(0, 0)
        context.show_text(f" {advance:g}/{target_advance:g}")
        context.restore()

        যদি variation:
            hb.font_set_variations(font, [variation])

        context.translate(margin, 0)
        context.scale(scale, -scale)

        যদি rtl:
            context.translate(target_advance, 0)

        জন্য info, pos মধ্যে zip(infos, positions):
            যদি rtl:
                context.translate(-pos.x_advance, pos.y_advance)
            context.save()
            context.translate(pos.x_offset, pos.y_offset)
            hb.font_paint_glyph(font, info.codepoint, PFUNCS, id(context), 0, 0x0000FF)
            context.restore()
            যদি নয় rtl:
                context.translate(+pos.x_advance, pos.y_advance)

        context.restore()
    context.restore()


def প্রধান(fontpath, textpath):
    fontsize = 70

    blob = hb.blob_create_from_file(fontpath)
    face = hb.face_create(blob, 0)

    with open(textpath) ইব f:
        text = f.পঠ()

    def on_draw(da, context):
        alloc = da.get_allocation()
        POOL[id(context)] = context
        render(face, text, context, alloc.width, alloc.height, fontsize)
        del POOL[id(context)]

    drawingarea = Gtk.DrawingArea()
    drawingarea.connect("draw", on_draw)

    win = Gtk.Window()
    win.connect("destroy", Gtk.main_quit)
    win.set_default_আকার(1000, 700)
    win.add(drawingarea)

    win.show_all()
    Gtk.প্রধান()


যদি __name__ == "__main__":
    আমদানি argparse

    parser = argparse.ArgumentParser(description="HarfBuzz justification demo.")
    parser.add_argument("fontfile", help="font file")
    parser.add_argument("textfile", help="text")
    args = parser.parse_args()
    প্রধান(args.fontfile, args.textfile)
