#!/usr/bin/env python3
"""The fuzzer module outputs random calculator expressions."""


import argparse
import random

OPERATORS = {
    "+": ("left", 100),
    "-": ("left", 100),
    "*": ("left", 200),
    "/": ("left", 200),
    "%": ("left", 200),
    "^": ("right", 300),
}


def generate_expression(complexity):
    if complexity == 0:
        return random.randint(-10, 10)
    complexity -= 1  # This node is worth 1 complexity.
    left = generate_expression(complexity // 2)
    right = generate_expression(complexity - complexity // 2)
    operator = random.choice(list(OPERATORS.keys()))
    return (operator, left, right)


def prec(expr):
    if isinstance(expr, int):
        return 1000
    return OPERATORS[expr[0]][1]


def assoc(expr):
    if isinstance(expr, int):
        return "none"
    return OPERATORS[expr[0]][0]


def show_expression(expr):
    if isinstance(expr, int):
        return str(expr)
    op, l, r = expr
    result = ""
    if prec(l) < prec(expr) or prec(l) == prec(expr) and assoc(l) == "right":
        result += "(" + show_expression(l) + ")"
    else:
        result += show_expression(l)
    result += " " + op + " "
    if prec(r) < prec(expr) or prec(r) == prec(expr) and assoc(r) == "left":
        result += "(" + show_expression(r) + ")"
    else:
        result += show_expression(r)
    return result


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-c",
        "--complexity",
        help="the complexity of the expression produced",
        default=10,
        type=int,
    )
    args = parser.parse_args()
    if args.complexity <= 0:
        print("the complexity must be a positive integer")
        return
    print(show_expression(generate_expression(args.complexity)))


if __name__ == "__main__":
    main()
