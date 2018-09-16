/*********************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of the QicsTable software.
**
** See the top level README file for license terms under which this
** software can be used, distributed, or modified.
**
**********************************************************************/

#include "QicsEvalDialog.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QApplication>

#include "QicsMagicDefs.h"

#include <stdio.h>
#include <stdlib.h>


void QicsEvalDialog::checkEvalLicense(const char *which)
{
    static bool hasChecked = false;
    if ( hasChecked )
        return;
    hasChecked = true;

    QString passwd = QString::fromLatin1( getenv( "ICS_EVAL_KEY" ) );


    QDate expire;
    Status status = check( passwd, &expire );
    while (status == Reject)
    {
        if( qApp && qApp->type() != QApplication::Tty )
    {
      passwd = QInputDialog::getText(0, QObject::tr( "License dialog" ),
                     QObject::tr( "Please enter license key for %1" ).arg( QString::fromLatin1(which)) );
    }
    else
    {
      printf("Please enter license key for %s: ", which );
      char buf[40];
      fgets( buf, sizeof(buf), stdin );
      passwd = QString::fromLocal8Bit(QByteArray(buf).trimmed());
    }
        if (passwd.isNull())
            exit(-1);

        status = check( passwd, &expire );
        if ( status == Reject )
    {
        if( qApp && qApp->type() != QApplication::Tty )
        {
        QMessageBox::critical( 0, QObject::tr("Invalid Key"),
                       QObject::tr("The key entered was invalid."),
                       QMessageBox::Ok, QMessageBox::NoButton );
        }
        else
        {
          printf( "%s\n",
              QObject::tr("The key entered was invalid.").toLocal8Bit().data());
        }
        }
    }

    if (status == Expired)
    {
        QMessageBox::critical(0, QObject::tr("Key Expired"),
                  QObject::tr("Evaluation period has expired.\nTo purchase %1, "
                      "please contact:\n\n"
                      "info@ics.com (for sales in the US, Canada, and Mexico),\n\n"
                      "sales@klaralvdalens-datakonsult.se (for sales in all other countries)").arg(QString::fromLatin1(which)),
                               QMessageBox::Ok, QMessageBox::NoButton);
        exit(-1);
    }
}

/*
 * This function uses the following layout for the base (unencrypted)
 * 8 byte uchar string:
 *
 * Index Meaning
 * ----- -------
 *  0    <Random value added when key is generated>
 *  1    day
 *  2    month
 *  3    year
 *  4    MAGIC1
 *  5    month
 *  6    day
 *  7    year
 *
 * To encrypt this string, the last 7 bytes of this value are XOR'ed
 * with the random number, then the whole is XOR'ed with the MAGIC2
 * array.
 * Decryption consists of XORing with MAGIC2, recovering the random
 * number from the first byte, XORing with it and verifying the
 * presence of MAGIC1 and the integrity of the matching values.
 */
QicsEvalDialog::Status QicsEvalDialog::check(const QString &password, QDate *expireReturn)
{
    unsigned char key8[8];
    unsigned char random_key, val;
    int year, month, day;
    int i;

    /*
     * Check input and build the 8 byte array
     * It's a long way to do this, but easily verifiable in code and
     * (hopefully) hard to deduce in disassembled binary.
     */
    if(password.length() != 16)
        return Reject;

    for(i=0; i<8; i++)
        key8[i] = 0;

    for(i=0; i<16; i++) {
        switch(password.at(i).toLatin1()) {
        case '0':
            val = 0;
            break;
        case '1':
            val = 1;
            break;
        case '2':
            val = 2;
            break;
        case '3':
            val = 3;
            break;
        case '4':
            val = 4;
            break;
        case '5':
            val = 5;
            break;
        case '6':
            val = 6;
            break;
        case '7':
            val = 7;
            break;
        case '8':
            val = 8;
            break;
        case '9':
            val = 9;
            break;
        case 'a':
        case 'A':
            val = 10;
            break;
        case 'b':
        case 'B':
            val = 11;
            break;
        case 'c':
        case 'C':
            val = 12;
            break;
        case 'd':
        case 'D':
            val = 13;
            break;
        case 'e':
        case 'E':
            val = 14;
            break;
        case 'f':
        case 'F':
            val = 15;
            break;
        default:
            return Reject;
    }
        if(i%2 == 0)
            key8[i / 2] += val << 4;
        else
            key8[i / 2] += val;
    }

    /*
     * XOR with MAGIC2
     */
    for(i=0; i<8; i++) {
        key8[i] ^= MAGIC2[i];
    }

    /*
     * Get random_key and XOR with it
     */
    random_key = key8[0];
    for(i=1; i<8; i++)
        key8[i] ^= random_key;

    /*
     * Check MAGIC1 and the integrity of matched values
     */
    if( key8[4] != MAGIC1 ||
        key8[1] != key8[6] ||
        key8[2] != key8[5] ||
        key8[3] != key8[7] )
    {
        return Reject;
    }

    /*
     * Now validate the date
     */
    day = key8[1]+1;
    month = key8[2]+1;
    year = key8[3] + 1997;	/* in struct tm, year 0 == 1900 */

    if(month > 12 || day > 31)
        return Reject;

    /*
     * OK, looks like a valid date, now save it in the return pointer,
     * if necessary, and check it against today's date
     */
    if(expireReturn) {
        *expireReturn = QDate( year, month, day );
    }

    if ( *expireReturn < QDate::currentDate() )
        return Expired;
    /*
     * OK, the fallthrough case:
     */
    return Accepted;
}
