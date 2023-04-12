require 'fileutils'

REPLACEMENTS = [
  ['bool_test.c', 'void bool_test_compat ()', 'TEST_CASE(test__bool_compat)'],
  ['bool_test.c', 'void bool_test_inspect ()', 'TEST_CASE(test__bool_inspect)'],
  ['buf_file_test.c', 'void buf_file_test_open_r_close ()', 'TEST_CASE(test__buf_file_open_r_close)'],
  ['buf_file_test.c', 'void buf_file_test_open_r_refill ()', 'TEST_CASE(test__buf_file_open_r_refill)'],
  ['buf_file_test.c', 'void buf_file_test_open_w_close ()', 'TEST_CASE(test__buf_file_open_w_close)'],
  ['buf_inspect_test.c', 'void buf_inspect_test_bool ()', 'TEST_CASE(test__buf_inspect_bool)'],
  ['buf_inspect_test.c', 'void buf_inspect_test_character ()', 'TEST_CASE(test__buf_inspect_character)'],
  ['buf_inspect_test.c', 'void buf_inspect_test_f32 ()', 'TEST_CASE(test__buf_inspect_f32)'],
  ['buf_inspect_test.c', 'void buf_inspect_test_f64 ()', 'TEST_CASE(test__buf_inspect_f64)'],
  ['buf_inspect_test.c', 'void buf_inspect_test_integer ()', 'TEST_CASE(test__buf_inspect_integer)'],
  ['buf_inspect_test.c', 'void buf_inspect_test_list ()', 'TEST_CASE(test__buf_inspect_list)'],
  ['buf_inspect_test.c', 'void buf_inspect_test_str ()', 'TEST_CASE(test__buf_inspect_str)'],
  ['buf_inspect_test.c', 'void buf_inspect_test_str_character ()', 'TEST_CASE(test__buf_inspect_str_character)'],
  ['buf_inspect_test.c', 'void buf_inspect_test_str_character_size ()', 'TEST_CASE(test__buf_inspect_str_character_size)'],
  ['buf_inspect_test.c', 'void buf_inspect_test_tag ()', 'TEST_CASE(test__buf_inspect_tag)'],
  ['buf_parse_test.c', 'void buf_parse_test_bool ()', 'TEST_CASE(test__buf_parse_bool)'],
  ['buf_parse_test.c', 'void buf_parse_test_call ()', 'TEST_CASE(test__buf_parse_call)'],
  ['buf_parse_test.c', 'void buf_parse_test_call_op ()', 'TEST_CASE(test__buf_parse_call_op)'],
  ['buf_parse_test.c', 'void buf_parse_test_cfn ()', 'TEST_CASE(test__buf_parse_cfn)'],
  ['buf_parse_test.c', 'void buf_parse_test_character ()', 'TEST_CASE(test__buf_parse_character)'],
  ['buf_parse_test.c', 'void buf_parse_test_digit_bin ()', 'TEST_CASE(test__buf_parse_digit_bin)'],
  ['buf_parse_test.c', 'void buf_parse_test_digit_hex ()', 'TEST_CASE(test__buf_parse_digit_hex)'],
  ['buf_parse_test.c', 'void buf_parse_test_digit_oct ()', 'TEST_CASE(test__buf_parse_digit_oct)'],
  ['buf_parse_test.c', 'void buf_parse_test_digit_dec ()', 'TEST_CASE(test__buf_parse_digit_dec)'],
  ['buf_parse_test.c', 'void buf_parse_test_fn ()', 'TEST_CASE(test__buf_parse_fn)'],
  ['buf_parse_test.c', 'void buf_parse_test_ident ()', 'TEST_CASE(test__buf_parse_ident)'],
  ['buf_parse_test.c', 'void buf_parse_test_integer ()', 'TEST_CASE(test__buf_parse_integer)'],
  ['buf_parse_test.c', 'void buf_parse_test_integer_bin ()', 'TEST_CASE(test__buf_parse_integer_bin)'],
  ['buf_parse_test.c', 'void buf_parse_test_list ()', 'TEST_CASE(test__buf_parse_list)'],
  ['buf_parse_test.c', 'void buf_parse_test_sw ()', 'TEST_CASE(test__buf_parse_sw)'],
  ['buf_parse_test.c', 'void buf_parse_test_str ()', 'TEST_CASE(test__buf_parse_str)'],
  ['buf_parse_test.c', 'void buf_parse_test_str_character ()', 'TEST_CASE(test__buf_parse_str_character)'],
  ['buf_parse_test.c', 'void buf_parse_test_str_u8 ()', 'TEST_CASE(test__buf_parse_str_u8)'],
  ['buf_parse_test.c', 'void buf_parse_test_sym ()', 'TEST_CASE(test__buf_parse_sym)'],
  ['buf_parse_test.c', 'void buf_parse_test_tag ()', 'TEST_CASE(test__buf_parse_tag)'],
  ['buf_parse_test.c', 'void buf_parse_test_tuple ()', 'TEST_CASE(test__buf_parse_tuple)'],
  ['buf_parse_test.c', 'void buf_parse_test_u ()', 'TEST_CASE(test__buf_parse_u)'],
  ['buf_parse_test.c', 'void buf_parse_test_uw ()', 'TEST_CASE(test__buf_parse_uw)'],
  ['buf_parse_test_s16.c', 'void buf_parse_test_s16_binary ()', 'TEST_CASE(test__buf_parse_s16_binary)'],
  ['buf_parse_test_s16.c', 'void buf_parse_test_s16_binary_negative ()', 'TEST_CASE(test__buf_parse_s16_binary_negative)'],
  ['buf_parse_test_s16.c', 'void buf_parse_test_s16_octal ()', 'TEST_CASE(test__buf_parse_s16_octal)'],
  ['buf_parse_test_s16.c', 'void buf_parse_test_s16_octal_negative ()', 'TEST_CASE(test__buf_parse_s16_octal_negative)'],
  ['buf_parse_test_s16.c', 'void buf_parse_test_s16_hexadecimal ()', 'TEST_CASE(test__buf_parse_s16_hexadecimal)'],
  ['buf_parse_test_s16.c', 'void buf_parse_test_s16_hexadecimal_negative ()', 'TEST_CASE(test__buf_parse_s16_hexadecimal_negative)'],
  ['buf_parse_test_s16.c', 'void buf_parse_test_s16_decimal ()', 'TEST_CASE(test__buf_parse_s16_decimal)'],
  ['buf_parse_test_s16.c', 'void buf_parse_test_s16_decimal_negative ()', 'TEST_CASE(test__buf_parse_s16_decimal_negative)'],
  ['buf_parse_test_s32.c', 'void buf_parse_test_s32_binary ()', 'TEST_CASE(test__buf_parse_s32_binary)'],
  ['buf_parse_test_s32.c', 'void buf_parse_test_s32_binary_negative ()', 'TEST_CASE(test__buf_parse_s32_binary_negative)'],
  ['buf_parse_test_s32.c', 'void buf_parse_test_s32_octal ()', 'TEST_CASE(test__buf_parse_s32_octal)'],
  ['buf_parse_test_s32.c', 'void buf_parse_test_s32_octal_negative ()', 'TEST_CASE(test__buf_parse_s32_octal_negative)'],
  ['buf_parse_test_s32.c', 'void buf_parse_test_s32_hexadecimal ()', 'TEST_CASE(test__buf_parse_s32_hexadecimal)'],
  ['buf_parse_test_s32.c', 'void buf_parse_test_s32_hexadecimal_negative ()', 'TEST_CASE(test__buf_parse_s32_hexadecimal_negative)'],
  ['buf_parse_test_s32.c', 'void buf_parse_test_s32_decimal ()', 'TEST_CASE(test__buf_parse_s32_decimal)'],
  ['buf_parse_test_s32.c', 'void buf_parse_test_s32_decimal_negative ()', 'TEST_CASE(test__buf_parse_s32_decimal_negative)'],
  ['buf_parse_test_s64.c', 'void buf_parse_test_s64_binary ()', 'TEST_CASE(test__buf_parse_s64_binary)'],
  ['buf_parse_test_s64.c', 'void buf_parse_test_s64_binary_negative ()', 'TEST_CASE(test__buf_parse_s64_binary_negative)'],
  ['buf_parse_test_s64.c', 'void buf_parse_test_s64_octal ()', 'TEST_CASE(test__buf_parse_s64_octal)'],
  ['buf_parse_test_s64.c', 'void buf_parse_test_s64_octal_negative ()', 'TEST_CASE(test__buf_parse_s64_octal_negative)'],
  ['buf_parse_test_s64.c', 'void buf_parse_test_s64_hexadecimal ()', 'TEST_CASE(test__buf_parse_s64_hexadecimal)'],
  ['buf_parse_test_s64.c', 'void buf_parse_test_s64_hexadecimal_negative ()', 'TEST_CASE(test__buf_parse_s64_hexadecimal_negative)'],
  ['buf_parse_test_s64.c', 'void buf_parse_test_s64_decimal ()', 'TEST_CASE(test__buf_parse_s64_decimal)'],
  ['buf_parse_test_s64.c', 'void buf_parse_test_s64_decimal_negative ()', 'TEST_CASE(test__buf_parse_s64_decimal_negative)'],
  ['buf_parse_test_s8.c', 'void buf_parse_test_s8_binary ()', 'TEST_CASE(test__buf_parse_s8_binary)'],
  ['buf_parse_test_s8.c', 'void buf_parse_test_s8_binary_negative ()', 'TEST_CASE(test__buf_parse_s8_binary_negative)'],
  ['buf_parse_test_s8.c', 'void buf_parse_test_s8_octal ()', 'TEST_CASE(test__buf_parse_s8_octal)'],
  ['buf_parse_test_s8.c', 'void buf_parse_test_s8_octal_negative ()', 'TEST_CASE(test__buf_parse_s8_octal_negative)'],
  ['buf_parse_test_s8.c', 'void buf_parse_test_s8_hexadecimal ()', 'TEST_CASE(test__buf_parse_s8_hexadecimal)'],
  ['buf_parse_test_s8.c', 'void buf_parse_test_s8_hexadecimal_negative ()', 'TEST_CASE(test__buf_parse_s8_hexadecimal_negative)'],
  ['buf_parse_test_s8.c', 'void buf_parse_test_s8_decimal ()', 'TEST_CASE(test__buf_parse_s8_decimal)'],
  ['buf_parse_test_s8.c', 'void buf_parse_test_s8_decimal_negative ()', 'TEST_CASE(test__buf_parse_s8_decimal_negative)'],
  ['buf_parse_test_u16.c', 'void buf_parse_test_u16_binary ()', 'TEST_CASE(test__buf_parse_u16_binary)'],
  ['buf_parse_test_u16.c', 'void buf_parse_test_u16_octal ()', 'TEST_CASE(test__buf_parse_u16_octal)'],
  ['buf_parse_test_u16.c', 'void buf_parse_test_u16_hexadecimal ()', 'TEST_CASE(test__buf_parse_u16_hexadecimal)'],
  ['buf_parse_test_u16.c', 'void buf_parse_test_u16_decimal ()', 'TEST_CASE(test__buf_parse_u16_decimal)'],
  ['buf_parse_test_u32.c', 'void buf_parse_test_u32_binary ()', 'TEST_CASE(test__buf_parse_u32_binary)'],
  ['buf_parse_test_u32.c', 'void buf_parse_test_u32_octal ()', 'TEST_CASE(test__buf_parse_u32_octal)'],
  ['buf_parse_test_u32.c', 'void buf_parse_test_u32_hexadecimal ()', 'TEST_CASE(test__buf_parse_u32_hexadecimal)'],
  ['buf_parse_test_u32.c', 'void buf_parse_test_u32_decimal ()', 'TEST_CASE(test__buf_parse_u32_decimal)'],
  ['buf_parse_test_u64.c', 'void buf_parse_test_u64_binary ()', 'TEST_CASE(test__buf_parse_u64_binary)'],
  ['buf_parse_test_u64.c', 'void buf_parse_test_u64_octal ()', 'TEST_CASE(test__buf_parse_u64_octal)'],
  ['buf_parse_test_u64.c', 'void buf_parse_test_u64_hexadecimal ()', 'TEST_CASE(test__buf_parse_u64_hexadecimal)'],
  ['buf_parse_test_u64.c', 'void buf_parse_test_u64_decimal ()', 'TEST_CASE(test__buf_parse_u64_decimal)'],
  ['buf_parse_test_u8.c', 'void buf_parse_test_u8_binary ()', 'TEST_CASE(test__buf_parse_u8_binary)'],
  ['buf_parse_test_u8.c', 'void buf_parse_test_u8_octal ()', 'TEST_CASE(test__buf_parse_u8_octal)'],
  ['buf_parse_test_u8.c', 'void buf_parse_test_u8_hexadecimal ()', 'TEST_CASE(test__buf_parse_u8_hexadecimal)'],
  ['buf_parse_test_u8.c', 'void buf_parse_test_u8_decimal ()', 'TEST_CASE(test__buf_parse_u8_decimal)'],
  ['buf_test.c', 'void buf_test_f ()', 'TEST_CASE(test__buf_f)'],
  ['buf_test.c', 'void buf_test_ignore ()', 'TEST_CASE(test__buf_ignore)'],
  ['buf_test.c', 'void buf_test_init_clean ()', 'TEST_CASE(test__buf_init_clean)'],
  ['buf_test.c', 'void buf_test_new_delete ()', 'TEST_CASE(test__buf_new_delete)'],
  ['buf_test.c', 'void buf_test_new_alloc_delete ()', 'TEST_CASE(test__buf_new_alloc_delete)'],
  ['buf_test.c', 'void buf_test_peek_1 ()', 'TEST_CASE(test__buf_peek_1)'],
  ['buf_test.c', 'void buf_test_peek_character_utf8 ()', 'TEST_CASE(test__buf_peek_character_utf8)'],
  ['buf_test.c', 'void buf_test_peek_s8 ()', 'TEST_CASE(test__buf_peek_s8)'],
  ['buf_test.c', 'void buf_test_peek_s16 ()', 'TEST_CASE(test__buf_peek_s16)'],
  ['buf_test.c', 'void buf_test_peek_u8 ()', 'TEST_CASE(test__buf_peek_u8)'],
  ['buf_test.c', 'void buf_test_read_1 ()', 'TEST_CASE(test__buf_read_1)'],
  ['buf_test.c', 'void buf_test_read_character_utf8 ()', 'TEST_CASE(test__buf_read_character_utf8)'],
  ['buf_test.c', 'void buf_test_read_f64 ()', 'TEST_CASE(test__buf_read_f64)'],
  ['buf_test.c', 'void buf_test_read_s8 ()', 'TEST_CASE(test__buf_read_s8)'],
  ['buf_test.c', 'void buf_test_read_s16 ()', 'TEST_CASE(test__buf_read_s16)'],
  ['buf_test.c', 'void buf_test_read_to_str ()', 'TEST_CASE(test__buf_read_to_str)'],
  ['buf_test.c', 'void buf_test_read_u8 ()', 'TEST_CASE(test__buf_read_u8)'],
  ['buf_test.c', 'void buf_test_write_s8 ()', 'TEST_CASE(test__buf_write_s8)'],
  ['buf_test.c', 'void buf_test_write_s16 ()', 'TEST_CASE(test__buf_write_s16)'],
  ['buf_test.c', 'void buf_test_write_s32 ()', 'TEST_CASE(test__buf_write_s32)'],
  ['buf_test.c', 'void buf_test_write_s64 ()', 'TEST_CASE(test__buf_write_s64)'],
  ['buf_test.c', 'void buf_test_write_u8 ()', 'TEST_CASE(test__buf_write_u8)'],
  ['buf_test.c', 'void buf_test_write_u16 ()', 'TEST_CASE(test__buf_write_u16)'],
  ['buf_test.c', 'void buf_test_write_u32 ()', 'TEST_CASE(test__buf_write_u32)'],
  ['buf_test.c', 'void buf_test_write_u64 ()', 'TEST_CASE(test__buf_write_u64)'],
  ['buf_test.c', 'void buf_test_write_str ()', 'TEST_CASE(test__buf_write_str)'],
  ['buf_test.c', 'void buf_test_xfer ()', 'TEST_CASE(test__buf_xfer)'],
  ['call_test.c', 'void call_test_init_1 ()', 'TEST_CASE(test__call_init_1)'],
  ['call_test.c', 'void call_test_inspect ()', 'TEST_CASE(test__call_inspect)'],
  ['cfn_test.c', 'void cfn_test_apply ()', 'TEST_CASE(test__cfn_apply)'],
  ['cfn_test.c', 'void cfn_test_copy ()', 'TEST_CASE(test__cfn_copy)'],
  ['cfn_test.c', 'void cfn_test_init_clean ()', 'TEST_CASE(test__cfn_init_clean)'],
  ['cfn_test.c', 'void cfn_test_link ()', 'TEST_CASE(test__cfn_link)'],
  ['cfn_test.c', 'void cfn_test_prep_cif ()', 'TEST_CASE(test__cfn_prep_cif)'],
  ['character_test.c', 'void character_test_1 ()', 'TEST_CASE(test__character_1)'],
  ['character_test.c', 'void character_test_is_digit ()', 'TEST_CASE(test__character_is_digit)'],
  ['character_test.c', 'void character_test_is_lowercase ()', 'TEST_CASE(test__character_is_lowercase)'],
  ['character_test.c', 'void character_test_is_space ()', 'TEST_CASE(test__character_is_space)'],
  ['character_test.c', 'void character_test_is_uppercase ()', 'TEST_CASE(test__character_is_uppercase)'],
  ['character_test.c', 'void character_test_utf8 ()', 'TEST_CASE(test__character_utf8)'],
  ['character_test.c', 'void character_test_utf8_size ()', 'TEST_CASE(test__character_utf8_size)'],
  ['compare_test.c', 'void compare_test_bool ()', 'TEST_CASE(test__compare_bool)'],
  ['compare_test.c', 'void compare_test_character ()', 'TEST_CASE(test__compare_character)'],
  ['compare_test.c', 'void compare_test_f32 ()', 'TEST_CASE(test__compare_f32)'],
  ['compare_test.c', 'void compare_test_f64 ()', 'TEST_CASE(test__compare_f64)'],
  ['compare_test.c', 'void compare_test_list ()', 'TEST_CASE(test__compare_list)'],
  ['compare_test.c', 'void compare_test_str ()', 'TEST_CASE(test__compare_str)'],
  ['compare_test.c', 'void compare_test_tag ()', 'TEST_CASE(test__compare_tag)'],
  ['compare_test.c', 'void compare_test_tuple ()', 'TEST_CASE(test__compare_tuple)'],
  ['env_test.c', 'void env_test_eval_equal_tag ()', 'TEST_CASE(test__env_eval_equal_tag)'],
  ['env_test.c', 'void env_test_init_clean ()', 'TEST_CASE(test__env_init_clean)'],
  ['fact_test.c', 'void fact_test_init ()', 'TEST_CASE(test__fact_init)'],
  ['fact_test.c', 'void fact_test_inspect ()', 'TEST_CASE(test__fact_inspect)'],
  ['facts_cursor_test.c', 'void facts_cursor_test_init ()', 'TEST_CASE(test__facts_cursor_init)'],
  ['facts_cursor_test.c', 'void facts_cursor_test_next ()', 'TEST_CASE(test__facts_cursor_next)'],
  ['facts_test.c', 'void facts_test_add ()', 'TEST_CASE(test__facts_add)'],
  ['facts_test.c', 'void facts_test_dump_file ()', 'TEST_CASE(test__facts_dump_file)'],
  ['facts_test.c', 'void facts_test_find ()', 'TEST_CASE(test__facts_find)'],
  ['facts_test.c', 'void facts_test_init_clean ()', 'TEST_CASE(test__facts_init_clean)'],
  ['facts_test.c', 'void facts_test_load ()', 'TEST_CASE(test__facts_load)'],
  ['facts_test.c', 'void facts_test_log_add ()', 'TEST_CASE(test__facts_log_add)'],
  ['facts_test.c', 'void facts_test_log_remove ()', 'TEST_CASE(test__facts_log_remove)'],
  ['facts_test.c', 'void facts_test_new_delete ()', 'TEST_CASE(test__facts_new_delete)'],
  ['facts_test.c', 'void facts_test_open_file ()', 'TEST_CASE(test__facts_open_file)'],
  ['facts_test.c', 'void facts_test_remove ()', 'TEST_CASE(test__facts_remove)'],
  ['facts_test.c', 'void facts_test_save ()', 'TEST_CASE(test__facts_save)'],
  ['facts_with_test.c', 'void facts_with_test_ ()', 'TEST_CASE(test__facts_with_)'],
  ['facts_with_test.c', 'void facts_with_test_tags ()', 'TEST_CASE(test__facts_with_tags)'],
  ['ident_test.c', 'void ident_test_character_is_reserved ()', 'TEST_CASE(test__ident_character_is_reserved)'],
  ['ident_test.c', 'void ident_test_first_character_is_reserved ()', 'TEST_CASE(test__ident_first_character_is_reserved)'],
  ['ident_test.c', 'void ident_test_inspect ()', 'TEST_CASE(test__ident_inspect)'],
  ['list_test.c', 'void list_test_1 ()', 'TEST_CASE(test__list_1)'],
  ['list_test.c', 'void list_test_inspect ()', 'TEST_CASE(test__list_inspect)'],
  ['set__fact_test.c', 'void set__fact_test_add ()', 'TEST_CASE(test__set__fact_add)'],
  ['set__fact_test.c', 'void set__fact_test_cursor ()', 'TEST_CASE(test__set__fact_cursor)'],
  ['set__fact_test.c', 'void set__fact_test_init_clean ()', 'TEST_CASE(test__set__fact_init_clean)'],
  ['set__fact_test.c', 'void set__fact_test_remove ()', 'TEST_CASE(test__set__fact_remove)'],
  ['set__fact_test.c', 'void set__fact_test_resize ()', 'TEST_CASE(test__set__fact_resize)'],
  ['set__tag_test.c', 'void set__tag_test_add ()', 'TEST_CASE(test__set__tag_add)'],
  ['set__tag_test.c', 'void set__tag_test_cursor ()', 'TEST_CASE(test__set__tag_cursor)'],
  ['set__tag_test.c', 'void set__tag_test_init_clean ()', 'TEST_CASE(test__set__tag_init_clean)'],
  ['set__tag_test.c', 'void set__tag_test_remove ()', 'TEST_CASE(test__set__tag_remove)'],
  ['set__tag_test.c', 'void set__tag_test_resize ()', 'TEST_CASE(test__set__tag_resize)'],
  ['skiplist__fact_test.c', 'void skiplist__fact_test_find ()', 'TEST_CASE(test__skiplist__fact_find)'],
  ['skiplist__fact_test.c', 'void skiplist__fact_test_insert ()', 'TEST_CASE(test__skiplist__fact_insert)'],
  ['skiplist__fact_test.c', 'void skiplist__fact_test_new_delete ()', 'TEST_CASE(test__skiplist__fact_new_delete)'],
  ['skiplist__fact_test.c', 'void skiplist__fact_test_remove ()', 'TEST_CASE(test__skiplist__fact_remove)'],
  ['str_test.c', 'void str_test_character_is_reserved ()', 'TEST_CASE(test__str_character_is_reserved)'],
  ['str_test.c', 'void str_test_init_clean ()', 'TEST_CASE(test__str_init_clean)'],
  ['str_test.c', 'void str_test_init_dup ()', 'TEST_CASE(test__str_init_dup)'],
  ['str_test.c', 'void str_test_init_dup_1 ()', 'TEST_CASE(test__str_init_dup_1)'],
  ['str_test.c', 'void str_test_inspect ()', 'TEST_CASE(test__str_inspect)'],
  ['str_test.c', 'void str_test_new_1 ()', 'TEST_CASE(test__str_new_1)'],
  ['str_test.c', 'void str_test_new_cpy ()', 'TEST_CASE(test__str_new_cpy)'],
  ['str_test.c', 'void str_test_new_delete ()', 'TEST_CASE(test__str_new_delete)'],
  ['str_test.c', 'void str_test_new_dup ()', 'TEST_CASE(test__str_new_dup)'],
  ['str_test.c', 'void str_test_new_f ()', 'TEST_CASE(test__str_new_f)'],
  ['str_test.c', 'void str_test_to_hex ()', 'TEST_CASE(test__str_to_hex)'],
  ['str_test.c', 'void str_test_to_sym ()', 'TEST_CASE(test__str_to_sym)'],
  ['sym_test.c', 'void sym_test_1 ()', 'TEST_CASE(test__sym_1)'],
  ['sym_test.c', 'void sym_test_inspect ()', 'TEST_CASE(test__sym_inspect)'],
  ['tag_test.c', 'void tag_test_hash_u64 ()', 'TEST_CASE(test__tag_hash_u64)'],
  ['tag_test.c', 'void tag_test_init_1 ()', 'TEST_CASE(test__tag_init_1)'],
  ['tuple_test.c', 'void tuple_test_init_clean ()', 'TEST_CASE(test__tuple_init_clean)'],
  ['tuple_test.c', 'void tuple_test_init_1 ()', 'TEST_CASE(test__tuple_init_1)'],
  ['tuple_test.c', 'void tuple_test_inspect ()', 'TEST_CASE(test__tuple_inspect)'],
  ['tuple_test.c', 'void tuple_test_new_1 ()', 'TEST_CASE(test__tuple_new_1)'],
  ['tuple_test.c', 'void tuple_test_new_delete ()', 'TEST_CASE(test__tuple_new_delete)'],
  ['types_test.c', 'void types_test_sizeof ()', 'TEST_CASE(test__types_sizeof)']
]

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
