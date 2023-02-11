#include "outipi.h"

OutiPi::OutiPi(WhiskVice *dev) {
    setDev(dev);
    setupConnection();
}

void OutiPi::slotNewMsg(void* p,AsyncUDPPacket &packet) {
    
    Serial.print("UDP Packet Type: ");
    Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
    Serial.print(", From: ");
    Serial.print(packet.remoteIP());
    Serial.print(":");
    Serial.print(packet.remotePort());
    Serial.print(", To: ");
    Serial.print(packet.localIP());
    Serial.print(":");
    Serial.print(packet.localPort());
    Serial.print(", Length: ");
    Serial.print(packet.length());
    Serial.print(", Data: ");
    Serial.write(packet.data(), packet.length());
    Serial.println();
    //reply to the client
    packet.printf("Got %u bytes of data", packet.length());


    
    OutiPi * ptr = (OutiPi*)p;

    if(! ptr ) return;
    if( ! ptr->mDev ) return;

    MiaouMsg msg((char*)(packet.data()));
    ptr->mDev->take(msg);


}

void OutiPi::give(const MiaouMsg &msg) {

    AsyncUDP tmpSock;
    // tmpSock.connectToHost(QHostAddress::LocalHost, CATS_PORT, QIODevice::WriteOnly);

    int dataWriten = tmpSock.broadcastTo(msg.toString().c_str(), (uint16_t)CATS_PORT);
    tmpSock.close();
}
