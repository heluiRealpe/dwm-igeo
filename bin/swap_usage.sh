#!/bin/sh

free | awk '/Swap/{printf("%.2f%\n", $3/$2 * 100.0)}'

