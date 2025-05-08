#ifndef MDNS_SCAN_CALLBACKS_H
#define MDNS_SCAN_CALLBACKS_H

//#include <avahi-client/client.h>
#include <avahi-client/lookup.h>
//#include <avahi-common/simple-watch.h>
//#include <avahi-common/error.h>
//#include <avahi-common/defs.h>


void resolve_callback(AvahiServiceResolver *r,
                      AvahiIfIndex interface,
                      AvahiProtocol protocol,
                      AvahiResolverEvent event,
                      const char *name,
                      const char *type,
                      const char *domain,
                      const char *host_name,
                      const AvahiAddress *address,
                      uint16_t port,
                      AvahiStringList *txt,
                      AvahiLookupResultFlags flags,
                      void *userdata);

void browse_callback(AvahiServiceBrowser *b,
                     AvahiIfIndex interface,
                     AvahiProtocol protocol,
                     AvahiBrowserEvent event,
                     const char *name,
                     const char *type,
                     const char *domain,
                     AvahiLookupResultFlags flags,
                     void *userdata);

void client_callback(AvahiClient *c, AvahiClientState state, void *userdata);


#endif