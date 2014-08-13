/* This file is part of the KDE project
   Copyright (C) 2000 David Faure <faure@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "pi.h"
#include <kparts/part.h>
#include <kicon.h>
#include <kactioncollection.h>
#include <kinputdialog.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kshell.h>
#include <kapplication.h>
#include <kpluginfactory.h>
#include <kauthorized.h>
#include <kio/netaccess.h>
#include <kparts/fileinfoextension.h>
#include <kdebug.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>

#include <limits.h>


#include <sys/time.h>
#include <sys/resource.h>

Pi::Pi( QObject* parent, const QVariantList & )
    : KParts::Plugin( parent )

{
    KAction *action = actionCollection()->addAction("calculatepi");
    action->setText(i18n( "&Calculate Pi..." ));
    connect(action, SIGNAL(triggered(bool)), SLOT(calcPi()));
}

void Pi::sandbox_init()
{
    if (pipe(pipe_fd) < 0) {
        perror("pipe");
        exit(1);
    }
    if (pipe(pipe_result_fd) < 0) {
        perror("pipe");
        exit(1);
    }

    if ((child_pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

}

void Pi::calcPi()
{
    KParts::ReadOnlyPart * part = qobject_cast<KParts::ReadOnlyPart *>(parent());

    sandbox_init();

    if (child_pid){
        validator = new QRegExpValidator( QRegExp("[0-9]+"), part->widget());
        bool ok;

        qint64 total = KInputDialog::getText(i18n("Please enter loop total count"),
                                             i18n("Value of loop count"),
                                             i18n(""),
                                             &ok,
                                             part->widget(),
                                             validator,
                                             i18n(""),
                                             i18n(""),
                                             QStringList("")).toLongLong();
        if(!ok)
            return;

        // write input
        {
            close(pipe_fd[0]);
            if (write(pipe_fd[1], &total, sizeof(total)) < 0) {
                perror("write");
            }
            close(pipe_fd[1]);
        }

        //read result
        {
            qreal result=0;

            close(pipe_result_fd[1]);
            if (read(pipe_result_fd[0], &result, sizeof(result)) < 0){
                perror("read");
            }
            close(pipe_result_fd[0]);
            kDebug() << result;
            KMessageBox::information(part->widget(),i18n("Pi : %1").arg(result));
        }
    }else{
        struct rlimit rl_zero;

        rl_zero.rlim_cur = rl_zero.rlim_max = 0;

        setrlimit(RLIMIT_CPU, &rl_zero);
        setrlimit(RLIMIT_STACK, &rl_zero);

        qint64 total;
        // read
        {
            close(pipe_fd[1]);
            if (read(pipe_fd[0], &total, sizeof(total)) < 0) {
                perror("read");
            }
            close(pipe_fd[0]);
        }

        qreal pi=0;
        int count=0;

        for (int i=0; i<total; i++) {
            qreal x = (qreal)qrand() / (RAND_MAX);
            qreal y = (qreal)qrand() / (RAND_MAX);

            if (x*x+y*y <= 1) {
                count++;
            }
        }

        pi = (qreal)count / (qreal)total * 4.0;

        //write result
        {

            close(pipe_result_fd[0]);

            kDebug() << pi;
            if (write(pipe_result_fd[1], &pi, sizeof(pi)) < 0) {
                perror("write");
            }

            close(pipe_result_fd[1]);

        }

        exit(0);
    }
}

K_PLUGIN_FACTORY(KonqPiFactory, registerPlugin<Pi>();)
K_EXPORT_PLUGIN(KonqPiFactory("pi"))

#include "pi.moc"

