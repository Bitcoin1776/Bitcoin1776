#!/bin/sh

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

JFKBITCOIN1776D=${JFKBITCOIN1776D:-$SRCDIR/jfkbitcoin1776d}
JFKBITCOIN1776CLI=${JFKBITCOIN1776CLI:-$SRCDIR/jfkbitcoin1776-cli}
JFKBITCOIN1776TX=${JFKBITCOIN1776TX:-$SRCDIR/jfkbitcoin1776-tx}
JFKBITCOIN1776QT=${JFKBITCOIN1776QT:-$SRCDIR/qt/jfkbitcoin1776-qt}

[ ! -x $JFKBITCOIN1776D ] && echo "$JFKBITCOIN1776D not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
JFKVER=($($JFKBITCOIN1776CLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for jfkbitcoin1776d if --version-string is not set,
# but has different outcomes for jfkbitcoin1776-qt and jfkbitcoin1776-cli.
echo "[COPYRIGHT]" > footer.h2m
$JFKBITCOIN1776D --version | sed -n '1!p' >> footer.h2m

for cmd in $JFKBITCOIN1776D $JFKBITCOIN1776CLI $JFKBITCOIN1776TX $JFKBITCOIN1776QT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${JFKVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${JFKVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
