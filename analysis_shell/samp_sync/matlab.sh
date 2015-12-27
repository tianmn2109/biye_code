#!/bin/bash
awk '{print NR, $1}' slow > m
