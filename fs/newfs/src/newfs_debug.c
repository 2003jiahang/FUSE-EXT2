#include "../include/newfs.h"

extern struct newfs_super newfs_super;
extern struct custom_options newfs_options;

void newfs_dump_inode_map()
{
    int byte_cursor = 0;
    int bit_cursor = 0;

    for (byte_cursor = 0; byte_cursor < NFS_BLKS_SZ(newfs_super.ino_map_blks);
         byte_cursor += 4)
    {
        for (bit_cursor = 0; bit_cursor < UINT8_BITS; bit_cursor++)
        {
            printf("%d ", (newfs_super.map_inode[byte_cursor] & (0x1 << bit_cursor)) >> bit_cursor);
        }
        printf("\t");

        for (bit_cursor = 0; bit_cursor < UINT8_BITS; bit_cursor++)
        {
            printf("%d ", (newfs_super.map_inode[byte_cursor + 1] & (0x1 << bit_cursor)) >> bit_cursor);
        }
        printf("\t");

        for (bit_cursor = 0; bit_cursor < UINT8_BITS; bit_cursor++)
        {
            printf("%d ", (newfs_super.map_inode[byte_cursor + 2] & (0x1 << bit_cursor)) >> bit_cursor);
        }
        printf("\t");

        for (bit_cursor = 0; bit_cursor < UINT8_BITS; bit_cursor++)
        {
            printf("%d ", (newfs_super.map_inode[byte_cursor + 3] & (0x1 << bit_cursor)) >> bit_cursor);
        }
        printf("\n");
    }
}

void newfs_dump_data_map()
{
    int byte_cursor = 0;
    int bit_cursor = 0;

    for (byte_cursor = 0; byte_cursor < NFS_BLKS_SZ(newfs_super.data_map_blks);
         byte_cursor += 4)
    {
        for (bit_cursor = 0; bit_cursor < UINT8_BITS; bit_cursor++)
        {
            printf("%d ", (newfs_super.map_data[byte_cursor] & (0x1 << bit_cursor)) >> bit_cursor);
        }
        printf("\t");

        for (bit_cursor = 0; bit_cursor < UINT8_BITS; bit_cursor++)
        {
            printf("%d ", (newfs_super.map_data[byte_cursor + 1] & (0x1 << bit_cursor)) >> bit_cursor);
        }
        printf("\t");

        for (bit_cursor = 0; bit_cursor < UINT8_BITS; bit_cursor++)
        {
            printf("%d ", (newfs_super.map_data[byte_cursor + 2] & (0x1 << bit_cursor)) >> bit_cursor);
        }
        printf("\t");

        for (bit_cursor = 0; bit_cursor < UINT8_BITS; bit_cursor++)
        {
            printf("%d ", (newfs_super.map_data[byte_cursor + 3] & (0x1 << bit_cursor)) >> bit_cursor);
        }
        printf("\n");
    }
}