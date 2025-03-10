/*
;    Project:       Open Vehicle Monitor System
;    Module:        CAN logging framework
;    Date:          18th January 2018
;
;    (C) 2018       Michael Balzer
;    (C) 2019       Mark Webb-Johnson
;
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in
; all copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
; THE SOFTWARE.
*/

#ifndef __CANLOG_UDP_SERVER_H__
#define __CANLOG_UDP_SERVER_H__

#include <sdkconfig.h>
#ifdef CONFIG_OVMS_SC_GPL_MONGOOSE

#include "canlog.h"
#include "ovms_netmanager.h"
#include "ovms_mutex.h"

class udpcanlogconnection: public canlogconnection
  {
  public:
    udpcanlogconnection(canlog* logger, std::string format, canformat::canformat_serve_mode_t mode);
    virtual ~udpcanlogconnection();

  public:
    virtual void OutputMsg(CAN_log_message_t& msg, std::string &result);

  public:
    void Tickle();

  public:
#if MG_VERSION_NUMBER >= MG_VERSION_VAL(7, 0, 0)
    void * m_fd;               // Our main listening UDP socket
    struct mg_addr m_rem;      // Our remote client address
#else /* MG_VERSION_NUMBER */
    sock_t m_sock;             // Our main listening UDP socket
    struct sockaddr m_sa;      // Our remote client address
#endif /* MG_VERSION_NUMBER */
    mg_connection m_fakenc;    // A fake nc, just as an index to us
    uint32_t m_timeout;        // Our timeout
  };

class canlog_udpserver : public canlog
  {
  public:
    canlog_udpserver(std::string path, std::string format, canformat::canformat_serve_mode_t mode);
    virtual ~canlog_udpserver();

  public:
    virtual bool Open();
    virtual void Close();
    virtual std::string GetInfo();

  public:
    void MongooseHandler(struct mg_connection *nc, int ev, void *p);
    void Ticker(std::string event, void* data);

  public:
    struct mg_connection *m_mgconn;

  public:
    std::string         m_path;
  };

#endif // #ifdef CONFIG_OVMS_SC_GPL_MONGOOSE
#endif // __CANLOG_UDP_SERVER_H__
