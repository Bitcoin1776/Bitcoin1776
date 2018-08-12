JFKBitcoin1776-Qt version 0.8.5 is now available from:
  http://sourceforge.net/projects/jfkbitcoin1776/files/JFKBitcoin1776/jfkbitcoin1776-0.8.5/

This is a maintenance release to fix a critical bug;
we urge all users to upgrade.

Please report bugs using the issue tracker at github:
  https://github.com/jfkbitcoin1776/jfkbitcoin1776/issues


How to Upgrade
--------------

If you are running an older version, shut it down. Wait
until it has completely shut down (which might take a few minutes for older
versions), then run the installer (on Windows) or just copy over
/Applications/JFKBitcoin1776-Qt (on Mac) or jfkbitcoin1776d/jfkbitcoin1776-qt (on Linux).

If you are upgrading from version 0.7.2 or earlier, the first time you
run 0.8.5 your blockchain files will be re-indexed, which will take
anywhere from 30 minutes to several hours, depending on the speed of
your machine.

0.8.5 Release notes
===================

Bugs fixed
----------

Transactions with version numbers larger than 0x7fffffff were
incorrectly being relayed and included in blocks.

Blocks containing transactions with version numbers larger
than 0x7fffffff caused the code that checks for LevelDB database
inconsistencies at startup to erroneously report database
corruption and suggest that you reindex your database.

This release also contains a non-critical fix to the code that
enforces BIP 34 (block height in the coinbase transaction).

--

Thanks to Gregory Maxwell and Pieter Wuille for quickly
identifying and fixing the transaction version number bug.
