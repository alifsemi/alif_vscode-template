import argparse
import re
import subprocess

##      80008010 :    FFEEAABB    CC001133    12345678    1A2B3C4D    ....3...xV".M<+.
_DUMP_VALUES_RE = "^[0-9a-fA-F]{8} :    ([0-9a-fA-F]{8}|[ ]{8})    ([0-9a-fA-F]{8}|[ ]{8})    ([0-9a-fA-F]{8}|[ ]{8})    ([0-9a-fA-F]{8}|[ ]{8})"

## R0  = 00000000 R1  = 00000000 R2  = FFFFFFD0 R3  = 0000FFFF
_REGISTER_VALUES_RE = "[0-9a-zA-Z]{2,3}[ ]{1,2}= ([0-9a-fA-F]{8})"

## 0x12345677
_ADDRESS_RE = "0x([0-9a-zA-Z]{8})"

## 0x2000a3e0: __bss_start__ at ... and such
_BUILTIN_SYMBOL_FINDER_RE = "\_\_[a-zA-Z]"


# -p --pretty-print      Make the output easier to read for humans
# -i --inlines           Unwind inlined functions
# -f --functions         Show function names
# -a --addresses         Show addresses
# -e --exe=<executable>  Set the input file name (default is a.out)
_ADDR2LINE_CMD = ["arm-none-eabi-addr2line", "-pifae"]


def analyse_dump(dump_file: str, elf_file: str, filter_output: bool=False, filter_zero: bool=False, filter_address: int=None):
    with open(dump_file, "r") as f:
        dump_data = f.read()
    dump_data = dump_data.splitlines()

    dump_re_object = re.compile(_DUMP_VALUES_RE)
    register_re_object = re.compile(_REGISTER_VALUES_RE)
    stack_values = []
    for line in dump_data:
        re_match = dump_re_object.match(line)
        if re_match:
            for value in re_match.groups():
                if value.isalnum():
                    stack_values.append(value)
        else:
            re_match = register_re_object.finditer(line)
            for match in re_match:
                stack_values.append(match.group(1))

    cmd = _ADDR2LINE_CMD[:] # copy the list
    cmd.append(elf_file)
    cmd.extend(stack_values)
    result = subprocess.run(cmd, capture_output=True)
    if result.returncode == 0:
        output = result.stdout.decode("UTF-8")
        symbol_re_object = re.compile(_BUILTIN_SYMBOL_FINDER_RE)
        address_re_object = re.compile(_ADDRESS_RE)
        output = output.splitlines()
        filtered = False
        for line in output:
            if filtered and line.startswith(" (inlined by)"):
                continue
            filtered = False
            filter_for_output = filter_output and ('??' in line or symbol_re_object.search(line))
            filter_for_zero = filter_zero and (line.startswith('0x00'))
            filter_for_address = filter_address is not None and line.startswith('0x') and address_re_object.match(line) and int(address_re_object.match(line).group(1), base=16) <= filter_address

            filtered = filter_for_output or filter_for_zero or filter_for_address

            if not filtered:
                print(line)

    else:
        print("running %s reported and error:" % cmd[0])
        print(result.stderr.decode("UTF-8"))


def to_int(val):
    return int(val, 16)


def main():
    parser = argparse.ArgumentParser(description="Analyses a fault dump by parsing register values and stack dump values and feeding them to arm-none-eabi-addr2line.\nOptionally filters the output.")
    parser.add_argument("dump_filename")
    parser.add_argument("elf_filename")
    parser.add_argument('-f', '--filter_output', action='store_true', help="Filter output lines that include '??' or '__[A-Za-z0-9]'.")
    parser.add_argument('-z', '--filter_zero', action='store_true', help="Filter output lines that have address lower than 0x01000000.")
    parser.add_argument('-a', '--filter_address', default=None, type=to_int, action="store", metavar="ADDRESS", help="Filter output lines that have address lower than or equal to ADDRESS in hex")
    args = parser.parse_args()
    analyse_dump(args.dump_filename, args.elf_filename, args.filter_output, args.filter_zero, args.filter_address)


if __name__ == '__main__':
    main()
