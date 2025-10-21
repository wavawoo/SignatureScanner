# Primitive Signature Scanner in C

A first-year university coursework project. This is a simple signature scanner written in C that searches for specific byte patterns within files.

## Overview

This program scans a target file for predefined byte sequences (signatures). These signatures, along with their names and optional offsets, are stored in a separate signature file. It's a basic implementation of a technique often used in antivirus software and reverse engineering tools.

## Features

*   **File Scanning:** Scans any file provided via a path.
*   **External Signature File:** Reads signatures from a separate configuration file.
*   **Signature Definition:** Supports defining a signature's name, the byte sequence to find, and a starting offset for the search.
*   **Basic Results:** Outputs the name of the found signature and its location in the file.

## How to Build

This project is written in pure C and should be compiled with a standard C compiler. 

## Objective

The goal of this project was to implement a fundamental cybersecurity primitive: the ability to detect known patterns of data (signatures) within binary files. This demonstrates an understanding of file I/O, memory management, and string/byte manipulation in the C programming language.