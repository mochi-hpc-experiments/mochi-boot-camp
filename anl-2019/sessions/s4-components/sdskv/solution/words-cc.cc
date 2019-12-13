#include <assert.h>
#include <sdskv-client.hpp>
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char **argv)
{
    int ret;
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server address> <dictionary>\n", argv[0]);
        exit(0);
    }

    /* we are going to use margo/mercury to communicate */
    margo_instance_id mid = margo_init("na+sm", MARGO_CLIENT_MODE, 0, 0);
    assert(mid);

    /* fire up the 'sdskv' client library to prepare to find a server */
    sdskv::client keyval_client(mid);

    /* resolve a mercury address to a 'handle' to the provider on a specific address */
    hg_addr_t server;
    margo_addr_lookup(mid, argv[1], &server);
    sdskv::provider_handle handle = sdskv::provider_handle(keyval_client, server, 1);

    sdskv::database DB = keyval_client.open(handle, "test-db");

#define MAX_WORD 256
    char buffer[MAX_WORD];
    size_t buflen;

    FILE *dict = fopen(argv[2], "r");
    std::vector<std::string> empty;
    while(fgets(buffer, MAX_WORD, dict) != NULL) {
        std::string key(buffer);
        DB.put(key, empty);
    }

    std::string start_key;
    std::vector<std::string> output(25);
    DB.list_keys(start_key, output);

    for (auto x : output)
        std::cout << "key: " << x;



    /* shut down (every) provider at given address and clean up resources */
    keyval_client.shutdown(server);
    margo_addr_free(mid, server);

    margo_finalize(mid);
    return 0;
}
