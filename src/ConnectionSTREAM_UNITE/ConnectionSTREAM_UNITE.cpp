/**
    @file ConnectionSTREAM_UNITE.cpp
    @author Lime Microsystems
    @brief Implementation of STREAM+UNITE board connection.
*/

#include "ConnectionSTREAM_UNITE.h"
#include "ErrorReporting.h"
using namespace std;
namespace lime
{

ConnectionSTREAM_UNITE::ConnectionSTREAM_UNITE(void* ctx, const unsigned index, const int vid, const int pid, const char* comPortName)
    : ConnectionSTREAM(ctx, index, vid, pid), comPort(nullptr)
{
    if(comPortName && strlen(comPortName))
    {
        comPort = new ConnectionEVB7COM(comPortName, 9600);
        if(comPort->IsOpen() == false)
        {
            delete comPort;
            comPort = nullptr;
        }
    }
}

ConnectionSTREAM_UNITE::~ConnectionSTREAM_UNITE(void)
{
    if(comPort)
        delete comPort;
}

DeviceInfo ConnectionSTREAM_UNITE::GetDeviceInfo(void)
{
    DeviceInfo usbInfo = ConnectionSTREAM::GetDeviceInfo();
    if(comPort)
    {
        DeviceInfo comInfo;
        comInfo = comPort->GetDeviceInfo();
        usbInfo.deviceName += std::string("+")+comInfo.deviceName;
    }
    return usbInfo;
}

int ConnectionSTREAM_UNITE::TransactSPI(const int addr, const uint32_t *writeData, uint32_t *readData, const size_t size)
{
    if(comPort && addr == 0x10)
    {
        return comPort->TransactSPI(addr, writeData, readData, size);
    }
    else
        return ConnectionSTREAM::TransactSPI(addr, writeData, readData, size);
}

int ConnectionSTREAM_UNITE::DeviceReset(void)
{
    if(comPort)
    {
        int status = comPort->DeviceReset();
        if(status != 0)
            return status;
    }
    return ConnectionSTREAM::DeviceReset();
}

int ConnectionSTREAM_UNITE::TransferPacket(GenericPacket &pkt)
{
    if(comPort && (pkt.cmd == CMD_PROG_MCU ||
                   pkt.cmd == CMD_LMS7002_WR ||
                   pkt.cmd == CMD_LMS7002_RD ))
        return comPort->TransferPacket(pkt);
    else
        return ConnectionSTREAM::TransferPacket(pkt);
}

}
