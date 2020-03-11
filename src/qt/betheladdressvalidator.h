// Copyright (c) 2011-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BETHEL_QT_BETHELADDRESSVALIDATOR_H
#define BETHEL_QT_BETHELADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class BethelAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BethelAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Bethel address widget validator, checks for a valid bethel address.
 */
class BethelAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BethelAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // BETHEL_QT_BETHELADDRESSVALIDATOR_H
