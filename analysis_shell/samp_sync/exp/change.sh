#!/bin/bash
awk '{if ($2 <= 5 && $2 >= 0) {print NR, $2}}' m > mm
