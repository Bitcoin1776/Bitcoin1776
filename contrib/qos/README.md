### QoS (Quality of service) ###

This is a Linux bash script that will set up tc to limit the outgoing bandwidth for connections to the JFKBitcoin1776 network. It limits outbound TCP traffic with a source or destination port of 8333, but not if the destination IP is within a LAN.

This means one can have an always-on jfkbitcoin1776d instance running, and another local jfkbitcoin1776d/jfkbitcoin1776-qt instance which connects to this node and receives blocks from it.
