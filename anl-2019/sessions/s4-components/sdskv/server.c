#include <assert.h>

#include <margo.h>
#include <sdskv-server.h>

int main(int argc, char **argv)
{

    margo_instance_id mid = margo_init("na+sm", MARGO_SERVER_MODE, 0, 0);
    assert(mid);

    /* let humans know where to direct clients */
    hg_addr_t my_address;
    margo_addr_self(mid, &my_address);
    char addr_str[128];
    size_t addr_str_size = 128;
    margo_addr_to_string(mid, addr_str, &addr_str_size, my_address);
    margo_addr_free(mid,my_address);
    printf("Server running at address %s\n", addr_str);


    /* connect SDSKV to margo progess engine */
    sdskv_provider_t provider;
    sdskv_provider_register(mid, 1, SDSKV_ABT_POOL_DEFAULT,
            &provider);

    /* create a "Leveldb" database named "test-db" in the current
     * directory, obliterating any prior instance */
    sdskv_database_id_t db_id;
    sdskv_config_t db_config = {
        .db_name = "test-db",
        .db_path = "",
        .db_type = KVDB_LEVELDB,
        .db_no_overwrite = 0
    };
    sdskv_provider_attach_database(provider, &db_config, &db_id);

    margo_enable_remote_shutdown(mid);
    margo_wait_for_finalize(mid);
}
