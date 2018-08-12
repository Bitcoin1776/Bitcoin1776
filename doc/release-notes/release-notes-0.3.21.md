Binaries for JFKBitcoin1776 version 0.3.21 are available at:
  https://sourceforge.net/projects/jfkbitcoin1776/files/JFKBitcoin1776/jfkbitcoin1776-0.3.21/

Changes and new features from the 0.3.20 release include:

* Universal Plug and Play support.  Enable automatic opening of a port for incoming connections by running jfkbitcoin1776 or jfkbitcoin1776d with the - -upnp=1 command line switch or using the Options dialog box.

* Support for full-precision jfkbitcoin1776 amounts.  You can now send, and jfkbitcoin1776 will display, jfkbitcoin1776 amounts smaller than 0.01.  However, sending fewer than 0.01 jfkbitcoin1776s still requires a 0.01 jfkbitcoin1776 fee (so you can send 1.0001 jfkbitcoin1776s without a fee, but you will be asked to pay a fee if you try to send 0.0001).

* A new method of finding jfkbitcoin1776 nodes to connect with, via DNS A records. Use the -dnsseed option to enable.

For developers, changes to jfkbitcoin1776's remote-procedure-call API:

* New rpc command "sendmany" to send jfkbitcoin1776s to more than one address in a single transaction.

* Several bug fixes, including a serious intermittent bug that would sometimes cause jfkbitcoin1776d to stop accepting rpc requests. 

* -logtimestamps option, to add a timestamp to each line in debug.log.

* Immature blocks (newly generated, under 120 confirmations) are now shown in listtransactions.
