import csv
from operator import itemgetter
from code import *

CODE_CSV_FILE = "shingeta_code.csv"
FORMED_LIST_C_FILE = "shingeta_code.txt"
MACRO_C_FILE = "shingeta_macro.txt"

C_INDENT = 8
C_LINE_MAX_LENGTH = 79

# def get_hash(code):
    # xor_40503 = [(x % 256) * 40503 for x in code]
    # return (((xor_40503[0] ^ xor_40503[1]) & 0xFFFF) >> (16 - HASH_DIGIT))

def get_output_string(s, command_list):
    """get string that is output in shingeta
    if string is given, return same string
    if number is given, return the string in command_list[number - 1]

    ex: 'pa' -> '"pa"'
        2 -> command_list[1] """
    if isinstance(s, str):
        # return given string with "" wrapping
        return "\"{}\"".format(s)
    elif isinstance(s, int):
        # return (s-1)th attribute in list
        return command_list[s - 1]
    else:
        return ""

def get_command_str(command_list, i):
    """ get command raw string used in macro definer in C code"""
    # length is 2 or <length of max id (if longer)>
    # ex. 156 -> 3
    #       7 -> 2
    id_str_length = max(2, len(command_list) // 10 + 1)
    # return c<id>
    # ex. 3 -> c03, 12 -> c12
    return "c{id:0{length}}".format(
        id = i + 1, length = id_str_length)

def generate_shingeta_code_list(string_dict, code_mapping, command_list):
    """generate a list of tuples which show the matching of keycodes and strings
    input:
        string_dict:
            {modifier: [string, ...], ...}
        code_mapping:
            list of keycodes
            this shows which position in a element in string_dict
            means which keycode
        command_list:
            list of the names of commands
    return:
        [(code1, code2, string), ...]
        code1: smaller code of key
        code2: bigger code of key, or NA_NUMBER if not require second key
        string: string output in shingeta """
    # {(code1, code2): string, ...}
    # code1: code of key, non-modifier priored
    # code2: code of modifier, or NA_NUMBER if not require second key
    # string: string output in shingeta
    shingeta_code_dict = dict()

    # register single tap input into shingeta_code_dict
    for (i, s) in enumerate(string_dict[NA_NUMBER]):
        # skip none-input
        if s == "":
            continue

        shingeta_code_dict[(code_mapping[i], NA_NUMBER)] = s

    # register modifier of shingeta
    modifiers = [i for i in string_dict.keys() if i != NA_NUMBER]

    # register two tap input
    for m in modifiers:
        for (i, s) in enumerate(string_dict[m]):
            code = code_mapping[i]
            # pair of code (m, code) show string s
            if s == "":
                # skip none-input
                continue

            if (code, NA_NUMBER) in shingeta_code_dict:
                if s == shingeta_code_dict[(code, NA_NUMBER)]:
                    # skip if modifier has no effect
                    # when compared with single code input
                    continue

            if (m, code) in shingeta_code_dict:
                if s == shingeta_code_dict[(m, code)]:
                    # skip if reversal code(code, m) has been registered
                    continue

            # register code (if not skipped)
            shingeta_code_dict[(code, m)] = s

    # convert dict into list with formating output string
    shingeta_code_list = [(min(c[0], c[1]),
                           max(c[0], c[1]),
                           get_output_string(s, command_list))
                              for (c, s) in shingeta_code_dict.items()]
    # TODO: think again how to sort code
    # sort list as (<priority>: <rule>)
    # 1: code2 desc
    # 2: code1 asc
    shingeta_code_list.sort()
    shingeta_code_list.sort(key=itemgetter(1), reverse=True)

    return shingeta_code_list

def output_code_csv_file(filename, code_list):
    """write csv file which show the matching of keycodes and strings
    each row of csv file will be:
        code1, code2, string"""
    with open(filename, 'w') as f:
        writer = csv.writer(f, lineterminator='\n')

        HEADER = ("code1", "code2", "str")
        writer.writerow(HEADER)
        writer.writerows(code_list)

def generate_code_shingeta_assoc_t(code):
    """convert code tuple into raw C lang code
    input:
        (code1, code2, string)
    return:
        "{<code1>, <code2>, <string>}"
        code[0-9]+ are hexdecimal nuber or NA_NUMBER_NAME
    """
    keycodes = [hex(c) if c != NA_NUMBER else NA_NUMBER_NAME for c in code[:2]]
    return "{{{}, {}, {}}}".format(keycodes[0], keycodes[1], code[2])

def indent_and_split_chunk_into_lines(chunks, line_length=80, indent_length=0, delim=","):
    """split chunks into lines within give line length
    each line will be indented with <indent_length> spaces
    each chunk splited with delim, and when length is exceed line_length,
    it will also be splited with "\n"
    """
    def __make_line(line_body):
        indent_string = " " * indent_length
        line_string = (indent_string + line_body).rstrip(" ")
        return line_string + "\n"

    retv = ""
    line_body_buffer = ""

    buffer_limit = line_length - indent_length

    for chunk in chunks:
        contents = chunk + delim

        if len(contents) >= buffer_limit:
            # if single chunk even exceed limit length
            if line_body_buffer != "":
                # if buffer have some string
                retv += __make_line(line_body_buffer)

            retv += __make_line(contents)
        elif len(line_body_buffer) + len(contents) >= buffer_limit:
            # if buffer will exceed limit length when add new chunk
            retv += __make_line(line_body_buffer)
            line_body_buffer = contents
        else:
            # if buffer have space to add new chunk
            line_body_buffer += contents

    if line_body_buffer != "":
        # if buffer have some strung reamaining include last chunk
        retv += __make_line(line_body_buffer)

    return retv

def output_formed_list_c_file(filename, code_list):
    """output code list as raw C lang code
    C lang code will be the element of the (array of shingeta_assoc_t)"""

    # add last code for watchdog
    copied_code_list = code_list[:]
    copied_code_list.append((NA_NUMBER, NA_NUMBER, '""'))

    # convert (list of code) into ()list of raw C lang string)
    code_str_list = [generate_code_shingeta_assoc_t(code) for code in copied_code_list]

    # make string that splited into lines with apropos length
    output_string = indent_and_split_chunk_into_lines(
        code_str_list, 80, C_INDENT, ", ")

    with open(filename, 'w') as f:
        f.write(output_string)


def output_macro_c_file(filename, command_list):
    """output commands as raw C lang code that deifining marco named as commands"""
    with open(filename, 'w') as f:
        for i, macro in enumerate(command_list):
            f.write("#define {macro} \"{command_str}\"\n".format(
                macro = macro, command_str = get_command_str(command_list, i)))

if __name__ == "__main__":
    shingeta_code_list = generate_shingeta_code_list(
        shingeta_ergodox_string_dict, ergodox_code_mapping, MACRO_COMMAND_LIST)

    output_code_csv_file(CODE_CSV_FILE, shingeta_code_list)

    output_formed_list_c_file(FORMED_LIST_C_FILE, shingeta_code_list)

    output_macro_c_file(MACRO_C_FILE, MACRO_COMMAND_LIST)

    print("{} codes written".format(len(shingeta_code_list)))
    print("{} macros defined".format(len(MACRO_COMMAND_LIST)))
    input("press Enter to quit")
