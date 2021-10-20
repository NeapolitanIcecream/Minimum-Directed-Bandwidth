# Minimum Directed Bandwidth

This repository provides a plain-vanilla algorithm for a variant of the Minimum Directed Bandwidth problem, designed to provide instruction scheduling for a particular class of instruction set architectures.

此仓库提供了一个 Minimum Directed Bandwidth 问题的变体的朴素算法，旨在为一类特殊的指令集架构提供指令调度。

Such instruction sets work on a given number of registers. These registers hold the results of the previous instructions in sequence. A copy instruction is provided to propagate the result of the instruction as far as needed.

这类指令集工作在给定数目的寄存器上。这些寄存器依次保存了前面几条指令的结果。一条 copy 指令被提供，用来使指令结果在需要的时候传播得更远。
