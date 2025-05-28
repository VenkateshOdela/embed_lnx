#!/bin/sh
# Copyright (c) 2020 Techveda
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
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

# Ref: Kernel Hackers' Guide to git
#  http://linux.yyz.us/git-howto.html , 
#  http://pradheepshrinivasan.github.io/2011/12/29/cloning-linux-next-tree-i-wanted-to-do/ 
#  http://pradheepshrinivasan.github.io/2015/08/05/Tracking-current-in-linux-next/
name=$(basename $0)

echo "${name}: !NOTE! You must specify whether you want to clone the:
 'Stable' kernel src tree (by setting STABLE_TREE=1 and LINUX_NEXT_TREE=0  in this script), 
-or-
 'linux-next' kernel src tree (by setting STABLE_TREE=0 and LINUX_NEXT_TREE=1 in this script).

Press [Enter] to continue, or ^C to exit ...
 [Enter] will cause the 'Stable' kernel to be git clone'd
"
read x

STABLE_TREE=1
LINUX_NEXT_TREE=0  # linux-next: working with the bleeding edge?

if [ ${STABLE_TREE} -eq 1 -a ${LINUX_NEXT_TREE} -eq 1 ] ; then
  echo "${name}: Both 'stable' and 'linux-next' can't be cloned, choose one of them pl.."
  exit 1
fi
if [ ${STABLE_TREE} -eq 0 -a ${LINUX_NEXT_TREE} -eq 1 ] ; then
  [ $# -ne 1 ] && {
     echo "Working with linux-next:"
	 echo "Usage: ${name} new-branch-to-work-under"
     exit 1
  }
  NEW_BRANCH=$1
fi

[ -d .git ] && {
 echo "${name}: whoops, this dir already has a .git working folder. Continue here or abort?
  [Enter] to continue, ^C to abort"
  read x
}

[ ${STABLE_TREE} -eq 1 ] && {
  GITURL=https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git
  echo "${name}: cloning 'stable' linux kernel now ... (this can take a while)..."
  echo "Running: time git clone ${GITURL}"
  time git clone ${GITURL}
}
# For 'regular': to update to latest:
# git pull ${GITURL}
# or just
# git pull

[ ${LINUX_NEXT_TREE} -eq 1 ] && {
  GITURL=git://git.kernel.org/pub/scm/linux/kernel/git/next/linux-next.git
  echo "${name}: cloning latest 'linux-next' linux kernel now ... (this can take a while)..."
  echo "Running: time git clone ${GITURL}"
  time git clone ${GITURL}
  cd linux-next || exit 1
  echo " Running: git checkout master"
  git checkout master
  echo " Running: git remote update"
  git remote update
  LATEST_TAG=$(git tag -l next-* | tail -n1)
  echo " Latest tag: ${LATEST_TAG}"
  echo " Running: git checkout -b ${NEW_BRANCH} ${LATEST_TAG}"
  git checkout -b ${NEW_BRANCH} ${LATEST_TAG}
}

echo "Done"
# Could use 'gitk' to see git repos in a GUI
# http://gitk.sourceforge.net/
# http://lostechies.com/joshuaflanagan/2010/09/03/use-gitk-to-understand-git/
