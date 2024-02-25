# sources.sh generated by update_sources
C3_CONFIGURES='c3c/configure c3s/configure c3s/sources.sh c3s/update_sources ic3/configure ic3/sources.sh ic3/update_sources libc3/configure libc3/sources.sh libc3/update_sources libc3/window/cairo/configure libc3/window/cairo/demo/configure libc3/window/cairo/demo/sources.sh libc3/window/cairo/demo/update_sources libc3/window/cairo/quartz/configure libc3/window/cairo/quartz/demo/configure libc3/window/cairo/quartz/demo/sources.sh libc3/window/cairo/quartz/demo/update_sources libc3/window/cairo/quartz/sources.sh libc3/window/cairo/quartz/update_sources libc3/window/cairo/sources.sh libc3/window/cairo/update_sources libc3/window/cairo/win32/configure libc3/window/cairo/win32/demo/configure libc3/window/cairo/win32/demo/sources.sh libc3/window/cairo/win32/demo/update_sources libc3/window/cairo/win32/sources.sh libc3/window/cairo/win32/update_sources libc3/window/cairo/xcb/configure libc3/window/cairo/xcb/demo/configure libc3/window/cairo/xcb/demo/sources.sh libc3/window/cairo/xcb/demo/update_sources libc3/window/cairo/xcb/sources.sh libc3/window/cairo/xcb/update_sources libc3/window/configure libc3/window/sdl2/configure libc3/window/sdl2/demo/configure libc3/window/sdl2/demo/macos/configure libc3/window/sdl2/demo/sources.sh libc3/window/sdl2/demo/update_sources libc3/window/sdl2/sources.sh libc3/window/sdl2/update_sources libc3/window/sources.sh libc3/window/update_sources libtommath/configure libtommath/sources.sh libtommath/update_sources test/configure test/sources.sh test/update_sources ucd2c/configure '
C3_MAKEFILES='c3c/Makefile c3s/Makefile c3s/sources.mk ic3/Makefile ic3/sources.mk libc3/Makefile libc3/gen.mk libc3/sources.mk libc3/window/Makefile libc3/window/cairo/Makefile libc3/window/cairo/demo/Makefile libc3/window/cairo/demo/sources.mk libc3/window/cairo/quartz/Makefile libc3/window/cairo/quartz/demo/Makefile libc3/window/cairo/quartz/demo/sources.mk libc3/window/cairo/quartz/sources.mk libc3/window/cairo/sources.mk libc3/window/cairo/win32/Makefile libc3/window/cairo/win32/demo/Makefile libc3/window/cairo/win32/demo/sources.mk libc3/window/cairo/win32/sources.mk libc3/window/cairo/xcb/Makefile libc3/window/cairo/xcb/demo/Makefile libc3/window/cairo/xcb/demo/sources.mk libc3/window/cairo/xcb/sources.mk libc3/window/sdl2/Makefile libc3/window/sdl2/demo/Makefile libc3/window/sdl2/demo/macos/Makefile libc3/window/sdl2/demo/sources.mk libc3/window/sdl2/sources.mk libc3/window/sources.mk libtommath/Makefile libtommath/sources.mk test/Makefile test/sources.mk ucd2c/Makefile '
C3_C_SOURCES='c3c/c3c.c c3s/buf_readline.c c3s/buf_readline.h c3s/c3s.c ic3/buf_linenoise.c ic3/buf_linenoise.h ic3/buf_wineditline.c ic3/buf_wineditline.h ic3/config.h ic3/ic3.c ic3/linenoise.c libc3/abs.c libc3/abs.h libc3/arg.c libc3/arg.h libc3/array.c libc3/array.h libc3/assert.h libc3/binding.c libc3/binding.h libc3/block.c libc3/block.h libc3/bool.c libc3/bool.h libc3/buf.c libc3/buf.h libc3/buf_file.c libc3/buf_file.h libc3/buf_inspect.c libc3/buf_inspect.h libc3/buf_inspect_s.c.in libc3/buf_inspect_s.h.in libc3/buf_inspect_s16.c libc3/buf_inspect_s16.h libc3/buf_inspect_s16_binary.c libc3/buf_inspect_s16_binary.h libc3/buf_inspect_s16_decimal.c libc3/buf_inspect_s16_decimal.h libc3/buf_inspect_s16_hexadecimal.c libc3/buf_inspect_s16_hexadecimal.h libc3/buf_inspect_s16_octal.c libc3/buf_inspect_s16_octal.h libc3/buf_inspect_s32.c libc3/buf_inspect_s32.h libc3/buf_inspect_s32_binary.c libc3/buf_inspect_s32_binary.h libc3/buf_inspect_s32_decimal.c libc3/buf_inspect_s32_decimal.h libc3/buf_inspect_s32_hexadecimal.c libc3/buf_inspect_s32_hexadecimal.h libc3/buf_inspect_s32_octal.c libc3/buf_inspect_s32_octal.h libc3/buf_inspect_s64.c libc3/buf_inspect_s64.h libc3/buf_inspect_s64_binary.c libc3/buf_inspect_s64_binary.h libc3/buf_inspect_s64_decimal.c libc3/buf_inspect_s64_decimal.h libc3/buf_inspect_s64_hexadecimal.c libc3/buf_inspect_s64_hexadecimal.h libc3/buf_inspect_s64_octal.c libc3/buf_inspect_s64_octal.h libc3/buf_inspect_s8.c libc3/buf_inspect_s8.h libc3/buf_inspect_s8_binary.c libc3/buf_inspect_s8_binary.h libc3/buf_inspect_s8_decimal.c libc3/buf_inspect_s8_decimal.h libc3/buf_inspect_s8_hexadecimal.c libc3/buf_inspect_s8_hexadecimal.h libc3/buf_inspect_s8_octal.c libc3/buf_inspect_s8_octal.h libc3/buf_inspect_s_base.c.in libc3/buf_inspect_s_base.h.in libc3/buf_inspect_sw.c libc3/buf_inspect_sw.h libc3/buf_inspect_sw_binary.c libc3/buf_inspect_sw_binary.h libc3/buf_inspect_sw_decimal.c libc3/buf_inspect_sw_decimal.h libc3/buf_inspect_sw_hexadecimal.c libc3/buf_inspect_sw_hexadecimal.h libc3/buf_inspect_sw_octal.c libc3/buf_inspect_sw_octal.h libc3/buf_inspect_u.c.in libc3/buf_inspect_u.h.in libc3/buf_inspect_u16.c libc3/buf_inspect_u16.h libc3/buf_inspect_u16_binary.c libc3/buf_inspect_u16_binary.h libc3/buf_inspect_u16_decimal.c libc3/buf_inspect_u16_decimal.h libc3/buf_inspect_u16_hexadecimal.c libc3/buf_inspect_u16_hexadecimal.h libc3/buf_inspect_u16_octal.c libc3/buf_inspect_u16_octal.h libc3/buf_inspect_u32.c libc3/buf_inspect_u32.h libc3/buf_inspect_u32_binary.c libc3/buf_inspect_u32_binary.h libc3/buf_inspect_u32_decimal.c libc3/buf_inspect_u32_decimal.h libc3/buf_inspect_u32_hexadecimal.c libc3/buf_inspect_u32_hexadecimal.h libc3/buf_inspect_u32_octal.c libc3/buf_inspect_u32_octal.h libc3/buf_inspect_u64.c libc3/buf_inspect_u64.h libc3/buf_inspect_u64_binary.c libc3/buf_inspect_u64_binary.h libc3/buf_inspect_u64_decimal.c libc3/buf_inspect_u64_decimal.h libc3/buf_inspect_u64_hexadecimal.c libc3/buf_inspect_u64_hexadecimal.h libc3/buf_inspect_u64_octal.c libc3/buf_inspect_u64_octal.h libc3/buf_inspect_u8.c libc3/buf_inspect_u8.h libc3/buf_inspect_u8_binary.c libc3/buf_inspect_u8_binary.h libc3/buf_inspect_u8_decimal.c libc3/buf_inspect_u8_decimal.h libc3/buf_inspect_u8_hexadecimal.c libc3/buf_inspect_u8_hexadecimal.h libc3/buf_inspect_u8_octal.c libc3/buf_inspect_u8_octal.h libc3/buf_inspect_u_base.c.in libc3/buf_inspect_u_base.h.in libc3/buf_inspect_uw.c libc3/buf_inspect_uw.h libc3/buf_inspect_uw_binary.c libc3/buf_inspect_uw_binary.h libc3/buf_inspect_uw_decimal.c libc3/buf_inspect_uw_decimal.h libc3/buf_inspect_uw_hexadecimal.c libc3/buf_inspect_uw_hexadecimal.h libc3/buf_inspect_uw_octal.c libc3/buf_inspect_uw_octal.h libc3/buf_parse.c libc3/buf_parse.h libc3/buf_parse_s.c.in libc3/buf_parse_s.h.in libc3/buf_parse_s16.c libc3/buf_parse_s16.h libc3/buf_parse_s32.c libc3/buf_parse_s32.h libc3/buf_parse_s64.c libc3/buf_parse_s64.h libc3/buf_parse_s8.c libc3/buf_parse_s8.h libc3/buf_parse_sw.c libc3/buf_parse_sw.h libc3/buf_parse_u.c.in libc3/buf_parse_u.h.in libc3/buf_parse_u16.c libc3/buf_parse_u16.h libc3/buf_parse_u32.c libc3/buf_parse_u32.h libc3/buf_parse_u64.c libc3/buf_parse_u64.h libc3/buf_parse_u8.c libc3/buf_parse_u8.h libc3/buf_parse_uw.c libc3/buf_parse_uw.h libc3/buf_save.c libc3/buf_save.h libc3/c3.c libc3/c3.h libc3/c3_main.h libc3/call.c libc3/call.h libc3/ceiling.c libc3/ceiling.h libc3/cfn.c libc3/cfn.h libc3/character.c libc3/character.h libc3/compare.c libc3/compare.h libc3/data.c libc3/data.h libc3/env.c libc3/env.h libc3/error.c libc3/error.h libc3/error_handler.c libc3/error_handler.h libc3/eval.c libc3/eval.h libc3/f128.c libc3/f128.h libc3/f32.c libc3/f32.h libc3/f64.c libc3/f64.h libc3/fact.c libc3/fact.h libc3/fact_list.c libc3/fact_list.h libc3/facts.c libc3/facts.h libc3/facts_cursor.c libc3/facts_cursor.h libc3/facts_spec.c libc3/facts_spec.h libc3/facts_spec_cursor.c libc3/facts_spec_cursor.h libc3/facts_with.c libc3/facts_with.h libc3/facts_with_cursor.c libc3/facts_with_cursor.h libc3/file.c libc3/file.h libc3/float.h libc3/fn.c libc3/fn.h libc3/fn_clause.c libc3/fn_clause.h libc3/frame.c libc3/frame.h libc3/hash.c libc3/hash.h libc3/ident.c libc3/ident.h libc3/integer.c libc3/integer.h libc3/io.c libc3/io.h libc3/license.c libc3/list.c libc3/list.h libc3/list_init.c libc3/list_init.h libc3/log.c libc3/log.h libc3/map.c libc3/map.h libc3/module.c libc3/module.h libc3/operator.c libc3/operator.h libc3/point.h.in libc3/ptag.c libc3/ptag.h libc3/ptr.c libc3/ptr.h libc3/ptr_free.c libc3/ptr_free.h libc3/quote.c libc3/quote.h libc3/s.c.in libc3/s.h.in libc3/s16.c libc3/s16.h libc3/s32.c libc3/s32.h libc3/s64.c libc3/s64.h libc3/s8.c libc3/s8.h libc3/sequence.c libc3/sequence.h libc3/set.c.in libc3/set.h.in libc3/set__fact.c libc3/set__fact.h libc3/set__tag.c libc3/set__tag.h libc3/set_cursor.c.in libc3/set_cursor.h.in libc3/set_cursor__fact.c libc3/set_cursor__fact.h libc3/set_cursor__tag.c libc3/set_cursor__tag.h libc3/set_item.c.in libc3/set_item.h.in libc3/set_item__fact.c libc3/set_item__fact.h libc3/set_item__tag.c libc3/set_item__tag.h libc3/sha1.h libc3/sign.c libc3/sign.h libc3/skiplist.c.in libc3/skiplist.h.in libc3/skiplist__fact.c libc3/skiplist__fact.h libc3/skiplist_node.c.in libc3/skiplist_node.h.in libc3/skiplist_node__fact.c libc3/skiplist_node__fact.h libc3/special_operator.c libc3/special_operator.h libc3/str.c libc3/str.h libc3/struct.c libc3/struct.h libc3/struct_type.c libc3/struct_type.h libc3/sw.c libc3/sw.h libc3/sym.c libc3/sym.h libc3/tag.c libc3/tag.h libc3/tag_add.c libc3/tag_band.c libc3/tag_bor.c libc3/tag_bxor.c libc3/tag_div.c libc3/tag_init.c libc3/tag_init.h libc3/tag_mod.c libc3/tag_mul.c libc3/tag_shift_left.c libc3/tag_shift_right.c libc3/tag_sub.c libc3/tag_type.c libc3/tag_type.h libc3/time.c libc3/time.h libc3/tuple.c libc3/tuple.h libc3/type.c libc3/type.h libc3/types.h libc3/u.c.in libc3/u.h.in libc3/u16.c libc3/u16.h libc3/u32.c libc3/u32.h libc3/u64.c libc3/u64.h libc3/u8.c libc3/u8.h libc3/ucd.c libc3/ucd.h libc3/unquote.c libc3/unquote.h libc3/uw.c libc3/uw.h libc3/var.c libc3/var.h libc3/void.c libc3/void.h libc3/window/cairo/cairo_font.c libc3/window/cairo/cairo_font.h libc3/window/cairo/cairo_sprite.c libc3/window/cairo/cairo_sprite.h libc3/window/cairo/cairo_text.c libc3/window/cairo/cairo_text.h libc3/window/cairo/demo/bg_rect.c libc3/window/cairo/demo/bg_rect.h libc3/window/cairo/demo/flies.c libc3/window/cairo/demo/flies.h libc3/window/cairo/demo/lightspeed.c libc3/window/cairo/demo/lightspeed.h libc3/window/cairo/demo/mandelbrot_f128.c libc3/window/cairo/demo/mandelbrot_f128.h libc3/window/cairo/demo/toasters.c libc3/window/cairo/demo/toasters.h libc3/window/cairo/demo/window_cairo_demo.c libc3/window/cairo/demo/window_cairo_demo.h libc3/window/cairo/quartz/demo/window_cairo_quartz_demo.c libc3/window/cairo/quartz/quartz_to_xkbcommon.c libc3/window/cairo/quartz/quartz_to_xkbcommon.h libc3/window/cairo/quartz/window_cairo_quartz.h libc3/window/cairo/quartz/window_cairo_quartz_app_delegate.h libc3/window/cairo/quartz/window_cairo_quartz_view.h libc3/window/cairo/quartz/window_cairo_quartz_view_controller.h libc3/window/cairo/quartz/xkbquartz.h libc3/window/cairo/types.h libc3/window/cairo/win32/demo/window_cairo_win32_demo.c libc3/window/cairo/win32/vk_to_xkbcommon.c libc3/window/cairo/win32/vk_to_xkbcommon.h libc3/window/cairo/win32/window_cairo_win32.c libc3/window/cairo/win32/window_cairo_win32.h libc3/window/cairo/window_cairo.c libc3/window/cairo/window_cairo.h libc3/window/cairo/xcb/config.h libc3/window/cairo/xcb/demo/window_cairo_xcb_demo.c libc3/window/cairo/xcb/window_cairo_xcb.c libc3/window/cairo/xcb/window_cairo_xcb.h libc3/window/sdl2/demo/bg_rect.c libc3/window/sdl2/demo/bg_rect.h libc3/window/sdl2/demo/earth.c libc3/window/sdl2/demo/earth.h libc3/window/sdl2/demo/flies.c libc3/window/sdl2/demo/flies.h libc3/window/sdl2/demo/lightspeed.c libc3/window/sdl2/demo/lightspeed.h libc3/window/sdl2/demo/mandelbrot_f128.c libc3/window/sdl2/demo/mandelbrot_f128.h libc3/window/sdl2/demo/matrix.c libc3/window/sdl2/demo/matrix.h libc3/window/sdl2/demo/toasters.c libc3/window/sdl2/demo/toasters.h libc3/window/sdl2/demo/window_sdl2_demo.c libc3/window/sdl2/demo/window_sdl2_demo.h libc3/window/sdl2/disabled/mandelbrot.c libc3/window/sdl2/disabled/mandelbrot.h libc3/window/sdl2/disabled/sdl2_font.c libc3/window/sdl2/disabled/sdl2_font.h libc3/window/sdl2/disabled/sdl2_sprite.c libc3/window/sdl2/disabled/sdl2_sprite.h libc3/window/sdl2/dmat3.h libc3/window/sdl2/dmat4.c libc3/window/sdl2/dmat4.h libc3/window/sdl2/dvec2.c libc3/window/sdl2/dvec2.h libc3/window/sdl2/dvec3.c libc3/window/sdl2/dvec3.h libc3/window/sdl2/gl_camera.c libc3/window/sdl2/gl_camera.h libc3/window/sdl2/gl_cylinder.c libc3/window/sdl2/gl_cylinder.h libc3/window/sdl2/gl_deprecated.c libc3/window/sdl2/gl_deprecated.h libc3/window/sdl2/gl_font.c libc3/window/sdl2/gl_font.h libc3/window/sdl2/gl_lines.c libc3/window/sdl2/gl_lines.h libc3/window/sdl2/gl_object.c libc3/window/sdl2/gl_object.h libc3/window/sdl2/gl_ortho.c libc3/window/sdl2/gl_ortho.h libc3/window/sdl2/gl_sphere.c libc3/window/sdl2/gl_sphere.h libc3/window/sdl2/gl_sprite.c libc3/window/sdl2/gl_sprite.h libc3/window/sdl2/gl_square.c libc3/window/sdl2/gl_square.h libc3/window/sdl2/gl_text.c libc3/window/sdl2/gl_text.h libc3/window/sdl2/gl_triangle.c libc3/window/sdl2/gl_triangle.h libc3/window/sdl2/gl_vertex.c libc3/window/sdl2/gl_vertex.h libc3/window/sdl2/gl_vtext.c libc3/window/sdl2/gl_vtext.h libc3/window/sdl2/mat3.h libc3/window/sdl2/mat4.c libc3/window/sdl2/mat4.h libc3/window/sdl2/types.h libc3/window/sdl2/vec2.c libc3/window/sdl2/vec2.h libc3/window/sdl2/vec3.c libc3/window/sdl2/vec3.h libc3/window/sdl2/window_sdl2.c libc3/window/sdl2/window_sdl2.h libc3/window/types.h libc3/window/window.c libc3/window/window.h test/array_test.c test/bool_test.c test/buf_file_test.c test/buf_inspect_test.c test/buf_parse_test.c test/buf_parse_test.h test/buf_parse_test_s16.c test/buf_parse_test_s32.c test/buf_parse_test_s64.c test/buf_parse_test_s8.c test/buf_parse_test_su.h test/buf_parse_test_u16.c test/buf_parse_test_u32.c test/buf_parse_test_u64.c test/buf_parse_test_u8.c test/buf_test.c test/call_test.c test/cfn_test.c test/character_test.c test/compare_test.c test/compare_test.h test/env_test.c test/fact_test.c test/fact_test.h test/facts_cursor_test.c test/facts_test.c test/facts_with_test.c test/fn_test.c test/hash_test.c test/ident_test.c test/libc3_test.c test/list_test.c test/set__fact_test.c test/set__tag_test.c test/skiplist__fact_test.c test/str_test.c test/sym_test.c test/tag_test.c test/tag_test.h test/test.c test/test.h test/tuple_test.c test/types_test.c ucd2c/ucd.h ucd2c/ucd2c.c '
C3_OBJC_SOURCES='libc3/window/cairo/quartz/window_cairo_quartz.m libc3/window/cairo/quartz/window_cairo_quartz_app_delegate.m libc3/window/cairo/quartz/window_cairo_quartz_view.m libc3/window/cairo/quartz/window_cairo_quartz_view_controller.m '
C3_OTHER_SOURCES='AUTHORS Makefile README.md c3.index c3.version config.subr configure fonts/Courier New/Courier New.ttf fonts/NotoSans-Regular.ttf img/c3.1.xcf img/c3.1080.jpg img/c3.1080.png img/c3.128.jpg img/c3.128.png img/c3.16.png img/c3.256.jpg img/c3.256.png img/c3.32.jpg img/c3.32.png img/c3.48.jpg img/c3.48.png img/c3.512.jpg img/c3.512.png img/c3.64.jpg img/c3.64.png img/c3.640.jpg img/c3.640.png img/c3.720.jpg img/c3.720.png img/c3.iconset/icon_128x128.png img/c3.iconset/icon_16x16.png img/c3.iconset/icon_256x256.png img/c3.iconset/icon_32x32.png img/c3.iconset/icon_512x512.png img/c3.iconset/icon_64x64.png img/c3.xcf img/earth.jpg img/earth.png img/flaps.256.png img/flaps.png img/fly-dead.png img/fly-noto.png img/iris-c3-004.jpeg img/iris-c3-004.png img/mandelbrot_f128_limit.1.png img/mandelbrot_f128_limit.2.png img/mandelbrot_f128_limit.png img/matrix_shade.png img/thodg_No_Prompt_073261d5-2c81-4b6e-9572-e0b840c55f1f.jpeg img/toast.128.png img/toast.png lib/c3/0.1/array.facts lib/c3/0.1/c3.facts lib/c3/0.1/f32.facts lib/c3/0.1/f64.facts lib/c3/0.1/gl/object.facts lib/c3/0.1/gl/point2d.facts lib/c3/0.1/gl/point2f.facts lib/c3/0.1/gl/point3d.facts lib/c3/0.1/gl/point3f.facts lib/c3/0.1/gl/sphere.facts lib/c3/0.1/gl/triangle.facts lib/c3/0.1/gl/vertex.facts lib/c3/0.1/integer.facts lib/c3/0.1/list.facts lib/c3/0.1/map.facts lib/c3/0.1/ptr.facts lib/c3/0.1/ptr_free.facts lib/c3/0.1/s16.facts lib/c3/0.1/s32.facts lib/c3/0.1/s64.facts lib/c3/0.1/s8.facts lib/c3/0.1/str.facts lib/c3/0.1/sw.facts lib/c3/0.1/u16.facts lib/c3/0.1/u32.facts lib/c3/0.1/u64.facts lib/c3/0.1/u8.facts lib/c3/0.1/uw.facts lib/c3/0.1/void.facts libc3/tag_init.rb license.h sources.mk sources.sh test/buf_parse_test_su.rb test/facts_test_dump_file.expected.facts test/facts_test_load_file.facts test/facts_test_log_add.expected.facts test/facts_test_log_remove.expected.facts test/facts_test_open_file.1.expected.facts test/facts_test_open_file.1.in.facts test/facts_test_open_file.2.expected.facts test/facts_test_open_file.2.in.facts test/facts_test_open_file.3.expected.facts test/facts_test_open_file.3.in.facts test/facts_test_save.expected.facts test/ic3/array.err.expected test/ic3/array.in test/ic3/array.out.expected test/ic3/array.ret.expected test/ic3/block.in test/ic3/block.out.expected test/ic3/block.ret.expected test/ic3/bool.err.expected test/ic3/bool.in test/ic3/bool.out.expected test/ic3/bool.ret.expected test/ic3/call.err.expected test/ic3/call.in test/ic3/call.out.expected test/ic3/call.ret.expected test/ic3/cast.in test/ic3/cast.out.expected test/ic3/cast.ret.expected test/ic3/character.err.expected test/ic3/character.in test/ic3/character.out.expected test/ic3/character.ret.expected test/ic3/comment.err.expected test/ic3/comment.in test/ic3/comment.out.expected test/ic3/comment.ret.expected test/ic3/equal.err.expected test/ic3/equal.in test/ic3/equal.out.expected test/ic3/equal.ret.expected test/ic3/fn.err.expected test/ic3/fn.in test/ic3/fn.out.expected test/ic3/fn.ret.expected test/ic3/function_call.err.expected test/ic3/function_call.out.expected test/ic3/function_call.ret.expected test/ic3/hello.err.expected test/ic3/hello.in test/ic3/hello.out.expected test/ic3/hello.ret.expected test/ic3/ident.err.expected test/ic3/ident.in test/ic3/ident.out.expected test/ic3/ident.ret.expected test/ic3/if.in test/ic3/if.out.expected test/ic3/if.ret.expected test/ic3/integer.in test/ic3/integer.lisp test/ic3/integer.out.expected test/ic3/integer.ret.expected test/ic3/integer_add.in test/ic3/integer_add.out.expected test/ic3/integer_add.ret.expected test/ic3/integer_band.in test/ic3/integer_band.out.expected test/ic3/integer_band.ret.expected test/ic3/integer_bnot.in test/ic3/integer_bnot.out.expected test/ic3/integer_bnot.ret.expected test/ic3/integer_bor-2.in test/ic3/integer_bor-2.out.expected test/ic3/integer_bor-2.ret.expected test/ic3/integer_bxor.in test/ic3/integer_bxor.out.expected test/ic3/integer_bxor.ret.expected test/ic3/integer_div.in test/ic3/integer_div.out.expected test/ic3/integer_div.ret.expected test/ic3/integer_eq.in test/ic3/integer_eq.out.expected test/ic3/integer_eq.ret.expected test/ic3/integer_gt.in test/ic3/integer_gt.out.expected test/ic3/integer_gt.ret.expected test/ic3/integer_lt.in test/ic3/integer_lt.out.expected test/ic3/integer_lt.ret.expected test/ic3/integer_mod-2.in test/ic3/integer_mod-2.out.expected test/ic3/integer_mod-2.ret.expected test/ic3/integer_mul.in test/ic3/integer_mul.out.expected test/ic3/integer_mul.ret.expected test/ic3/integer_neg.in test/ic3/integer_neg.out.expected test/ic3/integer_neg.ret.expected test/ic3/integer_sub.in test/ic3/integer_sub.out.expected test/ic3/integer_sub.ret.expected test/ic3/list.err.expected test/ic3/list.in test/ic3/list.out.expected test/ic3/list.ret.expected test/ic3/macro.in test/ic3/macro.out.expected test/ic3/macro.ret.expected test/ic3/map.in test/ic3/map.out.expected test/ic3/map.ret.expected test/ic3/op.err.expected test/ic3/op.in test/ic3/op.out.expected test/ic3/op.ret.expected test/ic3/plist.err.expected test/ic3/plist.in test/ic3/plist.out.expected test/ic3/plist.ret.expected test/ic3/quote.in test/ic3/quote.out.expected test/ic3/quote.ret.expected test/ic3/str.err.expected test/ic3/str.in test/ic3/str.out.expected test/ic3/str.ret.expected test/ic3/struct.in test/ic3/struct.out.expected test/ic3/struct.ret.expected test/ic3/sym.err.expected test/ic3/sym.in test/ic3/sym.out.expected test/ic3/sym.ret.expected test/ic3/tuple.err.expected test/ic3/tuple.in test/ic3/tuple.out.expected test/ic3/tuple.ret.expected test/ic3/void.in test/ic3/void.out.expected test/ic3/void.ret.expected test/ic3_test test/replace_lines.rb test/test.rb test/test_case_end.rb test/zero '
C3_EXTERNAL_SOURCES='libtommath/LICENSE libtommath/README.md libtommath/bn_cutoffs.c libtommath/bn_deprecated.c libtommath/bn_mp_2expt.c libtommath/bn_mp_abs.c libtommath/bn_mp_add.c libtommath/bn_mp_add_d.c libtommath/bn_mp_addmod.c libtommath/bn_mp_and.c libtommath/bn_mp_clamp.c libtommath/bn_mp_clear.c libtommath/bn_mp_clear_multi.c libtommath/bn_mp_cmp.c libtommath/bn_mp_cmp_d.c libtommath/bn_mp_cmp_mag.c libtommath/bn_mp_cnt_lsb.c libtommath/bn_mp_complement.c libtommath/bn_mp_copy.c libtommath/bn_mp_count_bits.c libtommath/bn_mp_decr.c libtommath/bn_mp_div.c libtommath/bn_mp_div_2.c libtommath/bn_mp_div_2d.c libtommath/bn_mp_div_3.c libtommath/bn_mp_div_d.c libtommath/bn_mp_dr_is_modulus.c libtommath/bn_mp_dr_reduce.c libtommath/bn_mp_dr_setup.c libtommath/bn_mp_error_to_string.c libtommath/bn_mp_exch.c libtommath/bn_mp_expt_u32.c libtommath/bn_mp_exptmod.c libtommath/bn_mp_exteuclid.c libtommath/bn_mp_fread.c libtommath/bn_mp_from_sbin.c libtommath/bn_mp_from_ubin.c libtommath/bn_mp_fwrite.c libtommath/bn_mp_gcd.c libtommath/bn_mp_get_double.c libtommath/bn_mp_get_i32.c libtommath/bn_mp_get_i64.c libtommath/bn_mp_get_l.c libtommath/bn_mp_get_ll.c libtommath/bn_mp_get_mag_u32.c libtommath/bn_mp_get_mag_u64.c libtommath/bn_mp_get_mag_ul.c libtommath/bn_mp_get_mag_ull.c libtommath/bn_mp_grow.c libtommath/bn_mp_incr.c libtommath/bn_mp_init.c libtommath/bn_mp_init_copy.c libtommath/bn_mp_init_i32.c libtommath/bn_mp_init_i64.c libtommath/bn_mp_init_l.c libtommath/bn_mp_init_ll.c libtommath/bn_mp_init_multi.c libtommath/bn_mp_init_set.c libtommath/bn_mp_init_size.c libtommath/bn_mp_init_u32.c libtommath/bn_mp_init_u64.c libtommath/bn_mp_init_ul.c libtommath/bn_mp_init_ull.c libtommath/bn_mp_invmod.c libtommath/bn_mp_is_square.c libtommath/bn_mp_iseven.c libtommath/bn_mp_isodd.c libtommath/bn_mp_kronecker.c libtommath/bn_mp_lcm.c libtommath/bn_mp_log_u32.c libtommath/bn_mp_lshd.c libtommath/bn_mp_mod.c libtommath/bn_mp_mod_2d.c libtommath/bn_mp_mod_d.c libtommath/bn_mp_montgomery_calc_normalization.c libtommath/bn_mp_montgomery_reduce.c libtommath/bn_mp_montgomery_setup.c libtommath/bn_mp_mul.c libtommath/bn_mp_mul_2.c libtommath/bn_mp_mul_2d.c libtommath/bn_mp_mul_d.c libtommath/bn_mp_mulmod.c libtommath/bn_mp_neg.c libtommath/bn_mp_or.c libtommath/bn_mp_pack.c libtommath/bn_mp_pack_count.c libtommath/bn_mp_prime_fermat.c libtommath/bn_mp_prime_frobenius_underwood.c libtommath/bn_mp_prime_is_prime.c libtommath/bn_mp_prime_miller_rabin.c libtommath/bn_mp_prime_next_prime.c libtommath/bn_mp_prime_rabin_miller_trials.c libtommath/bn_mp_prime_rand.c libtommath/bn_mp_prime_strong_lucas_selfridge.c libtommath/bn_mp_radix_size.c libtommath/bn_mp_radix_smap.c libtommath/bn_mp_rand.c libtommath/bn_mp_read_radix.c libtommath/bn_mp_reduce.c libtommath/bn_mp_reduce_2k.c libtommath/bn_mp_reduce_2k_l.c libtommath/bn_mp_reduce_2k_setup.c libtommath/bn_mp_reduce_2k_setup_l.c libtommath/bn_mp_reduce_is_2k.c libtommath/bn_mp_reduce_is_2k_l.c libtommath/bn_mp_reduce_setup.c libtommath/bn_mp_root_u32.c libtommath/bn_mp_rshd.c libtommath/bn_mp_sbin_size.c libtommath/bn_mp_set.c libtommath/bn_mp_set_double.c libtommath/bn_mp_set_i32.c libtommath/bn_mp_set_i64.c libtommath/bn_mp_set_l.c libtommath/bn_mp_set_ll.c libtommath/bn_mp_set_u32.c libtommath/bn_mp_set_u64.c libtommath/bn_mp_set_ul.c libtommath/bn_mp_set_ull.c libtommath/bn_mp_shrink.c libtommath/bn_mp_signed_rsh.c libtommath/bn_mp_sqr.c libtommath/bn_mp_sqrmod.c libtommath/bn_mp_sqrt.c libtommath/bn_mp_sqrtmod_prime.c libtommath/bn_mp_sub.c libtommath/bn_mp_sub_d.c libtommath/bn_mp_submod.c libtommath/bn_mp_to_radix.c libtommath/bn_mp_to_sbin.c libtommath/bn_mp_to_ubin.c libtommath/bn_mp_ubin_size.c libtommath/bn_mp_unpack.c libtommath/bn_mp_xor.c libtommath/bn_mp_zero.c libtommath/bn_prime_tab.c libtommath/bn_s_mp_add.c libtommath/bn_s_mp_balance_mul.c libtommath/bn_s_mp_exptmod.c libtommath/bn_s_mp_exptmod_fast.c libtommath/bn_s_mp_get_bit.c libtommath/bn_s_mp_invmod_fast.c libtommath/bn_s_mp_invmod_slow.c libtommath/bn_s_mp_karatsuba_mul.c libtommath/bn_s_mp_karatsuba_sqr.c libtommath/bn_s_mp_montgomery_reduce_fast.c libtommath/bn_s_mp_mul_digs.c libtommath/bn_s_mp_mul_digs_fast.c libtommath/bn_s_mp_mul_high_digs.c libtommath/bn_s_mp_mul_high_digs_fast.c libtommath/bn_s_mp_prime_is_divisible.c libtommath/bn_s_mp_rand_jenkins.c libtommath/bn_s_mp_rand_platform.c libtommath/bn_s_mp_reverse.c libtommath/bn_s_mp_sqr.c libtommath/bn_s_mp_sqr_fast.c libtommath/bn_s_mp_sub.c libtommath/bn_s_mp_toom_mul.c libtommath/bn_s_mp_toom_sqr.c libtommath/demo/mtest_opponent.c libtommath/demo/shared.c libtommath/demo/shared.h libtommath/demo/test.c libtommath/demo/timing.c libtommath/etc/2kprime.c libtommath/etc/drprime.c libtommath/etc/mersenne.c libtommath/etc/mont.c libtommath/etc/pprime.c libtommath/etc/tune.c libtommath/mtest/logtab.h libtommath/mtest/mpi-config.h libtommath/mtest/mpi-types.h libtommath/mtest/mpi.c libtommath/mtest/mpi.h libtommath/mtest/mtest.c libtommath/tommath.h libtommath/tommath_class.h libtommath/tommath_cutoffs.h libtommath/tommath_private.h libtommath/tommath_superclass.h linenoise/LICENSE linenoise/Makefile linenoise/README.markdown linenoise/example.c linenoise/linenoise.c linenoise/linenoise.h ucd2c/Makefile ucd2c/UCD.zip ucd2c/UCD/ArabicShaping.txt ucd2c/UCD/BidiBrackets.txt ucd2c/UCD/BidiCharacterTest.txt ucd2c/UCD/BidiMirroring.txt ucd2c/UCD/BidiTest.txt ucd2c/UCD/Blocks.txt ucd2c/UCD/CJKRadicals.txt ucd2c/UCD/CaseFolding.txt ucd2c/UCD/CompositionExclusions.txt ucd2c/UCD/DerivedAge.txt ucd2c/UCD/DerivedCoreProperties.txt ucd2c/UCD/DerivedNormalizationProps.txt ucd2c/UCD/EastAsianWidth.txt ucd2c/UCD/EmojiSources.txt ucd2c/UCD/EquivalentUnifiedIdeograph.txt ucd2c/UCD/HangulSyllableType.txt ucd2c/UCD/Index.txt ucd2c/UCD/IndicPositionalCategory.txt ucd2c/UCD/IndicSyllabicCategory.txt ucd2c/UCD/Jamo.txt ucd2c/UCD/LineBreak.txt ucd2c/UCD/NameAliases.txt ucd2c/UCD/NamedSequences.txt ucd2c/UCD/NamedSequencesProv.txt ucd2c/UCD/NamesList.html ucd2c/UCD/NamesList.txt ucd2c/UCD/NormalizationCorrections.txt ucd2c/UCD/NormalizationTest.txt ucd2c/UCD/NushuSources.txt ucd2c/UCD/PropList.txt ucd2c/UCD/PropertyAliases.txt ucd2c/UCD/PropertyValueAliases.txt ucd2c/UCD/ReadMe.txt ucd2c/UCD/ScriptExtensions.txt ucd2c/UCD/Scripts.txt ucd2c/UCD/SpecialCasing.txt ucd2c/UCD/StandardizedVariants.txt ucd2c/UCD/TangutSources.txt ucd2c/UCD/USourceData.txt ucd2c/UCD/USourceGlyphs.pdf ucd2c/UCD/USourceRSChart.pdf ucd2c/UCD/UnicodeData.txt ucd2c/UCD/VerticalOrientation.txt ucd2c/UCD/auxiliary/GraphemeBreakProperty.txt ucd2c/UCD/auxiliary/GraphemeBreakTest.html ucd2c/UCD/auxiliary/GraphemeBreakTest.txt ucd2c/UCD/auxiliary/LineBreakTest.html ucd2c/UCD/auxiliary/LineBreakTest.txt ucd2c/UCD/auxiliary/SentenceBreakProperty.txt ucd2c/UCD/auxiliary/SentenceBreakTest.html ucd2c/UCD/auxiliary/SentenceBreakTest.txt ucd2c/UCD/auxiliary/WordBreakProperty.txt ucd2c/UCD/auxiliary/WordBreakTest.html ucd2c/UCD/auxiliary/WordBreakTest.txt ucd2c/UCD/emoji/ReadMe.txt ucd2c/UCD/emoji/emoji-data.txt ucd2c/UCD/emoji/emoji-variation-sequences.txt ucd2c/UCD/extracted/DerivedBidiClass.txt ucd2c/UCD/extracted/DerivedBinaryProperties.txt ucd2c/UCD/extracted/DerivedCombiningClass.txt ucd2c/UCD/extracted/DerivedDecompositionType.txt ucd2c/UCD/extracted/DerivedEastAsianWidth.txt ucd2c/UCD/extracted/DerivedGeneralCategory.txt ucd2c/UCD/extracted/DerivedJoiningGroup.txt ucd2c/UCD/extracted/DerivedJoiningType.txt ucd2c/UCD/extracted/DerivedLineBreak.txt ucd2c/UCD/extracted/DerivedName.txt ucd2c/UCD/extracted/DerivedNumericType.txt ucd2c/UCD/extracted/DerivedNumericValues.txt ucd2c/config.mk ucd2c/configure ucd2c/license.txt ucd2c/ucd.c ucd2c/ucd.h ucd2c/ucd2c ucd2c/ucd2c.c ucd2c/ucd2c.lo ucd2c/ucd2c.o '
