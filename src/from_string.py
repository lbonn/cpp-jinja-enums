#!/usr/bin/env python

import argparse
import clang.cindex
import jinja2
import os
import subprocess
import sys
import tempfile

from clang.cindex import CursorKind


def has_annotation(cursor):
    for child in cursor.get_children():
        if child.kind == CursorKind.ANNOTATE_ATTR:
            if child.spelling == "from_string":
                return True
    return False


def get_target_enums(tu):
    # https://clang.llvm.org/docs/AttributeReference.html#annotate
    # print tree with `clang -Xclang -ast-dump input.hh`
    return [
        c
        for c in tu.cursor.walk_preorder()
        if c.kind == CursorKind.ENUM_DECL and has_annotation(c)
    ]


def get_enum_props(ecursor):
    # compute namespace and class chain
    namespace = ''
    c = ecursor
    while c is not None:
        if c.kind == CursorKind.NAMESPACE:
            if namespace == '':
                namespace = c.spelling
            else:
                namespace = f'{c.spelling}::{namespace}'
        c = c.semantic_parent

    # iterate through fields
    fields = []
    for c in ecursor.get_children():
        if c.kind == CursorKind.ENUM_CONSTANT_DECL:
            fields.append(c.spelling)

    return {
        "namespace": namespace,
        "name": ecursor.spelling,
        "fields": fields,
    }


def generate_all_enums_code(enums, output):
    dir_path = os.path.dirname(os.path.realpath(__file__))
    env = jinja2.Environment(loader=jinja2.FileSystemLoader(dir_path))
    template = env.get_template("from_string.hh.jinja")

    props = {
        "enums": [get_enum_props(e) for e in enums],
    }

    return template.render(**props)


def write_output(code, output, should_format):
    if not should_format:
        print(code, file=output)
        return

    # note: we don't use the default context manager to control the file lifetime
    # python 3.12 has `delete_on_close`
    tmpf = tempfile.NamedTemporaryFile(mode='w', suffix='.hh', delete=False)
    try:
        print(code, file=tmpf.file)
        tmpf.close()

        subprocess.run(["clang-format", "-i", tmpf.name])

        with open(tmpf.name) as file_r:
            print(file_r.read(), file=output)
    finally:
        os.remove(tmpf.name)


def main():
    parser = argparse.ArgumentParser(prog="from_string")
    parser.add_argument("input", metavar="INPUT", type=str)
    parser.add_argument("--format", action=argparse.BooleanOptionalAction, default=True)
    parser.add_argument("-o", "--output", type=argparse.FileType('w'),
                        default=sys.stdout)
    parser.add_argument("compile_opts", nargs="*")
    args, cc_args = parser.parse_known_args()

    cc_args = ["-Wno-unknown-attributes"] + cc_args

    index = clang.cindex.Index.create()
    tu = index.parse(args.input, cc_args)

    # show compilation warnings and errors
    for d in tu.diagnostics:
        print(d, file=sys.stderr)

    enums = get_target_enums(tu)

    code = generate_all_enums_code(enums, args.output)

    write_output(code, args.output, args.format)

    return 0


if __name__ == "__main__":
    sys.exit(main())
