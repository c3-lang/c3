require 'fileutils'

REPLACEMENTS = [['bool_test.c', 'TEST_CASE(test__bool_compat)', 'TEST_CASE(bool_compat)'],
                ['bool_test.c', 'TEST_CASE(test__bool_inspect)', 'TEST_CASE(bool_inspect)'],
                ['buf_file_test.c', 'TEST_CASE(test__buf_file_open_r_close)', 'TEST_CASE(buf_file_open_r_close)'],
                ['buf_file_test.c', 'TEST_CASE(test__buf_file_open_r_refill)', 'TEST_CASE(buf_file_open_r_refill)'],
                ['buf_file_test.c', 'TEST_CASE(test__buf_file_open_w_close)', 'TEST_CASE(buf_file_open_w_close)'],
                ['buf_inspect_test.c', 'TEST_CASE(test__buf_inspect_bool)', 'TEST_CASE(buf_inspect_bool)'],
                ['buf_inspect_test.c', 'TEST_CASE(test__buf_inspect_character)', 'TEST_CASE(buf_inspect_character)'],
                ['buf_inspect_test.c', 'TEST_CASE(test__buf_inspect_f32)', 'TEST_CASE(buf_inspect_f32)'],
                ['buf_inspect_test.c', 'TEST_CASE(test__buf_inspect_f64)', 'TEST_CASE(buf_inspect_f64)'],
                ['buf_inspect_test.c', 'TEST_CASE(test__buf_inspect_integer)', 'TEST_CASE(buf_inspect_integer)'],
                ['buf_inspect_test.c', 'TEST_CASE(test__buf_inspect_list)', 'TEST_CASE(buf_inspect_list)'],
                ['buf_inspect_test.c', 'TEST_CASE(test__buf_inspect_str)', 'TEST_CASE(buf_inspect_str)'],
                ['buf_inspect_test.c', 'TEST_CASE(test__buf_inspect_str_character)', 'TEST_CASE(buf_inspect_str_character)'],
                ['buf_inspect_test.c', 'TEST_CASE(test__buf_inspect_str_character_size)', 'TEST_CASE(buf_inspect_str_character_size)'],
                ['buf_inspect_test.c', 'TEST_CASE(test__buf_inspect_tag)', 'TEST_CASE(buf_inspect_tag)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_bool)', 'TEST_CASE(buf_parse_bool)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_call)', 'TEST_CASE(buf_parse_call)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_call_op)', 'TEST_CASE(buf_parse_call_op)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_cfn)', 'TEST_CASE(buf_parse_cfn)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_character)', 'TEST_CASE(buf_parse_character)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_digit_bin)', 'TEST_CASE(buf_parse_digit_bin)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_digit_hex)', 'TEST_CASE(buf_parse_digit_hex)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_digit_oct)', 'TEST_CASE(buf_parse_digit_oct)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_digit_dec)', 'TEST_CASE(buf_parse_digit_dec)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_fn)', 'TEST_CASE(buf_parse_fn)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_ident)', 'TEST_CASE(buf_parse_ident)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_integer)', 'TEST_CASE(buf_parse_integer)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_integer_bin)', 'TEST_CASE(buf_parse_integer_bin)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_list)', 'TEST_CASE(buf_parse_list)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_sw)', 'TEST_CASE(buf_parse_sw)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_str)', 'TEST_CASE(buf_parse_str)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_str_character)', 'TEST_CASE(buf_parse_str_character)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_str_u8)', 'TEST_CASE(buf_parse_str_u8)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_sym)', 'TEST_CASE(buf_parse_sym)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_tag)', 'TEST_CASE(buf_parse_tag)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_tuple)', 'TEST_CASE(buf_parse_tuple)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_u)', 'TEST_CASE(buf_parse_u)'],
                ['buf_parse_test.c', 'TEST_CASE(test__buf_parse_uw)', 'TEST_CASE(buf_parse_uw)'],
                ['buf_parse_test_s16.c', 'TEST_CASE(test__buf_parse_s16_binary)', 'TEST_CASE(buf_parse_s16_binary)'],
                ['buf_parse_test_s16.c', 'TEST_CASE(test__buf_parse_s16_binary_negative)', 'TEST_CASE(buf_parse_s16_binary_negative)'],
                ['buf_parse_test_s16.c', 'TEST_CASE(test__buf_parse_s16_octal)', 'TEST_CASE(buf_parse_s16_octal)'],
                ['buf_parse_test_s16.c', 'TEST_CASE(test__buf_parse_s16_octal_negative)', 'TEST_CASE(buf_parse_s16_octal_negative)'],
                ['buf_parse_test_s16.c', 'TEST_CASE(test__buf_parse_s16_hexadecimal)', 'TEST_CASE(buf_parse_s16_hexadecimal)'],
                ['buf_parse_test_s16.c', 'TEST_CASE(test__buf_parse_s16_hexadecimal_negative)', 'TEST_CASE(buf_parse_s16_hexadecimal_negative)'],
                ['buf_parse_test_s16.c', 'TEST_CASE(test__buf_parse_s16_decimal)', 'TEST_CASE(buf_parse_s16_decimal)'],
                ['buf_parse_test_s16.c', 'TEST_CASE(test__buf_parse_s16_decimal_negative)', 'TEST_CASE(buf_parse_s16_decimal_negative)'],
                ['buf_parse_test_s32.c', 'TEST_CASE(test__buf_parse_s32_binary)', 'TEST_CASE(buf_parse_s32_binary)'],
                ['buf_parse_test_s32.c', 'TEST_CASE(test__buf_parse_s32_binary_negative)', 'TEST_CASE(buf_parse_s32_binary_negative)'],
                ['buf_parse_test_s32.c', 'TEST_CASE(test__buf_parse_s32_octal)', 'TEST_CASE(buf_parse_s32_octal)'],
                ['buf_parse_test_s32.c', 'TEST_CASE(test__buf_parse_s32_octal_negative)', 'TEST_CASE(buf_parse_s32_octal_negative)'],
                ['buf_parse_test_s32.c', 'TEST_CASE(test__buf_parse_s32_hexadecimal)', 'TEST_CASE(buf_parse_s32_hexadecimal)'],
                ['buf_parse_test_s32.c', 'TEST_CASE(test__buf_parse_s32_hexadecimal_negative)', 'TEST_CASE(buf_parse_s32_hexadecimal_negative)'],
                ['buf_parse_test_s32.c', 'TEST_CASE(test__buf_parse_s32_decimal)', 'TEST_CASE(buf_parse_s32_decimal)'],
                ['buf_parse_test_s32.c', 'TEST_CASE(test__buf_parse_s32_decimal_negative)', 'TEST_CASE(buf_parse_s32_decimal_negative)'],
                ['buf_parse_test_s64.c', 'TEST_CASE(test__buf_parse_s64_binary)', 'TEST_CASE(buf_parse_s64_binary)'],
                ['buf_parse_test_s64.c', 'TEST_CASE(test__buf_parse_s64_binary_negative)', 'TEST_CASE(buf_parse_s64_binary_negative)'],
                ['buf_parse_test_s64.c', 'TEST_CASE(test__buf_parse_s64_octal)', 'TEST_CASE(buf_parse_s64_octal)'],
                ['buf_parse_test_s64.c', 'TEST_CASE(test__buf_parse_s64_octal_negative)', 'TEST_CASE(buf_parse_s64_octal_negative)'],
                ['buf_parse_test_s64.c', 'TEST_CASE(test__buf_parse_s64_hexadecimal)', 'TEST_CASE(buf_parse_s64_hexadecimal)'],
                ['buf_parse_test_s64.c', 'TEST_CASE(test__buf_parse_s64_hexadecimal_negative)', 'TEST_CASE(buf_parse_s64_hexadecimal_negative)'],
                ['buf_parse_test_s64.c', 'TEST_CASE(test__buf_parse_s64_decimal)', 'TEST_CASE(buf_parse_s64_decimal)'],
                ['buf_parse_test_s64.c', 'TEST_CASE(test__buf_parse_s64_decimal_negative)', 'TEST_CASE(buf_parse_s64_decimal_negative)'],
                ['buf_parse_test_s8.c', 'TEST_CASE(test__buf_parse_s8_binary)', 'TEST_CASE(buf_parse_s8_binary)'],
                ['buf_parse_test_s8.c', 'TEST_CASE(test__buf_parse_s8_binary_negative)', 'TEST_CASE(buf_parse_s8_binary_negative)'],
                ['buf_parse_test_s8.c', 'TEST_CASE(test__buf_parse_s8_octal)', 'TEST_CASE(buf_parse_s8_octal)'],
                ['buf_parse_test_s8.c', 'TEST_CASE(test__buf_parse_s8_octal_negative)', 'TEST_CASE(buf_parse_s8_octal_negative)'],
                ['buf_parse_test_s8.c', 'TEST_CASE(test__buf_parse_s8_hexadecimal)', 'TEST_CASE(buf_parse_s8_hexadecimal)'],
                ['buf_parse_test_s8.c', 'TEST_CASE(test__buf_parse_s8_hexadecimal_negative)', 'TEST_CASE(buf_parse_s8_hexadecimal_negative)'],
                ['buf_parse_test_s8.c', 'TEST_CASE(test__buf_parse_s8_decimal)', 'TEST_CASE(buf_parse_s8_decimal)'],
                ['buf_parse_test_s8.c', 'TEST_CASE(test__buf_parse_s8_decimal_negative)', 'TEST_CASE(buf_parse_s8_decimal_negative)'],
                ['buf_parse_test_u16.c', 'TEST_CASE(test__buf_parse_u16_binary)', 'TEST_CASE(buf_parse_u16_binary)'],
                ['buf_parse_test_u16.c', 'TEST_CASE(test__buf_parse_u16_octal)', 'TEST_CASE(buf_parse_u16_octal)'],
                ['buf_parse_test_u16.c', 'TEST_CASE(test__buf_parse_u16_hexadecimal)', 'TEST_CASE(buf_parse_u16_hexadecimal)'],
                ['buf_parse_test_u16.c', 'TEST_CASE(test__buf_parse_u16_decimal)', 'TEST_CASE(buf_parse_u16_decimal)'],
                ['buf_parse_test_u32.c', 'TEST_CASE(test__buf_parse_u32_binary)', 'TEST_CASE(buf_parse_u32_binary)'],
                ['buf_parse_test_u32.c', 'TEST_CASE(test__buf_parse_u32_octal)', 'TEST_CASE(buf_parse_u32_octal)'],
                ['buf_parse_test_u32.c', 'TEST_CASE(test__buf_parse_u32_hexadecimal)', 'TEST_CASE(buf_parse_u32_hexadecimal)'],
                ['buf_parse_test_u32.c', 'TEST_CASE(test__buf_parse_u32_decimal)', 'TEST_CASE(buf_parse_u32_decimal)'],
                ['buf_parse_test_u64.c', 'TEST_CASE(test__buf_parse_u64_binary)', 'TEST_CASE(buf_parse_u64_binary)'],
                ['buf_parse_test_u64.c', 'TEST_CASE(test__buf_parse_u64_octal)', 'TEST_CASE(buf_parse_u64_octal)'],
                ['buf_parse_test_u64.c', 'TEST_CASE(test__buf_parse_u64_hexadecimal)', 'TEST_CASE(buf_parse_u64_hexadecimal)'],
                ['buf_parse_test_u64.c', 'TEST_CASE(test__buf_parse_u64_decimal)', 'TEST_CASE(buf_parse_u64_decimal)'],
                ['buf_parse_test_u8.c', 'TEST_CASE(test__buf_parse_u8_binary)', 'TEST_CASE(buf_parse_u8_binary)'],
                ['buf_parse_test_u8.c', 'TEST_CASE(test__buf_parse_u8_octal)', 'TEST_CASE(buf_parse_u8_octal)'],
                ['buf_parse_test_u8.c', 'TEST_CASE(test__buf_parse_u8_hexadecimal)', 'TEST_CASE(buf_parse_u8_hexadecimal)'],
                ['buf_parse_test_u8.c', 'TEST_CASE(test__buf_parse_u8_decimal)', 'TEST_CASE(buf_parse_u8_decimal)'],
                ['buf_test.c', 'TEST_CASE(test__buf_f)', 'TEST_CASE(buf_f)'],
                ['buf_test.c', 'TEST_CASE(test__buf_ignore)', 'TEST_CASE(buf_ignore)'],
                ['buf_test.c', 'TEST_CASE(test__buf_init_clean)', 'TEST_CASE(buf_init_clean)'],
                ['buf_test.c', 'TEST_CASE(test__buf_new_delete)', 'TEST_CASE(buf_new_delete)'],
                ['buf_test.c', 'TEST_CASE(test__buf_new_alloc_delete)', 'TEST_CASE(buf_new_alloc_delete)'],
                ['buf_test.c', 'TEST_CASE(test__buf_peek_1)', 'TEST_CASE(buf_peek_1)'],
                ['buf_test.c', 'TEST_CASE(test__buf_peek_character_utf8)', 'TEST_CASE(buf_peek_character_utf8)'],
                ['buf_test.c', 'TEST_CASE(test__buf_peek_s8)', 'TEST_CASE(buf_peek_s8)'],
                ['buf_test.c', 'TEST_CASE(test__buf_peek_s16)', 'TEST_CASE(buf_peek_s16)'],
                ['buf_test.c', 'TEST_CASE(test__buf_peek_u8)', 'TEST_CASE(buf_peek_u8)'],
                ['buf_test.c', 'TEST_CASE(test__buf_read_1)', 'TEST_CASE(buf_read_1)'],
                ['buf_test.c', 'TEST_CASE(test__buf_read_character_utf8)', 'TEST_CASE(buf_read_character_utf8)'],
                ['buf_test.c', 'TEST_CASE(test__buf_read_f64)', 'TEST_CASE(buf_read_f64)'],
                ['buf_test.c', 'TEST_CASE(test__buf_read_s8)', 'TEST_CASE(buf_read_s8)'],
                ['buf_test.c', 'TEST_CASE(test__buf_read_s16)', 'TEST_CASE(buf_read_s16)'],
                ['buf_test.c', 'TEST_CASE(test__buf_read_to_str)', 'TEST_CASE(buf_read_to_str)'],
                ['buf_test.c', 'TEST_CASE(test__buf_read_u8)', 'TEST_CASE(buf_read_u8)'],
                ['buf_test.c', 'TEST_CASE(test__buf_write_s8)', 'TEST_CASE(buf_write_s8)'],
                ['buf_test.c', 'TEST_CASE(test__buf_write_s16)', 'TEST_CASE(buf_write_s16)'],
                ['buf_test.c', 'TEST_CASE(test__buf_write_s32)', 'TEST_CASE(buf_write_s32)'],
                ['buf_test.c', 'TEST_CASE(test__buf_write_s64)', 'TEST_CASE(buf_write_s64)'],
                ['buf_test.c', 'TEST_CASE(test__buf_write_u8)', 'TEST_CASE(buf_write_u8)'],
                ['buf_test.c', 'TEST_CASE(test__buf_write_u16)', 'TEST_CASE(buf_write_u16)'],
                ['buf_test.c', 'TEST_CASE(test__buf_write_u32)', 'TEST_CASE(buf_write_u32)'],
                ['buf_test.c', 'TEST_CASE(test__buf_write_u64)', 'TEST_CASE(buf_write_u64)'],
                ['buf_test.c', 'TEST_CASE(test__buf_write_str)', 'TEST_CASE(buf_write_str)'],
                ['buf_test.c', 'TEST_CASE(test__buf_xfer)', 'TEST_CASE(buf_xfer)'],
                ['call_test.c', 'TEST_CASE(test__call_init_1)', 'TEST_CASE(call_init_1)'],
                ['call_test.c', 'TEST_CASE(test__call_inspect)', 'TEST_CASE(call_inspect)'],
                ['cfn_test.c', 'TEST_CASE(test__cfn_apply)', 'TEST_CASE(cfn_apply)'],
                ['cfn_test.c', 'TEST_CASE(test__cfn_copy)', 'TEST_CASE(cfn_copy)'],
                ['cfn_test.c', 'TEST_CASE(test__cfn_init_clean)', 'TEST_CASE(cfn_init_clean)'],
                ['cfn_test.c', 'TEST_CASE(test__cfn_link)', 'TEST_CASE(cfn_link)'],
                ['cfn_test.c', 'TEST_CASE(test__cfn_prep_cif)', 'TEST_CASE(cfn_prep_cif)'],
                ['character_test.c', 'TEST_CASE(test__character_1)', 'TEST_CASE(character_1)'],
                ['character_test.c', 'TEST_CASE(test__character_is_digit)', 'TEST_CASE(character_is_digit)'],
                ['character_test.c', 'TEST_CASE(test__character_is_lowercase)', 'TEST_CASE(character_is_lowercase)'],
                ['character_test.c', 'TEST_CASE(test__character_is_space)', 'TEST_CASE(character_is_space)'],
                ['character_test.c', 'TEST_CASE(test__character_is_uppercase)', 'TEST_CASE(character_is_uppercase)'],
                ['character_test.c', 'TEST_CASE(test__character_utf8)', 'TEST_CASE(character_utf8)'],
                ['character_test.c', 'TEST_CASE(test__character_utf8_size)', 'TEST_CASE(character_utf8_size)'],
                ['compare_test.c', 'TEST_CASE(test__compare_bool)', 'TEST_CASE(compare_bool)'],
                ['compare_test.c', 'TEST_CASE(test__compare_character)', 'TEST_CASE(compare_character)'],
                ['compare_test.c', 'TEST_CASE(test__compare_f32)', 'TEST_CASE(compare_f32)'],
                ['compare_test.c', 'TEST_CASE(test__compare_f64)', 'TEST_CASE(compare_f64)'],
                ['compare_test.c', 'TEST_CASE(test__compare_list)', 'TEST_CASE(compare_list)'],
                ['compare_test.c', 'TEST_CASE(test__compare_str)', 'TEST_CASE(compare_str)'],
                ['compare_test.c', 'TEST_CASE(test__compare_tag)', 'TEST_CASE(compare_tag)'],
                ['compare_test.c', 'TEST_CASE(test__compare_tuple)', 'TEST_CASE(compare_tuple)'],
                ['env_test.c', 'TEST_CASE(test__env_eval_equal_tag)', 'TEST_CASE(env_eval_equal_tag)'],
                ['env_test.c', 'TEST_CASE(test__env_init_clean)', 'TEST_CASE(env_init_clean)'],
                ['fact_test.c', 'TEST_CASE(test__fact_init)', 'TEST_CASE(fact_init)'],
                ['fact_test.c', 'TEST_CASE(test__fact_inspect)', 'TEST_CASE(fact_inspect)'],
                ['facts_cursor_test.c', 'TEST_CASE(test__facts_cursor_init)', 'TEST_CASE(facts_cursor_init)'],
                ['facts_cursor_test.c', 'TEST_CASE(test__facts_cursor_next)', 'TEST_CASE(facts_cursor_next)'],
                ['facts_test.c', 'TEST_CASE(test__facts_add)', 'TEST_CASE(facts_add)'],
                ['facts_test.c', 'TEST_CASE(test__facts_dump_file)', 'TEST_CASE(facts_dump_file)'],
                ['facts_test.c', 'TEST_CASE(test__facts_find)', 'TEST_CASE(facts_find)'],
                ['facts_test.c', 'TEST_CASE(test__facts_init_clean)', 'TEST_CASE(facts_init_clean)'],
                ['facts_test.c', 'TEST_CASE(test__facts_load)', 'TEST_CASE(facts_load)'],
                ['facts_test.c', 'TEST_CASE(test__facts_log_add)', 'TEST_CASE(facts_log_add)'],
                ['facts_test.c', 'TEST_CASE(test__facts_log_remove)', 'TEST_CASE(facts_log_remove)'],
                ['facts_test.c', 'TEST_CASE(test__facts_new_delete)', 'TEST_CASE(facts_new_delete)'],
                ['facts_test.c', 'TEST_CASE(test__facts_open_file)', 'TEST_CASE(facts_open_file)'],
                ['facts_test.c', 'TEST_CASE(test__facts_remove)', 'TEST_CASE(facts_remove)'],
                ['facts_test.c', 'TEST_CASE(test__facts_save)', 'TEST_CASE(facts_save)'],
                ['facts_with_test.c', 'TEST_CASE(test__facts_with_)', 'TEST_CASE(facts_with_)'],
                ['facts_with_test.c', 'TEST_CASE(test__facts_with_tags)', 'TEST_CASE(facts_with_tags)'],
                ['ident_test.c', 'TEST_CASE(test__ident_character_is_reserved)', 'TEST_CASE(ident_character_is_reserved)'],
                ['ident_test.c', 'TEST_CASE(test__ident_first_character_is_reserved)', 'TEST_CASE(ident_first_character_is_reserved)'],
                ['ident_test.c', 'TEST_CASE(test__ident_inspect)', 'TEST_CASE(ident_inspect)'],
                ['list_test.c', 'TEST_CASE(test__list_1)', 'TEST_CASE(list_1)'],
                ['list_test.c', 'TEST_CASE(test__list_inspect)', 'TEST_CASE(list_inspect)'],
                ['set__fact_test.c', 'TEST_CASE(test__set__fact_add)', 'TEST_CASE(set__fact_add)'],
                ['set__fact_test.c', 'TEST_CASE(test__set__fact_cursor)', 'TEST_CASE(set__fact_cursor)'],
                ['set__fact_test.c', 'TEST_CASE(test__set__fact_init_clean)', 'TEST_CASE(set__fact_init_clean)'],
                ['set__fact_test.c', 'TEST_CASE(test__set__fact_remove)', 'TEST_CASE(set__fact_remove)'],
                ['set__fact_test.c', 'TEST_CASE(test__set__fact_resize)', 'TEST_CASE(set__fact_resize)'],
                ['set__tag_test.c', 'TEST_CASE(test__set__tag_add)', 'TEST_CASE(set__tag_add)'],
                ['set__tag_test.c', 'TEST_CASE(test__set__tag_cursor)', 'TEST_CASE(set__tag_cursor)'],
                ['set__tag_test.c', 'TEST_CASE(test__set__tag_init_clean)', 'TEST_CASE(set__tag_init_clean)'],
                ['set__tag_test.c', 'TEST_CASE(test__set__tag_remove)', 'TEST_CASE(set__tag_remove)'],
                ['set__tag_test.c', 'TEST_CASE(test__set__tag_resize)', 'TEST_CASE(set__tag_resize)'],
                ['skiplist__fact_test.c', 'TEST_CASE(test__skiplist__fact_find)', 'TEST_CASE(skiplist__fact_find)'],
                ['skiplist__fact_test.c', 'TEST_CASE(test__skiplist__fact_insert)', 'TEST_CASE(skiplist__fact_insert)'],
                ['skiplist__fact_test.c', 'TEST_CASE(test__skiplist__fact_new_delete)', 'TEST_CASE(skiplist__fact_new_delete)'],
                ['skiplist__fact_test.c', 'TEST_CASE(test__skiplist__fact_remove)', 'TEST_CASE(skiplist__fact_remove)'],
                ['str_test.c', 'TEST_CASE(test__str_character_is_reserved)', 'TEST_CASE(str_character_is_reserved)'],
                ['str_test.c', 'TEST_CASE(test__str_init_clean)', 'TEST_CASE(str_init_clean)'],
                ['str_test.c', 'TEST_CASE(test__str_init_dup)', 'TEST_CASE(str_init_dup)'],
                ['str_test.c', 'TEST_CASE(test__str_init_dup_1)', 'TEST_CASE(str_init_dup_1)'],
                ['str_test.c', 'TEST_CASE(test__str_inspect)', 'TEST_CASE(str_inspect)'],
                ['str_test.c', 'TEST_CASE(test__str_new_1)', 'TEST_CASE(str_new_1)'],
                ['str_test.c', 'TEST_CASE(test__str_new_cpy)', 'TEST_CASE(str_new_cpy)'],
                ['str_test.c', 'TEST_CASE(test__str_new_delete)', 'TEST_CASE(str_new_delete)'],
                ['str_test.c', 'TEST_CASE(test__str_new_dup)', 'TEST_CASE(str_new_dup)'],
                ['str_test.c', 'TEST_CASE(test__str_new_f)', 'TEST_CASE(str_new_f)'],
                ['str_test.c', 'TEST_CASE(test__str_to_hex)', 'TEST_CASE(str_to_hex)'],
                ['str_test.c', 'TEST_CASE(test__str_to_sym)', 'TEST_CASE(str_to_sym)'],
                ['sym_test.c', 'TEST_CASE(test__sym_1)', 'TEST_CASE(sym_1)'],
                ['sym_test.c', 'TEST_CASE(test__sym_inspect)', 'TEST_CASE(sym_inspect)'],
                ['tag_test.c', 'TEST_CASE(test__tag_hash_u64)', 'TEST_CASE(tag_hash_u64)'],
                ['tag_test.c', 'TEST_CASE(test__tag_init_1)', 'TEST_CASE(tag_init_1)'],
                ['tuple_test.c', 'TEST_CASE(test__tuple_init_clean)', 'TEST_CASE(tuple_init_clean)'],
                ['tuple_test.c', 'TEST_CASE(test__tuple_init_1)', 'TEST_CASE(tuple_init_1)'],
                ['tuple_test.c', 'TEST_CASE(test__tuple_inspect)', 'TEST_CASE(tuple_inspect)'],
                ['tuple_test.c', 'TEST_CASE(test__tuple_new_1)', 'TEST_CASE(tuple_new_1)'],
                ['tuple_test.c', 'TEST_CASE(test__tuple_new_delete)', 'TEST_CASE(tuple_new_delete)'],
                ['types_test.c', 'TEST_CASE(test__types_sizeof)', 'TEST_CASE(types_sizeof)']]

files = []
replacements = {}
REPLACEMENTS.each do |r|
  file = r[0]
  if replacements[file].nil?
    files << file
    replacements[file] = []
  end
  replacements[file] << r[1..2]
end

files.each do |file|
  file_tmp = "#{file}.tmp"
  File.open(file_tmp, "w") do |out|
    File.foreach(file) do |line|
      replacements[file].each do |r|
        search = r[0]
        replace = r[1]
        line = replace + "\n" if line == search + "\n"
      end
      out.write(line)
    end
  end
  FileUtils.cp(file, file + "~")
  FileUtils.mv(file_tmp, file)
end
