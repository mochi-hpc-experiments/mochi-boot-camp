#include <assert.h>
#include <sdskv-client.h>

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
    sdskv_client_t client;
    ret = sdskv_client_init(mid,  &client);
    assert(ret== SDSKV_SUCCESS);

    /* resolve a mercury address to a 'handle' to the provider on a specific address */
    hg_addr_t server;
    margo_addr_lookup(mid, argv[1], &server);
    sdskv_provider_handle_t handle;
    ret = sdskv_provider_handle_create(client, server, 1, &handle);
    assert(ret == SDSKV_SUCCESS);

    /* what is hosted here? */
    size_t count=10;
    char *db_names[10];
    sdskv_database_id_t db_ids[10];
    ret = sdskv_list_databases(handle,  &count, db_names, db_ids);
    assert(ret == SDSKV_SUCCESS);
    for (int i=0; i< count; i++) {
        fprintf(stderr, "%d of %ld: found db %ld with name [%s]\n", i, count, db_ids[i], db_names[i]);
    }

    /* open one of the databases hosted by that provider */
    sdskv_database_id_t db_id;
    ret = sdskv_open(handle, "test-db", &db_id);
    assert(ret == SDSKV_SUCCESS);

#define MAX_WORD 256
    char buffer[MAX_WORD];
    size_t buflen;

    FILE *dict = fopen(argv[2], "r");
    while(fgets(buffer, MAX_WORD, dict) != NULL) {
        buflen = strlen(buffer);
        ret = sdskv_put(handle, db_id, &buffer, buflen, NULL, 0);
        assert(ret == SDSKV_SUCCESS);
    }


    int flag=0;
    ret = sdskv_exists(handle, db_id, "Aachen\n", strlen("Aachen\n"), &flag);
    if (ret == SDSKV_SUCCESS && flag == 1) {
        printf("Found key!\n");
    } else {
        printf("Womp womp.. key not found.  error: %d\n", ret);
    }

#if 0
#define MAX_KEYS 50
    hg_size_t max_keys=MAX_KEYS;
    char keys[MAX_KEYS][MAX_WORD];
    hg_size_t key_sizes[MAX_KEYS];
    for (int i=0; i< MAX_KEYS; i++) {
        key_sizes[i] = MAX_WORD;
    }
    ret = sdskv_list_keys(handle, db_id, NULL, MAX_WORD, (void **)&keys, key_sizes, &max_keys);

    printf("Here are %ld keys\n", max_keys);
    for (int i= 0; i< max_keys; i++) {
        printf("%s", keys[i]);
    }
#endif

    /* shut down (every) provider at given address and clean up resources */
    sdskv_provider_handle_release(handle);
    sdskv_shutdown_service(client, server);
    margo_addr_free(mid, server);
    sdskv_client_finalize(client);
    margo_finalize(mid);
    return 0;
}
