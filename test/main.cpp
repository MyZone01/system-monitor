#include <netlink/route/addr.h>
#include <netlink/route/link.h>
#include <netlink/route/rtnl.h>
#include <netlink/socket.h>
#include <iostream>

int main() {
    struct nl_sock* sock;
    struct nl_cache* link_cache, *addr_cache;
    struct nl_object* obj;
    struct rtnl_addr* addr;
    struct nl_addr* local_addr;

    sock = nl_socket_alloc();
    nl_connect(sock, NETLINK_ROUTE);

    rtnl_link_alloc_cache(sock, AF_UNSPEC, &link_cache);
    rtnl_addr_alloc_cache(sock, &addr_cache);

    nl_cache_foreach(addr_cache, [](struct nl_object* obj, void* arg) {
        struct rtnl_addr* addr = (struct rtnl_addr*)obj;
        struct nl_addr* local_addr = rtnl_addr_get_local(addr);
        char buf[INET6_ADDRSTRLEN];
        nl_addr2str(local_addr, buf, sizeof(buf));
        std::cout << "Address: " << buf << std::endl;
    }, nullptr);

    nl_cache_free(addr_cache);
    nl_cache_free(link_cache);
    nl_socket_free(sock);

    return 0;
}
