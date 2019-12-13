#include <assert.h>
#include <sdskv-client.h>

int main(int argc, char **argv)
{
    int ret;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server address>\n", argv[0]);
        exit(0);
    }

    /* we are going to use margo/mercury to communicate */
    margo_instance_id mid = margo_init("na+sm", MARGO_CLIENT_MODE, 0, 0);
    assert(mid);

    /* fire up the 'sdskv' client library to prepare to find a server */
    sdskv_client_t client;
    ret = sdskv_client_init(mid,  &client);
    assert(ret== SDSKV_SUCCESS);

    /* resolve a mercury address to a 'handle' to the provider on a specific address */
    hg_addr_t server;
    margo_addr_lookup(mid, argv[1], &server);
    sdskv_provider_handle_t handle;
    ret = sdskv_provider_handle_create(client, server, 1, &handle);
    assert(ret== SDSKV_SUCCESS);

    /* open one of the databases hosted by that provider */
    sdskv_database_id_t db_id;
    ret = sdskv_open(handle, "test-db", &db_id);
    assert(ret== SDSKV_SUCCESS);

    int key=10;
    int value=99;
    ret = sdskv_put(handle, db_id, &key, sizeof(key), &value, sizeof(value));
    assert(ret== SDSKV_SUCCESS);

    int get_value=0;
    size_t value_size=sizeof(get_value);

    ret = sdskv_get(handle, db_id, &key, sizeof(key), &get_value, &value_size);
    assert(ret== SDSKV_SUCCESS);
    printf("key %d had value %d\n", key, get_value);

    /* shut down (every) provider at given address and clean up resources */
    sdskv_provider_handle_release(handle);
    sdskv_shutdown_service(client, server);
    margo_addr_free(mid, server);
    sdskv_client_finalize(client);
    margo_finalize(mid);
    return 0;
}
