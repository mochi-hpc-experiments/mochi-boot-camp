#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <margo.h>
#include <abt-io.h>

#include "types.h"

typedef struct {
    int max_rpcs;
    int num_rpcs;
    abt_io_instance_id aiod;
    ABT_mutex mutex;
} server_data;

static void sum(hg_handle_t h);
DECLARE_MARGO_RPC_HANDLER(sum)

int main(int argc, char** argv)
{
    server_data svr_data;

    svr_data.max_rpcs = 4;
    svr_data.num_rpcs = 0;

    margo_instance_id mid = margo_init("na+sm", MARGO_SERVER_MODE, 0, 0);
    assert(mid);

    /* initialize abt-io and allocate 4 execution streams (OS threads) to
     * perform I/O
     */
    svr_data.aiod = abt_io_init(4);
    assert(svr_data.aiod);

    /* intialize a mutex lock to help coordinate concurrent RPCs */
    ABT_mutex_create(&svr_data.mutex);

    hg_addr_t my_address;
    margo_addr_self(mid, &my_address);
    char addr_str[128];
    size_t addr_str_size = 128;
    margo_addr_to_string(mid, addr_str, &addr_str_size, my_address);
    margo_addr_free(mid,my_address);
    printf("Server running at address %s\n", addr_str);

    hg_id_t rpc_id = MARGO_REGISTER(mid, "sum", sum_in_t, sum_out_t, sum);
    margo_register_data(mid, rpc_id, &svr_data, NULL);

    margo_wait_for_finalize(mid);

    /* shut down abt-io */
    abt_io_finalize(svr_data.aiod);

    ABT_mutex_free(&svr_data.mutex);

    return 0;
}

static void sum(hg_handle_t h)
{
    hg_return_t ret;
    int my_rpc_number;
    char buffer[64];
    char filename[32];
    int fd;

    sum_in_t in;
    sum_out_t out;

    margo_instance_id mid = margo_hg_handle_get_instance(h);

    const struct hg_info* info = margo_get_info(h);
    server_data* svr_data = (server_data*)margo_registered_data(mid, info->id);

    ABT_mutex_lock(svr_data->mutex);
    my_rpc_number = svr_data->num_rpcs;
    svr_data->num_rpcs++;
    ABT_mutex_unlock(svr_data->mutex);

    ret = margo_get_input(h, &in);
    assert(ret == HG_SUCCESS);

    out.ret = in.x + in.y;
    sprintf(buffer, "Computed %d + %d = %d\n",in.x,in.y,out.ret);
    sprintf(filename, "%d.log", my_rpc_number);

    /***************/
    /* write log message to a file */
    fd = abt_io_open(svr_data->aiod, filename, O_CREAT|O_TRUNC|O_WRONLY, S_IWUSR|S_IRUSR);
    assert(fd > -1);

    ret = abt_io_pwrite(svr_data->aiod, fd, buffer, strlen(buffer)+1, 0);
    assert(ret == strlen(buffer)+1);

    abt_io_close(svr_data->aiod, fd);
    /***************/

    ret = margo_respond(h, &out);
    assert(ret == HG_SUCCESS);

    ret = margo_free_input(h, &in);
    assert(ret == HG_SUCCESS);

    ret = margo_destroy(h);
    assert(ret == HG_SUCCESS);

    if(my_rpc_number == (svr_data->max_rpcs-1)) {
        margo_finalize(mid);
    }
}
DEFINE_MARGO_RPC_HANDLER(sum)
