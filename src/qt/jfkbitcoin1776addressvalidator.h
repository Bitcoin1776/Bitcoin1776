// Copyright (c) 2011-2014 The JFKBitcoin1776 Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef JFKBITCOIN1776_QT_JFKBITCOIN1776ADDRESSVALIDATOR_H
#define JFKBITCOIN1776_QT_JFKBITCOIN1776ADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class JFKBitcoin1776AddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit JFKBitcoin1776AddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** JFKBitcoin1776 address widget validator, checks for a valid jfkbitcoin1776 address.
 */
class JFKBitcoin1776AddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit JFKBitcoin1776AddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // JFKBITCOIN1776_QT_JFKBITCOIN1776ADDRESSVALIDATOR_H
