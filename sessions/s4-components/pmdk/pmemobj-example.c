/*
 * (C) 2016 The University of Chicago
 *
 * See COPYRIGHT in top-level directory.
 */

/* test program for measuring bandwidth to a pmemobj device */

#include <stdio.h>
#include <assert.h>

#include <libpmemobj.h>

int main(int argc, char **argv)
{
    PMEMobjpool *pool;
    PMEMoid oid1, oid2;
    int ret;
    char* buffer;
    char* buffer2;

    if(argc != 2)
    {
        fprintf(stderr, "Usage: pmemobj-bw <filename>\n");
        return(-1);
    }

    pool = pmemobj_open(argv[1], NULL);
    if(!pool)
    {
        fprintf(stderr, "pmemobj_open: %s\n", pmemobj_errormsg());
        return(-1);
    }

    /* create an object */
    ret = pmemobj_alloc(pool, &oid1, 1024, 0, NULL, NULL);
    if(ret != 0)
    {
        fprintf(stderr, "pmemobj_create: %s\n", pmemobj_errormsg());
        pmemobj_close(pool);
        return(-1);
    }
    printf("Created an object with oid: %lu\n", oid1.off);

    /* put a string in it and persist */
    buffer = pmemobj_direct(oid1);
    sprintf(buffer, "Hello ");
    pmemobj_persist(pool, buffer, strlen(buffer)+1);

    /* create an object */
    ret = pmemobj_alloc(pool, &oid2, 1024, 0, NULL, NULL);
    if(ret != 0)
    {
        fprintf(stderr, "pmemobj_create: %s\n", pmemobj_errormsg());
        pmemobj_close(pool);
        return(-1);
    }
    printf("Created an object with oid: %lu\n", oid2.off);

    /* put a string in it and persist */
    buffer = pmemobj_direct(oid2);
    sprintf(buffer, "World!");
    pmemobj_persist(pool, buffer, strlen(buffer)+1);

    /* close */
    pmemobj_close(pool);

    /* reopen */
    pool = pmemobj_open(argv[1], NULL);
    if(!pool)
    {
        fprintf(stderr, "pmemobj_open: %s\n", pmemobj_errormsg());
        return(-1);
    }

    buffer = pmemobj_direct(oid1);
    buffer2 = pmemobj_direct(oid2);

    printf("Checking data after reopen of device: %s%s\n", buffer, buffer2);

    /* close */
    pmemobj_close(pool);

    return(0);
}

/*
 * Local variables:
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 *
 * vim: ft=c ts=8 sts=4 sw=4 expandtab
 */
