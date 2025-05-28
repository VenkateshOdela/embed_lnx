# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# ******************************************************************************
# This program is part of the source code provided with
# "Linux Systems Programming Essentials" Course offered by TECHVEDA(c) 2020 - 2022
# Course page : https://www.techveda.org/linuxsysprg
# Trainer:  Raghu Bharadwaj
# ******************************************************************************
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# Query the scheduling attributes - the scheduling policy, RT (static)
# priority and CPU affinity - of all processes currently alive on the system.
# Just a simple wrapper around chrt and taskset.
# Tip: Pipe this o/p to grep for FIFO / RR tasks..
# Also note that a multithreaded process shows up as several same PIDs.
#  (resolve these using ps -eLf - to see actual PIDs of threads).
#

for p in $(ps -A -To pid)
do
	chrt -p $p 2>/dev/null
	taskset -p $p 2>/dev/null
done
exit 0
