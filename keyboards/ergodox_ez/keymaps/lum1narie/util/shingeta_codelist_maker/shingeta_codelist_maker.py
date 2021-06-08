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
    """
    get string that is output in shingeta
    
    ex: 'pa' -> '"pa"'
        2 -> command_list[1]
    """
    if isinstance(s, str):
        # return given string with "" wrapping
        return "\"{}\"".format(s)
    elif isinstance(s, int):
        # return (s-1)th attribute in list
        return command_list[s - 1]
    else:
        return ""


def get_command_str(command_list, i):
    """
    get command raw string used in macro definer
    """
    # length is 2 or <length of max id (if longer)>
    # ex. 156 -> 3
    #       7 -> 2
    id_str_length = max(2, len(command_list) // 10 + 1)
    # return c<id>
    # ex. 3 -> c03, 12 -> c12
    return "c{id:0{length}}".format(
        id = i + 1, length = id_str_length)

# code list will be the list
# [(code1, code2, string), ...]
# code1: smaller code of key
# code2: bigger code of key or 0xFF if not require second key
# string: string output in shingeta
def calc_shingeta_code_list(string_dict, command_list):
    # {(code1, code2): string, ...}
    # code1: code of key, non-modifier priored
    # code2: code of modifier or 0xFF if not require second key
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

    for m in modifiers:
        for (i, s) in enumerate(string_dict[m]):
            if s == "":
                # skip none-input
                continue

            if (code_mapping[i], NA_NUMBER) in shingeta_code_dict:
                if s == shingeta_code_dict[(code_mapping[i], NA_NUMBER)]:
                    # skip if modifier has no effect
                    # when compared with single tap input
                    continue
                   
            if (m, code_mapping[i]) in shingeta_code_dict:
                if s == shingeta_code_dict[(m, code_mapping[i])]:
                    # skip if reversal code has been registered
                    continue
                    
            # register code (if not skipped)
            shingeta_code_dict[(code_mapping[i], m)] = s

    # convert dict into list with formating output string
    shingeta_code_list = [(min(c[0], c[1]),
                           max(c[0], c[1]),
                           get_output_string(s, command_list))
                              for (c, s) in shingeta_code_dict.items()]
    # sort list as (<priority>: <rule>)
    # 1: code2 desc 
    # 2: code1 asc
    shingeta_code_list.sort()
    shingeta_code_list.sort(key=itemgetter(1), reverse=True)
    
    return shingeta_code_list

def output_code_csv_file(filename, code_list):
    with open(filename, 'w') as f:
        writer = csv.writer(f, lineterminator='\n')
        
        HEADER = ("code1", "code2", "str")
        writer.writerow(HEADER)
        writer.writerows(code_list)
        
def output_formed_list_c_file(filename, code_list):
    with open(filename, 'w') as f:
        line_buffer_str = ""
        
        # add last code for watchdog
        copied_code_list = code_list[:]
        copied_code_list.append((0xFF, 0xFF, '""'))
        for code in copied_code_list:
            # convert code into string
            code_str = "{{{}, {}, {}}},".format(
                hex(code[0]), hex(code[1]), code[2])

            if (max(len(line_buffer_str), C_INDENT) + len(code_str) + 1 >
                    C_LINE_MAX_LENGTH):
                # if string will spill from line buffer 
                # when add new code to it
                if code_str == "":
                    # if line buffer is empty but spill,
                    # just write new code into file
                    f.write(" " * C_INDENT + code_str + "\n")
                
                else:
                    # if line buffer is not empty, write line buffer into file,
                    # clear buffer
                    f.write(line_buffer_str + "\n")
                    
                    if (C_INDENT + len(code_str) > C_LINE_MAX_LENGTH):
                        # if new command will be spill even if it were only
                        # single command in line, then just write new code into file
                        f.write(" " * C_INDENT + code_str + "\n")
                    else:
                        # if new command can be set in next line,
                        # then set new code into buffer
                        line_buffer_str = " " * C_INDENT + code_str
            
            else:
            # if string will not spill from line buffer,
                
                if line_buffer_str == "":
                # if line buffer is empty
                # add new code to line buffer with indent
                    line_buffer_str = " " * C_INDENT + code_str
                else:
                # if line buffer is not empty
                # just add new code to line buffer
                    line_buffer_str += " " + code_str

        if line_buffer_str != "":
            # if line buffer is remain (after waching code list),
            # write line buffer into file
            f.write(line_buffer_str + "\n")
                

def output_macro_c_file(filename, command_list):
    with open(filename, 'w') as f:
        for i, macro in enumerate(command_list):
            f.write("#define {macro} \"{command_str}\"\n".format(
                macro = macro, command_str = get_command_str(command_list, i)))
                
if __name__ == "__main__":
    shingeta_code_list = calc_shingeta_code_list(
        shingeta_string_dict, MACRO_COMMAND_LIST)
    
    output_code_csv_file(CODE_CSV_FILE, shingeta_code_list)
    
    output_formed_list_c_file(FORMED_LIST_C_FILE, shingeta_code_list)

    output_macro_c_file(MACRO_C_FILE, MACRO_COMMAND_LIST)

    print("{} codes written".format(len(shingeta_code_list)))
    input("press Enter to quit")
