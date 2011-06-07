/*
 * Copyright (C) 2010, 2011 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "filetransfer-handler.h"

#include <TelepathyQt4/Debug>

#include <KAboutData>
#include <KCmdLineArgs>
#include <KUniqueApplication>
#include <TelepathyQt4/ClientRegistrar>
#include <TelepathyQt4/FileTransferChannel>

int main(int argc, char* argv[])
{
    KAboutData aboutData("telepathy-kde-filetransfer-handler",
                         0,
                         ki18n("Telepathy File Transfer Handler"),
                         "0.1");
    aboutData.addAuthor(ki18n("Daniele E. Domenichelli"), ki18n("Developer"), "daniele.domenichelli@gmail.com");
    aboutData.setProductName("telepathy/filetransfer");

    KCmdLineArgs::init(argc, argv, &aboutData);
    KUniqueApplication app;

    Tp::registerTypes();
    //Enable telepathy-Qt4 debug
    Tp::enableDebug(true);
    Tp::enableWarnings(true);


    Tp::AccountFactoryPtr accountFactory = Tp::AccountFactory::create(QDBusConnection::sessionBus());

    Tp::ConnectionFactoryPtr  connectionFactory = Tp::ConnectionFactory::create(QDBusConnection::sessionBus());

    Tp::ChannelFactoryPtr channelFactory = Tp::ChannelFactory::create(QDBusConnection::sessionBus());
    channelFactory->addCommonFeatures(Tp::Channel::FeatureCore);
    channelFactory->addFeaturesForIncomingFileTransfers(Tp::FileTransferChannel::FeatureCore);
    channelFactory->addFeaturesForOutgoingFileTransfers(Tp::FileTransferChannel::FeatureCore);

    Tp::ContactFactoryPtr contactFactory = Tp::ContactFactory::create();

    Tp::ClientRegistrarPtr registrar = Tp::ClientRegistrar::create(accountFactory,
                                                                   connectionFactory,
                                                                   channelFactory,
                                                                   contactFactory);

    Tp::SharedPtr<FileTransferHandler> fth = Tp::SharedPtr<FileTransferHandler>(new FileTransferHandler);
    registrar->registerClient(Tp::AbstractClientPtr(fth),
                              QLatin1String("KDE.FileTransfer"));

    return app.exec();
}
