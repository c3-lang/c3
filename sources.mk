# sources.mk generated by update_sources
C3_CONFIGURES = \
	c3c/configure \
	c3s/configure \
	c3s/update_sources \
	ic3/configure \
	ic3/update_sources \
	libc3/configure \
	libc3/update_sources \
	libtommath/configure \
	libtommath/update_sources \
	test/configure \
	test/update_sources \
	ucd2c/configure \

C3_MAKEFILES = \
	c3c/Makefile \
	c3s/Makefile \
	ic3/Makefile \
	libc3/Makefile \
	libc3/gen.mk \
	libtommath/Makefile \
	test/Makefile \
	ucd2c/Makefile \

C3_C_SOURCES = \
	c3c/c3c.c \
	c3s/buf_readline.c \
	c3s/c3s.c \
	c3s/buf_readline.h \
	ic3/buf_linenoise.c \
	ic3/ic3.c \
	ic3/buf_linenoise.h \
	ic3/linenoise.c \
	libc3/abs.c \
	libc3/abs.h \
	libc3/buf_inspect_s8.c \
	libc3/arg.c \
	libc3/arg.h \
	libc3/array.c \
	libc3/array.h \
	libc3/binding.c \
	libc3/c3.c \
	libc3/skiplist__fact.c \
	libc3/skiplist__fact.h \
	libc3/skiplist_node__fact.c \
	libc3/skiplist_node__fact.h \
	libc3/binding.h \
	libc3/bool.c \
	libc3/bool.h \
	libc3/buf.c \
	libc3/buf.h \
	libc3/buf_file.c \
	libc3/buf_file.h \
	libc3/buf_inspect.h \
	libc3/buf_parse.h \
	libc3/buf_parse_c.c \
	libc3/buf_parse_u16.c \
	libc3/buf_save.c \
	libc3/facts_spec_cursor.c \
	libc3/buf_parse_u16.h \
	libc3/buf_parse_u32.c \
	libc3/buf_parse_u32.h \
	libc3/buf_parse_u64.c \
	libc3/buf_parse_u64.h \
	libc3/buf_parse_uw.c \
	libc3/buf_parse_uw.h \
	libc3/set__fact.c \
	libc3/set__fact.h \
	libc3/set__tag.c \
	libc3/buf_parse_c.h \
	libc3/buf_save.h \
	libc3/c3.h \
	libc3/c_types.h \
	libc3/call.c \
	libc3/call.h \
	libc3/ceiling.c \
	libc3/ceiling.h \
	libc3/cfn.c \
	libc3/cfn.h \
	libc3/character.c \
	libc3/character.h \
	libc3/buf_inspect_s8_binary.c \
	libc3/compare.c \
	libc3/buf_inspect_u32.h \
	libc3/buf_inspect_u64.c \
	libc3/compare.h \
	libc3/buf_inspect_u64.h \
	libc3/buf_inspect_u64_binary.c \
	libc3/buf_inspect_u64_binary.h \
	libc3/env.c \
	libc3/buf_inspect_u64_octal.c \
	libc3/buf_inspect_u64_octal.h \
	libc3/env.h \
	libc3/error.c \
	libc3/error.h \
	libc3/error_handler.c \
	libc3/error_handler.h \
	libc3/eval.c \
	libc3/eval.h \
	libc3/fact.c \
	libc3/fact.h \
	libc3/facts.h \
	libc3/facts_cursor.c \
	libc3/buf_parse.c \
	libc3/tag.c \
	libc3/buf_inspect.c \
	libc3/facts_cursor.h \
	libc3/facts_spec.c \
	libc3/facts_spec.h \
	libc3/facts_spec_cursor.h \
	libc3/facts_with.c \
	libc3/facts_with.h \
	libc3/facts_with_cursor.c \
	libc3/facts_with_cursor.h \
	libc3/buf_inspect_u64_decimal.c \
	libc3/float.h \
	libc3/fn.c \
	libc3/buf_inspect_u64_decimal.h \
	libc3/fn.h \
	libc3/frame.c \
	libc3/buf_inspect_u64_hexadecimal.c \
	libc3/buf_inspect_u64_hexadecimal.h \
	libc3/frame.h \
	libc3/buf_inspect_uw.c \
	libc3/buf_inspect_uw.h \
	libc3/buf_parse_s8.h \
	libc3/hash.c \
	libc3/hash.h \
	libc3/ident.c \
	libc3/ident.h \
	libc3/integer.c \
	libc3/integer.h \
	libc3/io.c \
	libc3/io.h \
	libc3/buf_inspect_s8_binary.h \
	libc3/list.c \
	libc3/list.h \
	libc3/log.c \
	libc3/log.h \
	libc3/buf_inspect_s.h.in \
	libc3/module.c \
	libc3/buf_parse_s16.c \
	libc3/buf_parse_s16.h \
	libc3/buf_parse_s32.c \
	libc3/buf_parse_s32.h \
	libc3/buf_parse_s64.c \
	libc3/buf_parse_s64.h \
	libc3/buf_parse_sw.c \
	libc3/buf_parse_sw.h \
	libc3/buf_parse_u8.c \
	libc3/buf_parse_u8.h \
	libc3/facts.c \
	libc3/str.c \
	libc3/module.h \
	libc3/operator.c \
	libc3/set__tag.h \
	libc3/operator.h \
	libc3/buf_parse_u.c.in \
	libc3/quote.c \
	libc3/quote.h \
	libc3/set_cursor__fact.c \
	libc3/set_cursor__fact.h \
	libc3/set_cursor__tag.c \
	libc3/set_cursor__tag.h \
	libc3/set_item__fact.c \
	libc3/set_item__fact.h \
	libc3/set_item__tag.c \
	libc3/set_item__tag.h \
	libc3/set.c.in \
	libc3/set.h.in \
	libc3/tag.h \
	libc3/buf_inspect_s8.h \
	libc3/buf_inspect_s16.c \
	libc3/buf_inspect_s16.h \
	libc3/buf_inspect_s32.c \
	libc3/buf_inspect_s32.h \
	libc3/buf_inspect_s64.c \
	libc3/buf_inspect_s64.h \
	libc3/sha1.h \
	libc3/set_cursor.c.in \
	libc3/set_cursor.h.in \
	libc3/set_item.c.in \
	libc3/set_item.h.in \
	libc3/sign.c \
	libc3/sign.h \
	libc3/skiplist.c.in \
	libc3/skiplist.h.in \
	libc3/type.h \
	libc3/skiplist_node.c.in \
	libc3/skiplist_node.h.in \
	libc3/str.h \
	libc3/types.h \
	libc3/ucd.c \
	libc3/sym.c \
	libc3/sym.h \
	libc3/tuple.c \
	libc3/tuple.h \
	libc3/type.c \
	libc3/ucd.h \
	libc3/buf_inspect_sw.c \
	libc3/buf_parse_s.c.in \
	libc3/buf_parse_s.h.in \
	libc3/buf_inspect_sw.h \
	libc3/buf_inspect_u8.c \
	libc3/buf_inspect_u8.h \
	libc3/buf_inspect_u16.c \
	libc3/buf_inspect_u16.h \
	libc3/buf_inspect_u32.c \
	libc3/buf_inspect_s.c.in \
	libc3/buf_parse_s8.c \
	libc3/buf_parse_u.h.in \
	libc3/buf_inspect_s8_octal.c \
	libc3/buf_inspect_s8_octal.h \
	libc3/buf_inspect_s8_decimal.c \
	libc3/buf_inspect_s8_decimal.h \
	libc3/buf_inspect_s8_hexadecimal.c \
	libc3/buf_inspect_s_base.c.in \
	libc3/buf_inspect_s8_hexadecimal.h \
	libc3/buf_inspect_u.c.in \
	libc3/buf_inspect_u.h.in \
	libc3/buf_inspect_s16_binary.c \
	libc3/buf_inspect_s16_binary.h \
	libc3/buf_inspect_s16_octal.c \
	libc3/buf_inspect_s16_octal.h \
	libc3/buf_inspect_s16_decimal.c \
	libc3/buf_inspect_s16_decimal.h \
	libc3/buf_inspect_s16_hexadecimal.c \
	libc3/buf_inspect_s16_hexadecimal.h \
	libc3/buf_inspect_s32_binary.c \
	libc3/buf_inspect_s32_binary.h \
	libc3/buf_inspect_s32_octal.c \
	libc3/buf_inspect_s32_octal.h \
	libc3/buf_inspect_s32_decimal.c \
	libc3/buf_inspect_s32_decimal.h \
	libc3/buf_inspect_u_base.c.in \
	libc3/buf_inspect_u_base.h.in \
	libc3/buf_inspect_s32_hexadecimal.c \
	libc3/buf_inspect_s32_hexadecimal.h \
	libc3/buf_inspect_s64_binary.c \
	libc3/buf_inspect_s64_binary.h \
	libc3/buf_inspect_s64_octal.c \
	libc3/buf_inspect_s64_octal.h \
	libc3/buf_inspect_s64_decimal.c \
	libc3/buf_inspect_s64_decimal.h \
	libc3/buf_inspect_s64_hexadecimal.c \
	libc3/buf_inspect_s64_hexadecimal.h \
	libc3/buf_inspect_sw_binary.c \
	libc3/buf_inspect_sw_binary.h \
	libc3/buf_inspect_sw_octal.c \
	libc3/buf_inspect_sw_octal.h \
	libc3/buf_inspect_sw_decimal.c \
	libc3/buf_inspect_sw_decimal.h \
	libc3/buf_inspect_sw_hexadecimal.c \
	libc3/buf_inspect_sw_hexadecimal.h \
	libc3/buf_inspect_u16_binary.c \
	libc3/buf_inspect_u16_binary.h \
	libc3/buf_inspect_u16_octal.c \
	libc3/buf_inspect_u16_octal.h \
	libc3/buf_inspect_u16_decimal.c \
	libc3/buf_inspect_u16_decimal.h \
	libc3/buf_inspect_u16_hexadecimal.c \
	libc3/buf_inspect_u16_hexadecimal.h \
	libc3/buf_inspect_u32_binary.c \
	libc3/buf_inspect_u32_binary.h \
	libc3/buf_inspect_u32_octal.c \
	libc3/buf_inspect_u32_octal.h \
	libc3/buf_inspect_u32_decimal.c \
	libc3/buf_inspect_u32_decimal.h \
	libc3/buf_inspect_u32_hexadecimal.c \
	libc3/buf_inspect_u32_hexadecimal.h \
	libc3/buf_inspect_uw_binary.c \
	libc3/buf_inspect_uw_binary.h \
	libc3/buf_inspect_uw_octal.c \
	libc3/buf_inspect_uw_octal.h \
	libc3/buf_inspect_uw_decimal.c \
	libc3/buf_inspect_uw_decimal.h \
	libc3/buf_inspect_uw_hexadecimal.c \
	libc3/buf_inspect_uw_hexadecimal.h \
	test/bool_test.c \
	test/buf_file_test.c \
	test/buf_inspect_test.c \
	test/buf_parse_test_u8.c \
	test/buf_parse_test.h \
	test/buf_parse_test_s16.c \
	test/buf_parse_test_s32.c \
	test/list_test.c \
	test/buf_parse_test_s64.c \
	test/buf_parse_test_s8.c \
	test/buf_parse_test_su.h \
	test/buf_parse_test_u16.c \
	test/buf_parse_test_u32.c \
	test/buf_test.c \
	test/buf_parse_test_u64.c \
	test/call_test.c \
	test/facts_cursor_test.c \
	test/test.c \
	test/cfn_test.c \
	test/character_test.c \
	test/compare_test.c \
	test/compare_test.h \
	test/env_test.c \
	test/fact_test.c \
	test/fact_test.h \
	test/facts_test.c \
	test/buf_parse_test.c \
	test/facts_with_test.c \
	test/hash_test.c \
	test/ident_test.c \
	test/libc3_test.c \
	test/set__fact_test.c \
	test/set__tag_test.c \
	test/skiplist__fact_test.c \
	test/str_test.c \
	test/sym_test.c \
	test/tag_test.c \
	test/tag_test.h \
	test/test.h \
	test/tuple_test.c \
	test/types_test.c \
	ucd2c/ucd.h \
	ucd2c/ucd2c.c \

