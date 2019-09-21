#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <mpi.h>

#include <margo.h>
#include <ssg-mpi.h>

#define TOKEN_VALUE 0xBEEF

MERCURY_GEN_PROC(token_t,
    ((uint32_t)(token)))

/* server state structure */
struct server_data
{
    margo_instance_id mid;
    ssg_group_id_t gid;
    int self_rank;
    int group_size;
    hg_id_t token_forward_rpc_id;
};

/* token forward RPC */
void token_forward(struct server_data *serv_data);
static void token_forward_recv(hg_handle_t handle);
DECLARE_MARGO_RPC_HANDLER(token_forward_recv)

/* XXX shutdown forward RPC declaration */

int main(int argc, char **argv)
{
    struct server_data serv_data;

    /**
     * Initialization
     **/
    MPI_Init(&argc, &argv);

    serv_data.mid = margo_init("na+sm", MARGO_SERVER_MODE, 0, -1);
    assert(serv_data.mid);

    ssg_init(serv_data.mid);

    /* XXX shutdown RPC registration */
    serv_data.token_forward_rpc_id = MARGO_REGISTER(serv_data.mid, "token_forward",
        token_t, void, token_forward_recv);

    margo_registered_disable_response(serv_data.mid, serv_data.token_forward_rpc_id,
        HG_TRUE);
    margo_register_data(serv_data.mid, serv_data.token_forward_rpc_id, &serv_data, NULL);

    /**
     * SSG group creation and state query
     **/
    serv_data.gid = ssg_group_create_mpi("token-ring-group", MPI_COMM_WORLD, NULL, NULL);
    assert(serv_data.gid != SSG_GROUP_ID_INVALID);

    serv_data.self_rank = ssg_get_group_self_rank(serv_data.gid);
    assert(serv_data.self_rank >= 0);
    serv_data.group_size = ssg_get_group_size(serv_data.gid);
    assert(serv_data.group_size > 0);

    /**
     * Kick off token forward on rank 0
     **/
    if (serv_data.self_rank == 0)
        token_forward(&serv_data);

    /**
     * Wait for finalize signal
     **/
    margo_wait_for_finalize(serv_data.mid);

    MPI_Finalize();

    return 0;
}

void token_forward(struct server_data *serv_data)
{
    int target_rank;
    ssg_member_id_t target_id;
    hg_addr_t target_addr;
    token_t fwd_token;
    hg_handle_t h;

    /* Get target addr using rank, working forwards through ring */
    target_rank = (serv_data->self_rank + 1) % serv_data->group_size;
    target_id = ssg_get_group_member_id_from_rank(serv_data->gid, target_rank);
    target_addr = ssg_get_group_member_addr(serv_data->gid, target_id);

    margo_thread_sleep(serv_data->mid, 1000.0);

    fwd_token.token = TOKEN_VALUE;
    printf("Member %d forwarding token %u to %d\n", serv_data->self_rank, fwd_token.token, target_rank);
    margo_create(serv_data->mid, target_addr, serv_data->token_forward_rpc_id, &h);
    margo_forward(h, &fwd_token);

    margo_destroy(h);
    return;
}

static void token_forward_recv(hg_handle_t h)
{
    token_t fwd_token;

    margo_instance_id mid = margo_hg_handle_get_instance(h);
    const struct hg_info* info = margo_get_info(h);
    struct server_data* serv_data = (struct server_data *)
        margo_registered_data(mid, info->id);

    margo_get_input(h, &fwd_token);
    printf("Member %d got token %u\n", serv_data->self_rank, fwd_token.token);
    margo_free_input(h, &fwd_token);
    margo_destroy(h);

    /* non-zero ranks forward token */
    if (serv_data->self_rank > 0)
        token_forward(serv_data);

    printf("Member %d shutting down\n", serv_data->self_rank);

    /* XXX rather than shutting down immediately, send RPCs back through
     * the ring to shutdown in reverse order (i.e., 3, 2, 1, 0) */
    ssg_group_destroy(serv_data->gid);
    ssg_finalize();
    margo_finalize(serv_data->mid);

    return;
}
DEFINE_MARGO_RPC_HANDLER(token_forward_recv)

/* XXX shutdown RPC definitions */
